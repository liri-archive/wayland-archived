# System copy of Liri::Platform
if(ENABLE_ONLY_EGLDEVICEINTEGRATION)
    find_package(LiriPlatform REQUIRED)
endif()

add_subdirectory(egldeviceintegration)
