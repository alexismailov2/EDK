/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include <memory>

#include "huestream/common/http/HttpClient.h"

#include "support/network/http/_test/HttpRequestDelegator.h"
#include "support/threading/ThreadPool.h"
#include "support/util/DeleteLater.h"

#include <utility>

#define HTTP_REQUEST HttpRequestDelegator

namespace huestream {

struct IHttpClient::HttpRequest : public support::HTTP_REQUEST {
    using support::HTTP_REQUEST::HTTP_REQUEST;
};

void HttpClient::Execute(HttpRequestPtr request) {
    ExecuteAsync(request, {});
    request->WaitUntilReady();
}

HttpClient::~HttpClient() {
    while (true) {
        std::vector<HttpRequest*> active_requests;
        {
            std::lock_guard<std::mutex> lock{_data->_active_requests_mutex};
            _data->_is_shutdown = true;
            active_requests.swap(_data->_active_requests);
        }

        if (active_requests.empty()) {
            break;
        } else for (auto &&request : active_requests) {
            request->cancel();
            delete request;
        }
    }
}

void HttpClient::ExecuteAsync(HttpRequestPtr request, HttpRequestCallback callback) {
    auto thread_pool = support::GlobalThreadPool::get();
    if (!thread_pool) {
        return;
    }

    auto data = _data;
    std::lock_guard<std::mutex> lock{data->_active_requests_mutex};
    if (data->_is_shutdown) {
        return;
    }

    if (!request->StartRequest()) {
        return;
    }

    HttpRequest* req = new HttpRequest(request->GetUrl());

    if (!request->GetToken().empty()) {
        req->set_bearer_auth_header(request->GetToken());
    }

    req->set_verify_ssl(request->SslVerificationEnabled());
    req->expect_common_name(request->GetExpectedCommonName());
    req->set_trusted_certs(request->GetTrustedCertificates());

    req->do_request(request->GetMethod(), request->GetBody(), nullptr,
        [req, request, callback, thread_pool, data](const support::HttpRequestError &error, const support::IHttpResponse &response) mutable {

            request->SetSuccess(false);

            if (error.get_code() == support::HttpRequestError::HTTP_REQUEST_ERROR_CODE_SUCCESS) {
                request->SetResponse(response.get_body());
                request->SetStatusCode(response.get_status_code());
                request->SetSuccess(true);
            }

            if (callback) {
                callback(error, response);
            }

            request->FinishRequest();
            {
                std::lock_guard<std::mutex> lock{data->_active_requests_mutex};
                auto iter = std::find(data->_active_requests.begin(), data->_active_requests.end(), req);
                if (iter != data->_active_requests.end()) {
                    data->_active_requests.erase(iter);
                    support::delete_later(req);
                }
            }
        });
    data->_active_requests.emplace_back(std::move(req));
}

shared_ptr<IHttpClient::HttpRequest> HttpClient::CreateHttpRequest(const std::string& url,
                                                               int connect_timeout,
                                                               int receive_timeout,
                                                               int request_timeout,
                                                               bool enable_logging,
                                                               support::HttpRequestSecurityLevel security_level) {
    return std::make_shared<HttpRequest>(url, connect_timeout, receive_timeout, request_timeout, enable_logging, security_level);
}

}  // namespace huestream
