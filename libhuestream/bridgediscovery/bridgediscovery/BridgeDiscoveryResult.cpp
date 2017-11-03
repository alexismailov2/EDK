/*******************************************************************************
 Copyright (C) 2017 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include "bridgediscovery/BridgeDiscoveryResult.h"

#include <string>

using std::string;

namespace huesdk {

    BridgeDiscoveryResult::BridgeDiscoveryResult(const string &unique_id, const string &ip, const string &api_version, const string &model_id)
            : _unique_id(unique_id),
              _ip(ip),
              _api_version(api_version),
              _model_id(model_id) {
    }

    const char *BridgeDiscoveryResult::get_unique_id() const {
        return _unique_id.c_str();
    }

    const char *BridgeDiscoveryResult::get_ip() const {
        return _ip.c_str();
    }

    const char *BridgeDiscoveryResult::get_api_version() const {
        return _api_version.c_str();
    }

    const char *BridgeDiscoveryResult::get_model_id() const {
        return _model_id.c_str();
    }
}  // namespace huesdk