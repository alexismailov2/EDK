cmake_minimum_required(VERSION 3.6)

include(ExternalProjectUtils)

set_external_library(mdns_responder)

if(NOT EXISTS ${LIBRARY_PATH})
    set_library_path(mdns_responder MDNS_RESPONDER_PATH "")

    set(EXTERNAL_DEPENDENCY external_mdns_responder)

    ExternalProject_Add(${EXTERNAL_DEPENDENCY}
            PREFIX ${EXTERNAL_LIBRARIES_BUILD_PATH}
            URL ${mdns_responder_URL}
            UPDATE_COMMAND ""
            PATCH_COMMAND git apply --whitespace=nowarn --ignore-space-change --ignore-whitespace --no-index --directory=3rd_party/mdns_responder ${CMAKE_SOURCE_DIR}/cmake/patches/mdns_responder.patch
            SOURCE_DIR "${EXTERNAL_LIBRARIES_SOURCE_PATH}/mdns_responder"
            CMAKE_ARGS ${COMMON_ARGS} -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} -DINSTALL_DESTINATION=include/mdns_responder -DCMAKE_PREFIX_PATH=${EXTERNAL_LIBRARIES_INSTALL_PATH} -DCMAKE_INSTALL_PREFIX=${EXTERNAL_LIBRARIES_INSTALL_PATH} -DFOUNDATION=${FOUNDATION} -DANDROID_TOOLCHAIN_PREFIX=${ANDROID_TOOLCHAIN_PREFIX} -DAPPLE_PLATFORM=${APPLE_PLATFORM}
            BUILD_BYPRODUCTS ${MDNS_RESPONDER_PATH}
            LIST_SEPARATOR ^^
            )
endif(NOT EXISTS ${LIBRARY_PATH})

if(WIN32)
    file(MAKE_DIRECTORY ${EXTERNAL_LIBRARIES_INSTALL_PATH}/include/mdns_responder/mDNSShared)
    file(MAKE_DIRECTORY ${EXTERNAL_LIBRARIES_INSTALL_PATH}/include/mdns_responder/mDNSCore)
    list(APPEND LIBRARY_INCLUDE ${EXTERNAL_LIBRARIES_INSTALL_PATH}/include/mdns_responder/mDNSShared)
    list(APPEND LIBRARY_INCLUDE ${EXTERNAL_LIBRARIES_INSTALL_PATH}/include/mdns_responder/mDNSCore)
    list(APPEND LINK_DEPENDENCY netapi32)
endif(WIN32)

expose_external_library()
