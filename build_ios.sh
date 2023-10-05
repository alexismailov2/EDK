build_type=Debug
if [ -z "$1" ]; then
  echo "build_type='$build_type'";
else
  build_type=$1
  echo "build_type='$build_type'";
fi

ios_sdk_version=14.5

# Build for native macos(x86_64 or arm64) on M1 use terminal for x86_64 or comment this part
#cmake -Bbuild_macos \
#      -DCMAKE_BUILD_TYPE=$build_type \
#      -DLIB_BUILD_MODE=SHARED \
#      -DCMAKE_VERBOSE_MAKEFILE=ON \
#      -DCMAKE_OSX_ARCHITECTURES=x86_64 \
#      -DCMAKE_TRY_COMPILE_OSX_ARCHITECTURES=x86_64 \
#      -DCMAKE_SYSTEM_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.1.sdk"
#cmake --build build_macos --target huestream -- -j 6

# Build for arm64 macos on
#cmake -Bbuild_macos_arm64 \
#      -DCMAKE_BUILD_TYPE=$build_type \
#      -DLIB_BUILD_MODE=SHARED \
#      -DCMAKE_VERBOSE_MAKEFILE=ON \
#      -DCMAKE_OSX_ARCHITECTURES=arm64 \
#      -DCMAKE_TRY_COMPILE_OSX_ARCHITECTURES=arm64 \
#      -DCMAKE_SYSTEM_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.1.sdk"
#cmake --build build_macos_arm64 --target huestream -- -j 6

# building for simulator x86_64
cmake -Bbuild_ios_sim \
      -DENABLE_EMBED_BITCODE=ON \
      -DLIB_BUILD_MODE=SHARED \
      -DAPPLE_PLATFORM=iphonesimulator \
      -DCMAKE_OSX_ARCHITECTURES=x86_64 \
      -DCMAKE_SYSTEM_NAME="iOS" \
      -DXCODE_ATTRIBUTE_INSTALL_PATH=@rpath \
      -DCMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$ios_sdk_version.sdk" \
      -DCMAKE_TRY_COMPILE_OSX_ARCHITECTURES=x86_64 \
      -DCMAKE_BUILD_TYPE=$build_type \
      -DCMAKE_VERBOSE_MAKEFILE=ON \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc" \
      -DCMAKE_CXX_COMPILER="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++" \
      -DCMAKE_SYSTEM_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$ios_sdk_version.sdk" \
      -DCFNETWORK="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$ios_sdk_version.sdk/System/Library/Frameworks/CFNetwork.framework" \
      -DFOUNDATION="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$ios_sdk_version.sdk/System/Library/Frameworks/Foundation.framework" \
      -DSECURITY="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$ios_sdk_version.sdk/System/Library/Frameworks/Security.framework"
cmake --build build_ios_sim --target huestream -- -j 6

# # building for simulator arm64
#cmake -Bbuild_ios_sim_arm64 \
#      -DENABLE_EMBED_BITCODE=ON \
#      -DLIB_BUILD_MODE=SHARED \
#      -DAPPLE_PLATFORM=iphonesimulator \
#      -DCMAKE_OSX_ARCHITECTURES=arm64 \
#      -DCMAKE_SYSTEM_NAME="iOS" \
#      -DXCODE_ATTRIBUTE_INSTALL_PATH=@rpath \
#      -DCMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator16.2.sdk" \
#      -DCMAKE_TRY_COMPILE_OSX_ARCHITECTURES=arm64 \
#      -DCMAKE_BUILD_TYPE=$build_type \
#      -DCMAKE_VERBOSE_MAKEFILE=ON \
#      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
#      -DCMAKE_C_COMPILER="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc" \
#      -DCMAKE_CXX_COMPILER="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++" \
#      -DCMAKE_SYSTEM_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator16.2.sdk" \
#      -DCFNETWORK="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator16.2.sdk/System/Library/Frameworks/CFNetwork.framework" \
#      -DFOUNDATION="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator16.2.sdk/System/Library/Frameworks/Foundation.framework" \
#      -DSECURITY="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator16.2.sdk/System/Library/Frameworks/Security.framework"
#cmake --build build_ios_sim_arm64 --target huestream -- -j 6

# # building for arm64 device
cmake -Bbuild_ios \
      -DENABLE_EMBED_BITCODE=ON \
      -DLIB_BUILD_MODE=SHARED \
      -DAPPLE_PLATFORM=iphoneos \
      -DCMAKE_OSX_ARCHITECTURES=arm64 \
      -DCMAKE_SYSTEM_NAME="iOS" \
      -DXCODE_ATTRIBUTE_INSTALL_PATH=@rpath \
      -DCMAKE_OSX_SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$ios_sdk_version.sdk" \
      -DCMAKE_TRY_COMPILE_OSX_ARCHITECTURES=arm64 \
      -DCMAKE_BUILD_TYPE=$build_type \
      -DCMAKE_VERBOSE_MAKEFILE=ON \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc" \
      -DCMAKE_CXX_COMPILER="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++" \
      -DCMAKE_SYSTEM_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$ios_sdk_version.sdk" \
      -DCFNETWORK="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$ios_sdk_version.sdk/System/Library/Frameworks/CFNetwork.framework" \
      -DFOUNDATION="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$ios_sdk_version.sdk/System/Library/Frameworks/Foundation.framework" \
      -DSECURITY="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$ios_sdk_version.sdk/System/Library/Frameworks/Security.framework"
cmake --build build_ios --target huestream -- -j 6

# combine both macos framewroks x86_64 and arm64
#mkdir -p install_macos_combined
#cp -r ./build_macos/bin/huestream.framework ./install_macos_combined/
#lipo -create "./build_macos/bin/huestream.framework/huestream" "./build_macos_arm64/bin/huestream.framework/huestream" -output "./install_macos_combined/huestream.framework/huestream"

# combine both simulator framewroks x86_64 and arm64(needed for creating XCFramework)
mkdir -p install_ios_combined
cp -r ./build_ios/bin/huestream.framework ./install_ios_combined/
lipo -create "./build_ios_sim/bin/huestream.framework/huestream" "./build_ios_sim_arm64/bin/huestream.framework/huestream" -output "./install_ios_combined/huestream.framework/huestream"

# combine everything info XCFramework
# NOTE: XCFramework does not want create with combined macos framework that is why used only ./build_macos_arm64/bin/huestream.framework)
xcodebuild -create-xcframework \
        -framework "./install_ios_combined/huestream.framework" \
        -framework "./build_ios/bin/huestream.framework" \
        -framework "./build_macos_arm64/bin/huestream.framework" \
        -output "./install_ios_combined/huestream.xcframework"
