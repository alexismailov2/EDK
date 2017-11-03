/*******************************************************************************
 Copyright (C) 2017 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include "bridgediscovery/method/nupnp/tasks/BridgeDiscoveryNupnpTask.h"

#include <string>
#include <vector>

#include "bridgediscovery/BridgeDiscoveryConfiguration.h"
#include "bridgediscovery/BridgeDiscoveryConst.h"
#include "network/http/HttpRequestTask.h"
#include "libjson/libjson.h"
#include "logging/Log.h"


using Task = huesdk_lib::JobTask;
using huesdk_lib::HttpRequestTask;
using std::vector;

namespace {
    using huesdk::BridgeDiscoveryResult;
    using huesdk::BridgeDiscoveryConfiguration;

    vector<BridgeDiscoveryResult> parse_nupnp_response(HttpRequestError *error, IHttpResponse *response) {
        vector<BridgeDiscoveryResult> return_value;

        if (error != nullptr && response != nullptr &&
            error->get_code() == HttpRequestError::HTTP_REQUEST_ERROR_CODE_SUCCESS) {
            auto body = response->get_body();

            if (libjson::is_valid(body)) {
                JSONNode nodes = libjson::parse(body);

                for (JSONNode node : nodes) {
                    JSONNode::const_iterator it = node.find("internalipaddress");

                    if (it == node.end()) {
                        HUE_LOG << HUE_CORE << HUE_ERROR
                                << "BridgeDiscoveryNupnpTask: missing internalipaddress node" << HUE_ENDL;
                    } else {
                        return_value.emplace_back("", it->as_string(), "", "");
                    }
                }
            } else {
                HUE_LOG << HUE_CORE << HUE_ERROR << "BridgeDiscoveryNupnpTask: invalid json" << HUE_ENDL;
            }
        } else {
            HUE_LOG << HUE_CORE << HUE_ERROR << "BridgeDiscoveryNupnpTask: http request failed" << HUE_ENDL;
        }

        return return_value;
    }

    const HttpRequestTask::Options *http_options_nupnp() {
        static HttpRequestTask::Options options;

        options.connect_timeout = huesdk::bridge_discovery_const::NUPNP_HTTP_CONNECT_TIMEOUT;
        options.receive_timeout = huesdk::bridge_discovery_const::NUPNP_HTTP_RECEIVE_TIMEOUT;
        options.request_timeout = huesdk::bridge_discovery_const::NUPNP_HTTP_REQUEST_TIMEOUT;
        options.use_proxy = BridgeDiscoveryConfiguration::has_proxy_settings();
        options.proxy_address = BridgeDiscoveryConfiguration::get_proxy_address();
        options.proxy_port = static_cast<int>(BridgeDiscoveryConfiguration::get_proxy_port());

        return &options;
    }
}  // namespace

namespace huesdk {
    BridgeDiscoveryNupnpTask::BridgeDiscoveryNupnpTask(const std::string &url) : _url{url} {}

    void BridgeDiscoveryNupnpTask::execute(Task::CompletionHandler done) {
        create_job<HttpRequestTask>(_url, http_options_nupnp())->run([this, done](HttpRequestTask *task) {
            _results = parse_nupnp_response(task->get_error(), task->get_response());
            done();
        });
    }


    const vector<BridgeDiscoveryResult> &BridgeDiscoveryNupnpTask::get_result() const {
        return _results;
    }
}  // namespace huesdk