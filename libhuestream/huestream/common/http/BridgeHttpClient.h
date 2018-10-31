/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#ifndef HUESTREAM_COMMON_HTTP_BRIDGEHTTPCLIENT_H_
#define HUESTREAM_COMMON_HTTP_BRIDGEHTTPCLIENT_H_

#include <huestream/common/http/IBridgeHttpClient.h>

#include <memory>
#include <string>

namespace huestream {

    class BridgeHttpClient : public IBridgeHttpClient {
    public:
        BridgeHttpClient(HttpClientPtr httpClient);

        HttpRequestPtr ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body = {}) override;
        void ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body, HttpRequestCallback callback) override;

    private:
        HttpRequestPtr CreateRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body) const;
        HttpRequestCallback LinkHttpClientCallbackWithBridge(BridgePtr bridge, HttpRequestCallback original_callback);
        void ExecuteRequestInternal(BridgePtr bridge, HttpRequestPtr request, HttpRequestCallback callback);

        HttpClientPtr _httpClient;
    };

}  // namespace huestream

#endif  // HUESTREAM_COMMON_HTTP_BRIDGEHTTPCLIENT_H_
