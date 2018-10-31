/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include "huestream/HueEDK.h"

#include "support/network/http/HttpRequest.h"
#include "support/threading/QueueExecutor.h"
#include "support/threading/QueueDispatcher.h"
#include "support/threading/ThreadPool.h"

using support::GlobalQueueExecutor;
using support::GlobalQueueDispatcher;
using support::GlobalThreadPool;

namespace {
    template<typename T>
    void reset_provider() {
        if (T::get() == nullptr) {
            T::set(default_object<typename T::type>::get());
        };
    }
}

namespace huestream {

    void HueEDK::init() {
        reset_provider<GlobalThreadPool>();
        reset_provider<GlobalQueueExecutor>();
        reset_provider<GlobalQueueDispatcher>();
    }

    void HueEDK::deinit() {
        support::HttpRequest::set_http_client(nullptr);

        GlobalQueueExecutor::set(nullptr);
        GlobalQueueDispatcher::set(nullptr);
        GlobalThreadPool::set(nullptr);
    }

}  // namespace huestrean
