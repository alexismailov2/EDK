/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#pragma once

#include <string>
#include <vector>

#include "support/network/NetworkInterface.h"
#include "support/util/VectorOperations.h"

namespace support {

    class Network {
    public:
        /**
         @see lib/network/Network.h
         */
        static const std::vector<NetworkInterface> get_network_interfaces();

        /**
         @see lib/network/Network.h
         */
        static void set_default_network_interface(std::string ip, std::string name, NetworkInetType type);

        static bool is_wifi_connected();

    private:
        /**
         @see lib/network/Network.h
         */
        Network();

        static NetworkAdapterType get_adapter_type(const NetworkInterface& interface);
        static bool is_network_interface_connected(const NetworkInterface& interface);

        static bool _default_network_interface_set;
        static NetworkInterface _default_network_interface;
    };

}  // namespace support
