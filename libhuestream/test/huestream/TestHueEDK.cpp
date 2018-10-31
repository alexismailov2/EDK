/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include "huestream/HueEDK.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "support/network/http/HttpRequest.h"
#include "support/network/http/curl/CurlHttpClient.h"
#include "support/threading/QueueExecutor.h"
#include "support/threading/QueueDispatcher.h"
#include "support/threading/ThreadPool.h"

#include "huestream/HueStream.h"

using huestream::HueEDK;

template <typename T>
class TestHueEDKSupportThreads : public ::testing::Test {
public:
    void SetUp() override {
        HueEDK::deinit();
    }

    void TearDown() override {
        HueEDK::init();
    }
};

typedef ::testing::Types<
        support::GlobalQueueExecutor,
        support::GlobalQueueDispatcher,
        support::GlobalThreadPool> ThreadTypes;
TYPED_TEST_CASE(TestHueEDKSupportThreads, ThreadTypes);

TYPED_TEST(TestHueEDKSupportThreads, ThreadIsNotNull_Deinit_ThreadIsNull_Init_ThreadIsNotNull) {
    HueEDK::init();
    EXPECT_NE(TypeParam::get(), nullptr);

    HueEDK::deinit();
    EXPECT_EQ(TypeParam::get(), nullptr);

    HueEDK::init();
    EXPECT_NE(TypeParam::get(), nullptr);
}

TYPED_TEST(TestHueEDKSupportThreads, huesdk__deinit__nominal_case__shared_instance_destoyed_no_working_threads) {
    HueEDK::init();
    std::weak_ptr<typename std::remove_reference<decltype(*TypeParam::get().get())>::type> global_resource = TypeParam::get();

    huestream::HueStream stream(std::make_shared<huestream::Config>("appname", "devicename", huestream::PersistenceEncryptionKey{"key"}));
    stream.ConnectBridgeAsync();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stream.ShutDown();

    HueEDK::deinit();

    EXPECT_EQ(nullptr, global_resource.lock());
}