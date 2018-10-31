/*******************************************************************************
Copyright (C) 2018 Philips Lighting Holding B.V.
All Rights Reserved.
********************************************************************************/
#pragma once

#include <functional>

#include "support/util/Operation.h"

namespace support {

    /**
     * Provides functionality for executing functions.
     */
    class Executor {
    public:
        enum class OperationType {
            CANCELABLE,
            NON_CANCELABLE
        };

        /**
         * Virtual destructor
         */
        virtual ~Executor() = default;

        /**
         * Requests item execution.
         * @param invocable Function to be executed
         * @param operation_type Specifies if executor allowed to disacard this request on cancellation and destruction.
         */
        virtual support::Operation execute(std::function<void()> invocable, OperationType operation_type = OperationType::CANCELABLE) = 0;

        /**
         * Wait for all requests to be executed.
         * Blocks until there is no requests to process.
         * All incoming requests during wait_all() will be also processed.
         */
        virtual void wait_all() = 0;

        /**
         * Discards cancelable requests and waits other requests to be executed.
         * Blocks until there is no requests to process.
         * All incoming requests during cancel_all() will be also processed.
         */
        virtual void cancel_all() = 0;

        /**
         * Stops all incoming requests processing, discards cancelable requests and waits other requests to be executed.
         * Blocks until there is no requests to process.
         */
        virtual void shutdown() = 0;
    };
}  // namespace support