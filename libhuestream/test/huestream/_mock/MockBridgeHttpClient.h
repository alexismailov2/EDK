/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#ifndef LIBHUESTREAM_MOCKBRIDGEHTTPCLIENT_H
#define LIBHUESTREAM_MOCKBRIDGEHTTPCLIENT_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>

#include "huestream/common/http/IBridgeHttpClient.h"

using testing::_;

namespace huestream {
    class MockBridgeHttpClient : public IBridgeHttpClient {
    public:
        MOCK_METHOD4(ExecuteHttpRequest, HttpRequestPtr(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body));
        MOCK_METHOD5(ExecuteHttpRequest, void(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body, HttpRequestCallback callback));
    };

    class MockWrapperBridgeHttpClient : public IBridgeHttpClient {
    public:
        explicit MockWrapperBridgeHttpClient(const std::shared_ptr<MockBridgeHttpClient>& mock)
          : _mock(mock) {}

        HttpRequestPtr ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body = {}) override {
            return _mock->ExecuteHttpRequest(bridge, method, url, body);
        }

        void ExecuteHttpRequest(BridgePtr bridge, const std::string& method, const std::string& url, const std::string& body, HttpRequestCallback callback) override {
            _mock->ExecuteHttpRequest(bridge, method, url, body, callback);
        }

    private:
        std::shared_ptr<MockBridgeHttpClient> _mock;
    };
}

#endif //LIBHUESTREAM_MOCKBRIDGEHTTPCLIENT_H
