/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#ifndef HUESTREAM_COMMON_HTTP_IBRIDGEHTTPCLIENT_H_
#define HUESTREAM_COMMON_HTTP_IBRIDGEHTTPCLIENT_H_

#include <string>
#include <memory>

#include "huestream/common/http/IHttpClient.h"
#include "huestream/common/http/HttpRequestInfo.h"

namespace huestream {

    class IBridgeHttpClient {
    public:
        virtual ~IBridgeHttpClient() = default;
        virtual HttpRequestPtr ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body = {}) = 0;
        virtual void ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body, HttpRequestCallback callback) = 0;
    };

    typedef std::shared_ptr<IBridgeHttpClient> BridgeHttpClientPtr;
}  // namespace huestream

#endif  // HUESTREAM_COMMON_HTTP_IBRIDGEHTTPCLIENT_H_
