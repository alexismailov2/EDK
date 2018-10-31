/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#pragma once

#include <string>
#include <vector>

#include "support/util/StringUtil.h"

using std::string;

namespace support {

    enum NetworkInetType {
        INET_IPV4,
        INET_IPV6
    };

    enum NetworkAdapterType {
        NETWORK_ADAPTER_TYPE_UNKNOWN,
        NETWORK_ADAPTER_TYPE_WIRED,
        NETWORK_ADAPTER_TYPE_WIRELESS,
        NETWORK_ADAPTER_TYPE_CELLULAR
    };

    class NetworkInterface {
    public:
        /**
         Construct without data
         */
        NetworkInterface();

        /**
         Construct with data
         @param ip        The ip address
                          e.g. 192.168.1.1 (IPV4),
                               2607:f0d0:1002:51::4 (IPV6)
         @param inet_type The type of the network interface: IPV4 or IPV6
         @param name      The name of the network interface, e.g. "eth0"
         @param up        Whether the network interface is enabled
         @param loopback  Whether the network interface is a loopback
         */
        NetworkInterface(
                const string& ip, NetworkInetType inet_type, const string& name, bool up, bool loopback,
                NetworkAdapterType adapter_type = NetworkAdapterType::NETWORK_ADAPTER_TYPE_UNKNOWN, bool is_connected = false);
        
        /**
         Get name
         @return The name of the network interface
         */
        const string& get_name() const;
        
        /**
         Set name
         @param name The name of the network interface
         */
        void set_name(const string& name);

        /**
         Get ip address
         @return The ip address of the network interface
         */
        const string& get_ip() const;
        
        /**
         Set ip
         @param ip Set the ip address of the network interface
         */
        void set_ip(const string& ip);
        
        /**
         Get inet type
         @return The inet type of the network interface: IPV4 or IPV6
         */
        NetworkInetType get_inet_type() const;
        
        /**
         Set inet type
         @param inet_type The inet type of the network interface: IPV4 or IPV6
         */
        void set_inet_type(NetworkInetType inet_type);
        
        /**
         Whether the network interface is enabled
         @return true if enabled, false otherwise
         */
        bool is_up() const;
        
        /**
         Set whether the netwok interface is enabled
         @param up Whether the network interface is enabled
         */
        void set_up(bool up);
        
        /**
         Whether the network interface is a loopback
         @return true if loopback, false otherwise
         */
        bool is_loopback() const;
        
        /**
         Set whether the network interface is a loopback
         @param loopback Whether the network interface is a loopback
         */
        void set_loopback(bool loopback);

        /**
         the type of the physical adapter
         @return the type of the physical adapter
         */
        NetworkAdapterType get_adapter_type() const;

        /**
         Set the type of the physical adapter
         @param adapter_type the type of the physical adapter
         */
        void set_adapter_type(NetworkAdapterType adapter_type);

        /**
         * Set whether the interface is connected
         @param is_connected Whether the network interface is connected
         */
        void set_is_connected(bool is_connected);

        /**
         * Whether the network interface is connected
         @return true if connected, false otherwise
         */
        bool get_is_connected() const;

        /**
         Whether this is a private ip according to the RFC1918, only for IPV4
         @return true when the ip is private, false otherwise
         */
        bool is_private() const;
        
    private:
        /** the name, e.g. "eth0" */
        string          _name;
        /** the ip address
            format: 
            - IPV4: 192.168.1.1
            - IPV6: 2607:f0d0:1002:51::4 */
        string _ip;
        NetworkInetType _inet_type;
        bool _up;
        bool _loopback;
        NetworkAdapterType _adapter_type;
        bool _is_connected;
    };

    inline bool operator==(const NetworkInterface& lhs, const NetworkInterface& rhs) {
        return (lhs.get_name() == rhs.get_name()) &&
               (lhs.get_ip() == rhs.get_ip()) &&
               (lhs.get_inet_type() == rhs.get_inet_type()) &&
               (lhs.is_up() == rhs.is_up()) &&
               (lhs.is_loopback() == rhs.is_loopback()) &&
               (lhs.get_adapter_type() == rhs.get_adapter_type()) &&
               (lhs.get_is_connected() == rhs.get_is_connected());
    }

    std::vector<NetworkInterface> prioritize(
            std::vector<NetworkInterface> network_interfaces);

}  // namespace support

