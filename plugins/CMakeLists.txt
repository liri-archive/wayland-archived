# System copy of GreenIsland::Platform
if(ENABLE_ONLY_EGLDEVICEINTEGRATION)
    find_package(GreenIsland REQUIRED)
endif()

add_subdirectory(egldeviceintegration)
