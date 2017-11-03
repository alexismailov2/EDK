/*******************************************************************************
 Copyright (C) 2017 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#ifndef BRIDGEDISCOVERY_BRIDGEDISCOVERYCLASSTYPE_H_
#define BRIDGEDISCOVERY_BRIDGEDISCOVERYCLASSTYPE_H_

namespace huesdk {

    typedef enum BridgeDiscoveryClassType {
        BRIDGE_DISCOVERY_CLASS_TYPE_UNDEFINED = 0,
        BRIDGE_DISCOVERY_CLASS_TYPE_UPNP   = 1,
        BRIDGE_DISCOVERY_CLASS_TYPE_NUPNP  = 2,
        BRIDGE_DISCOVERY_CLASS_TYPE_IPSCAN = 3,
    }  BridgeDiscoveryClassType;  // typedef enum BridgeDiscoveryClassType

}  // namespace huesdk

#endif  // BRIDGEDISCOVERY_BRIDGEDISCOVERYCLASSTYPE_H_