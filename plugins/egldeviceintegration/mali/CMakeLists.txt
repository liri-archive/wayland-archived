include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

set(SOURCES
    eglfsmaliintegration.cpp
    main.cpp
)

add_library(mali SHARED MODULE ${SOURCES})
target_link_libraries(mali
    Liri::Platform
)

install(TARGETS mali
        DESTINATION ${PLUGIN_INSTALL_DIR}/liri/egldeviceintegration)
