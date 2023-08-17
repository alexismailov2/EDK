build_type="Debug"
if [ -z "$1" ]; then
  echo "Build type " $build_type
else
  build_type=$1
fi

additional_cmake_options="-DCMAKE_INSTALL_PREFIX=./libhuestream_install/"
if [ -z "$2" ]; then
  echo $additional_cmake_options
else
  additional_cmake_options=$2
fi

cmake -Bbuild -DBUILD_TEST=OFF -DCMAKE_BUILD_TYPE=$build_type $additional_cmake_options
cmake --build build --target install