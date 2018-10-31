/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#pragma once

#include <memory>

#include "support/util/Provider.h"

namespace support {

    class OsUtil {
    public:
        virtual bool is_mobile_platform();

        virtual ~OsUtil() = default;
    };

    using OsUtilProvider = support::Provider<std::shared_ptr<OsUtil>>;
    using OsUtilScopedProvider = support::ScopedProvider<std::shared_ptr<OsUtil>>;

}  // namespace support

template<>
struct default_object<std::shared_ptr<support::OsUtil>> {
    static std::shared_ptr<support::OsUtil> get() {
        return std::make_shared<support::OsUtil>();
    }
};
