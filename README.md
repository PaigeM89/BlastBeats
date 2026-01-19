# BUILDING LOCALLY

1. Run `install-taglib.ps1`
2. Run `cmake --preset debug` (or `cmake --preset default` if you have a `CMakeUserPresets.json` file specifying `VCPKG_ROOT`. I am not sure if this is needed or not lol).
3. Run `cmake --build build`

# TODO

1. Reading & writing appdata to save directories