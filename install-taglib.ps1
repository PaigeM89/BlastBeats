$env:TAGLIB_DST_DIR = "$PWD/external/taglib/build"
$env:TAGLIB_INSTALL_DIR = "$PWD/external/taglib"

cd taglib
cmake -B $env:TAGLIB_DST_DIR -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

$env:Path += -join(";$env:TAGLIB_DST_DIR\taglib\Debug;",
"$env:TAGLIB_DST_DIR\bindings\c\Debug;",
"$env:VCPKG_ROOT\packages\cppunit_x64-windows\bin;",
"$env:VCPKG_ROOT\packages\utfcpp_x64-windows\bin;",
"$env:VCPKG_ROOT\packages\zlib_x64-windows\bin")
cmake --build $env:TAGLIB_DST_DIR --config Debug

cmake --install $env:TAGLIB_DST_DIR --config Debug --prefix $env:TAGLIB_INSTALL_DIR --strip
