cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -DBUILD_TEST=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=./libhuestream_install/
cmake --build build --target install