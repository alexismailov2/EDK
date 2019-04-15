/*******************************************************************************
 Copyright (C) 2019 Signify Holding
 All Rights Reserved.
 ********************************************************************************/

#include "huestream/common/http/BridgeHttpClient.h"

#include <memory>
#include <utility>

#include "huestream/common/data/Bridge.h"

#include "support/threading/QueueExecutor.h"
#include "support/util/DeleteLater.h"
#include "support/network/NetworkConfiguration.h"
#include "support/logging/Log.h"

namespace huestream {
    BridgeHttpClient::BridgeHttpClient(HttpClientPtr httpClient) : _httpClient(std::move(httpClient)) {
    }

    HttpRequestPtr BridgeHttpClient::ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body) {
        if (bridge == nullptr) {
            return nullptr;
        }

        auto request = CreateRequest(bridge, method, url, body);
        ExecuteRequestInternal(std::move(bridge), request, {});

        request->WaitUntilReady();
        return request;
    }

    void BridgeHttpClient::ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body, HttpRequestCallback callback) {
        if (bridge == nullptr) {
            return;
        }

        auto request = CreateRequest(bridge, method, url, body);
        ExecuteRequestInternal(std::move(bridge), std::move(request), std::move(callback));
    }

    HttpRequestPtr BridgeHttpClient::CreateRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body) const {
        auto request = std::make_shared<HttpRequestInfo>(method, url, body);

        if (!bridge->GetIsUsingSsl() || bridge->GetCertificate().empty()) {
            request->SetSslVerificationEnabled(false);
        } else {
            auto trusted_certificates = support::NetworkConfiguration::get_root_certificates();
            trusted_certificates.push_back(bridge->GetCertificate());
            request->SetTrustedCertificates(trusted_certificates);
            request->SetExpectedCommonName(support::to_lower_case(bridge->GetId()));
            request->SetSslVerificationEnabled(true);
        }

        return request;
    }

    HttpRequestCallback BridgeHttpClient::LinkHttpClientCallbackWithBridge(BridgePtr bridge, HttpRequestCallback original_callback) {
        return [bridge, original_callback](const support::HttpRequestError& error, const support::IHttpResponse& response) {
            auto&& chain = response.get_certificate_chain();
            if (bridge->GetIsUsingSsl() && bridge->GetCertificate().empty() && !chain.empty()) {
                bridge->SetCertificate(chain.back());
            }

            if (original_callback) {
                original_callback(error, response);
            }
        };
    }

    void BridgeHttpClient::ExecuteRequestInternal(BridgePtr bridge, HttpRequestPtr request, HttpRequestCallback callback) {
        if (!request->SslVerificationEnabled()) {
            HUE_LOG << HUE_CORE << HUE_DEBUG << "BridgeHttpClient: Pinning: Connecting to a bridge without a pinned certificate: SSL verification is disabled." << HUE_ENDL;
        } else {
            HUE_LOG << HUE_CORE << HUE_DEBUG << "BridgeHttpClient: Pinning: Using common name for pinning: " << request->GetExpectedCommonName() << HUE_ENDL;
            HUE_LOG << HUE_CORE << HUE_DEBUG << "BridgeHttpClient: Pinning: Using persisted certificate for bridge " << bridge->GetId() << ", certificate: " << bridge->GetCertificate() << HUE_ENDL;
        }
        _httpClient->ExecuteAsync(request, LinkHttpClientCallbackWithBridge(std::move(bridge), std::move(callback)));
    }
}  // namespace huestream
