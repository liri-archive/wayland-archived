# libdl
find_package(Libdl REQUIRED)
set_package_properties(Libdl PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required to load GL functions")

include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../../headers"
    ${Qt5PlatformSupport_INCLUDE_DIR}
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

add_definitions(-DQT_EGL_WAYLAND)

set(SOURCES
    eglfswaylandblitter.cpp
    eglfswaylandcontext.cpp
    eglfswaylandinput.cpp
    eglfswaylandintegration.cpp
    eglfswaylandscreen.cpp
    eglfswaylandwindow.cpp
    main.cpp
)

include(ECMQtDeclareLoggingCategory)
ecm_qt_declare_logging_category(SOURCES
    HEADER eglfswaylandlogging.h
    IDENTIFIER Liri::Platform::gLcEglFSWayland
    CATEGORY_NAME liri.qpa.wayland
    DEFAULT_SEVERITY Debug
)

add_library(wayland MODULE ${SOURCES})
target_link_libraries(wayland
                      Liri::Platform
                      Liri::WaylandClient
                      Libdl::Libdl
                      Wayland::Egl
)

install(TARGETS wayland
        DESTINATION ${PLUGIN_INSTALL_DIR}/liri/egldeviceintegration)
