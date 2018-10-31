/*******************************************************************************
Copyright (C) 2018 Philips Lighting Holding B.V.
All Rights Reserved.
********************************************************************************/

#include <huestream/connect/ConnectionMonitor.h>
#include <iostream>
#include <memory>


namespace huestream {

ConnectionMonitor::ConnectionMonitor(BridgeStateCheckerPtr bridgeStateChecker) :
    _bridgeStateChecker(bridgeStateChecker),
    _running(false),
    _interval_msec(0) {
}

void ConnectionMonitor::Start(BridgePtr bridge, int interval_msec) {
    Stop();

    std::unique_lock<std::mutex> lk(_mutex);
    _running = true;
    _bridge = bridge;
    _interval_msec = interval_msec;
    _monitorThread = std::make_shared<std::thread>(&ConnectionMonitor::MonitorThread, this);
}

void ConnectionMonitor::MonitorThread() {
    while (true) {
        std::unique_lock<std::mutex> lk(_mutex);
        if (_cv.wait_for(lk, std::chrono::milliseconds(_interval_msec), [this]{return !_running;})) {
            break;
        }
        _bridgeStateChecker->Check(_bridge);
    }
}

void ConnectionMonitor::Stop() {
    std::shared_ptr<std::thread> monitorThread;
    {
        std::unique_lock<std::mutex> lk(_mutex);
        if (!_running) {
            return;
        }

        _running = false;
        _cv.notify_all();
        monitorThread.swap(_monitorThread);
    }

    if (monitorThread != nullptr) {
        monitorThread->join();
        monitorThread = nullptr;
    }
}
void ConnectionMonitor::SetFeedbackMessageCallback(std::function<void(const huestream::FeedbackMessage &)> callback) {
    _bridgeStateChecker->SetFeedbackMessageCallback(callback);
}
}  // namespace huestream
