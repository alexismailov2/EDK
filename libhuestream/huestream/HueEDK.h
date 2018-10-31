/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#ifndef HUESTREAM_HUEEDK_H_
#define HUESTREAM_HUEEDK_H_

namespace huestream {

    class HueEDK {
    public:
        /*
         * Initializes the environment, restarts support threads
         */
        static void init();

        /*
         * Deinitializes the environment, stops all running support threads
         */
        static void deinit();
    };

}  // namespace huestrean

#endif  // HUESTREAM_HUEEDK_H_
