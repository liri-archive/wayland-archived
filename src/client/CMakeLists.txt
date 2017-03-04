include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

# Avoid warnings regarding wl_buffer being deprecated
add_definitions(-Wno-deprecated-declarations)

set(SOURCES
    buffer.cpp
    clientconnection.cpp
    compositor.cpp
    cursortheme.cpp
    keyboard.cpp
    fullscreenshell.cpp
    output.cpp
    outputconfiguration.cpp
    outputmanagement.cpp
    pointer.cpp
    region.cpp
    registry.cpp
    screencaster.cpp
    screenshooter.cpp
    seat.cpp
    shm.cpp
    shmformats_p.cpp
    shmpool.cpp
    surface.cpp
    touch.cpp
)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wayland/wayland.xml"
    BASENAME wayland
    PREFIX wl_
)
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wayland/fullscreen-shell-unstable-v1.xml"
    BASENAME fullscreen-shell-unstable-v1
    PREFIX zwp_
)
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri-screencaster.xml"
    BASENAME liri-screencaster
    PREFIX liri_
)
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri-screenshooter.xml"
    BASENAME liri-screenshooter
    PREFIX liri_
)
ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri-outputmanagement.xml"
    BASENAME liri-outputmanagement
    PREFIX liri_
)

add_library(LiriWaylandClient SHARED ${SOURCES})
generate_export_header(LiriWaylandClient EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/waylandclient/liriwaylandclient_export.h")
add_library(Liri::WaylandClient ALIAS LiriWaylandClient)

target_link_libraries(LiriWaylandClient
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Wayland::Client
        Wayland::Cursor
)

target_include_directories(
    LiriWaylandClient
        INTERFACE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../headers>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri>"
            "$<INSTALL_INTERFACE:${LIRI_INCLUDEDIR}>"
)

set_target_properties(LiriWaylandClient PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_SOVERSION}
    EXPORT_NAME WaylandClient
)

ecm_generate_headers(LiriWaylandClient_CamelCase_HEADERS
    HEADER_NAMES
        Buffer
        ClientConnection
        Compositor
        CursorTheme
        FullScreenShell
        Keyboard
        Output
        OutputConfiguration
        OutputManagement
        Pointer
        Region
        Registry
        Screencaster,Screencast
        Screenshooter,Screenshot
        Seat
        Shm
        ShmPool
        Surface
        Touch
    PREFIX
        WaylandClient
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriWaylandClient_HEADERS
)

private_headers(LiriWaylandClient_PRIVATE_HEADERS
    HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/buffer_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/clientconnection_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/cursortheme_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/keyboard_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/fullscreenshell_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/output_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/outputconfiguration_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/outputmanagement_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/pointer_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/region_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/registry_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/screencaster_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/screenshooter_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/seat_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/shm_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/shmformats_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/shmpool_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/surface_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/touch_p.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-wayland.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-fullscreen-shell-unstable-v1.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-outputmanagement.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-screencaster.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-liri-screenshooter.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-wayland-client-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-fullscreen-shell-unstable-v1-client-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-outputmanagement-client-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-screencaster-client-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-screenshooter-client-protocol.h"
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/waylandclient"
)


install(TARGETS LiriWaylandClient
        EXPORT LiriWaylandClientTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${LiriWaylandClient_CamelCase_HEADERS}
        DESTINATION ${LIRI_INCLUDEDIR}/WaylandClient
        COMPONENT Devel)

install(FILES ${LiriWaylandClient_PRIVATE_HEADERS}
        DESTINATION ${LIRI_INCLUDEDIR}/waylandclient/private
        COMPONENT Devel)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/waylandclient/liriwaylandclient_export.h"
        ${LiriWaylandClient_HEADERS}
    DESTINATION
        ${LIRI_INCLUDEDIR}/waylandclient
    COMPONENT
        Devel
)

# CMake package version
ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX LIRI
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandClientConfigVersion.cmake"
                  SOVERSION ${PROJECT_SOVERSION})

# CMake package config
set(LiriWaylandClient_CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/LiriWaylandClient")
ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/LiriWaylandClientConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandClientConfig.cmake"
                                  INSTALL_DESTINATION ${LiriWaylandClient_CMAKECONFIG_INSTALL_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandClientConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandClientConfigVersion.cmake"
        DESTINATION "${LiriWaylandClient_CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)
install(EXPORT LiriWaylandClientTargets
        DESTINATION "${LiriWaylandClient_CMAKECONFIG_INSTALL_DIR}"
        FILE LiriWaylandClientTargets.cmake NAMESPACE Liri::)

# qmake pri file
include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME LiriWaylandClient
                      LIB_NAME LiriWaylandClient
                      DEPS "core gui"
                      FILENAME_VAR PRI_FILENAME
                      INCLUDE_INSTALL_DIR "${LIRI_INCLUDEDIR}")
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

# pkg-config file
include(ECMGeneratePkgConfigFile)
#ecm_generate_pkgconfig_file(BASE_NAME LiriWaylandClient
#                            LIB_NAME LiriWaylandClient
#                            DEPS "Qt5Core Qt5Gui"
#                            FILENAME_VAR PKGCONFIG_FILENAME
#                            INCLUDE_INSTALL_DIR "${LIRI_INCLUDEDIR}"
#                            INSTALL)
