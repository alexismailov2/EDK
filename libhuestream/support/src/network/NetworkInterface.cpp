/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include <regex>
#include <string>

#include <boost/range/algorithm/partition.hpp>

#include "support/network/NetworkInterface.h"

using std::regex;
using std::regex_search;
using std::smatch;
using std::string;

namespace support {

    NetworkInterface::NetworkInterface() {
        _inet_type = NetworkInetType::INET_IPV4;
        _up = false;
        _loopback = false;
        _adapter_type = NetworkAdapterType::NETWORK_ADAPTER_TYPE_UNKNOWN;
        _is_connected = false;
    }

    NetworkInterface::NetworkInterface(const string& ip,
                                       NetworkInetType inet_type,
                                       const string& name,
                                       bool up,
                                       bool loopback,
                                       NetworkAdapterType adapter_type,
                                       bool is_connected) :
        _name(name),
        _ip(ip),
        _inet_type(inet_type),
        _up(up),
        _loopback(loopback),
        _adapter_type(adapter_type),
        _is_connected(is_connected) {}

    const string& NetworkInterface::get_name() const {
        return _name;
    }

    void NetworkInterface::set_name(const string& name) {
        _name = string(name);
    }

    const string& NetworkInterface::get_ip() const {
        return _ip;
    }
    
    void NetworkInterface::set_ip(const string& ip) {
        _ip = string(ip);
    }
    
    NetworkInetType NetworkInterface::get_inet_type() const {
        return _inet_type;
    }
    
    void NetworkInterface::set_inet_type(NetworkInetType inet_type) {
        _inet_type = inet_type;
    }

    bool NetworkInterface::is_up() const {
        return _up;
    }

    void NetworkInterface::set_up(bool up) {
        _up = up;
    }

    bool NetworkInterface::is_loopback() const {
        return _loopback;
    }

    void NetworkInterface::set_loopback(bool loopback) {
        _loopback = loopback;
    }

    bool NetworkInterface::is_private() const {
        if (_inet_type != INET_IPV4) {
            // Only IPV4 supported for now
            return false;
        }
        
        smatch result;
        // Split the ip into pieces
        if (!regex_search(_ip, result, regex("^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})$"))) {
            return false;
        }

        // Get all bytes from the ip address
        int b1 = atoi(string(result[1].first, result[1].second).c_str());
        int b2 = atoi(string(result[2].first, result[2].second).c_str());
        //  unsigned char b3 = atoi(string(result[3].first, result[3].second).c_str())
        //  unsigned char b4 = atoi(string(result[4].first, result[4].second).c_str())

        // 10.x.x.x
        // 127.0.0.1
        if (b1 == 10
            || b1 == 127) {
            return true;
        }
        // 172.16.0.0 - 172.31.255.255
        if ((b1 == 172)
            && (b2 >= 16)
            && (b2 <= 31)) {
            return true;
        }
        // 192.168.0.0 - 192.168.255.255
        if ((b1 == 192) && (b2 == 168))
            return true;

        return false;
    }

    NetworkAdapterType NetworkInterface::get_adapter_type() const {
        return _adapter_type;
    }

    void NetworkInterface::set_adapter_type(NetworkAdapterType adapter_type) {
        _adapter_type = adapter_type;
    }

    void NetworkInterface::set_is_connected(bool is_connected) {
        _is_connected = is_connected;
    }

    bool NetworkInterface::get_is_connected() const {
        return _is_connected;
    }

    std::vector<NetworkInterface> prioritize(
            std::vector<NetworkInterface> network_interfaces) {
        boost::range::partition(network_interfaces, [](NetworkInterface interface) {
            return interface.get_adapter_type() == NetworkAdapterType::NETWORK_ADAPTER_TYPE_WIRELESS
                   && interface.get_is_connected();
        });

        return network_interfaces;
    }

}  // namespace support
