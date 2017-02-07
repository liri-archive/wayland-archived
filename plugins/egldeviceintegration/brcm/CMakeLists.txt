include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

set(SOURCES
    eglfsbrcmintegration.cpp
    main.cpp
)

add_library(brcm SHARED MODULE ${SOURCES})
target_link_libraries(brcm Liri::Platform BcmHost::BcmHost)

install(TARGETS brcm
        DESTINATION ${PLUGIN_INSTALL_DIR}/liri/egldeviceintegration)
