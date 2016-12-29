include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5WaylandCompositor_PRIVATE_INCLUDE_DIRS}
)

add_definitions(
    -Wno-deprecated-declarations
    -DQT_WAYLAND_COMPOSITOR_QUICK
)

set(SOURCES
    serverlogging_p.cpp
    core/compositorsettings.cpp
    core/diagnostic_p.cpp
    core/homeapplication.cpp
    core/quickoutput.cpp
    output/outputchangeset.cpp
    output/outputconfiguration.cpp
    output/outputmanagement.cpp
    output/quickoutputconfiguration.cpp
    screen/fakescreenbackend.cpp
    screen/nativescreenbackend.cpp
    screen/quickscreenmanager.cpp
    screen/screenbackend.cpp
    screen/screenmanager.cpp
    shell/clientwindow.cpp
    shell/clientwindowquickitem.cpp
    extensions/applicationmanager.cpp
    extensions/gtkshell.cpp
    extensions/screencaster.cpp
    extensions/screenshooter.cpp
)

ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/wayland/wayland.xml"
    BASENAME wayland
    PREFIX wl_
)
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri.xml"
    BASENAME liri
    PREFIX liri_
)
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri-screencaster.xml"
    BASENAME liri-screencaster
    PREFIX liri_
)
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri-screenshooter.xml"
    BASENAME liri-screenshooter
    PREFIX liri_
)
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/liri/liri-outputmanagement.xml"
    BASENAME liri-outputmanagement
    PREFIX liri_
)
ecm_add_qtwayland_server_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../data/protocols/gtk/gtk-shell.xml"
    BASENAME gtk
    PREFIX gtk_
)

add_library(LiriWaylandServer SHARED ${SOURCES})
generate_export_header(LiriWaylandServer EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/waylandserver/liriwaylandserver_export.h")
add_library(Liri::WaylandServer ALIAS LiriWaylandServer)

target_link_libraries(LiriWaylandServer
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Quick
        Qt5::WaylandCompositor
        Wayland::Server
    PRIVATE
        EGL::EGL
        Liri::Platform
)

if(EGL_FOUND)
    if(${Qt5Gui_OPENGL_IMPLEMENTATION} STREQUAL "GL")
        target_link_libraries(LiriWaylandServer PRIVATE ${OPENGL_LIBRARIES})
    else()
        target_link_libraries(LiriWaylandServer PRIVATE ${OPENGLES_LIBRARIES})
    endif()
endif()

target_include_directories(
    LiriWaylandServer
        INTERFACE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../headers>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri>"
            "$<INSTALL_INTERFACE:${LIRI_INCLUDEDIR}>"
)

set_target_properties(LiriWaylandServer PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_SOVERSION}
    EXPORT_NAME WaylandServer
)

ecm_generate_headers(LiriWaylandServer_CamelCase_HEADERS
    HEADER_NAMES
        CompositorSettings
        HomeApplication
        QuickOutput
    PREFIX
        WaylandServer
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriWaylandServer_HEADERS
    RELATIVE
        core
)

ecm_generate_headers(LiriWaylandServer_CamelCase_HEADERS
    HEADER_NAMES
        OutputChangeset
        OutputConfiguration
        OutputManagement
        QuickOutputConfiguration
    PREFIX
        WaylandServer
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriWaylandServer_HEADERS
    RELATIVE
        output
)

ecm_generate_headers(LiriWaylandServer_CamelCase_HEADERS
    HEADER_NAMES
        ApplicationManager
        GtkShell,GtkSurface
        Screencaster,Screencast
        Screenshooter,Screenshot
        TaskManager,TaskItem
    PREFIX
        WaylandServer
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriWaylandServer_HEADERS
    RELATIVE
        extensions
)

ecm_generate_headers(LiriWaylandServer_CamelCase_HEADERS
    HEADER_NAMES
        QuickScreenManager
        ScreenBackend,Screen
        ScreenManager
    PREFIX
        WaylandServer
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriWaylandServer_HEADERS
    RELATIVE
        screen
)

ecm_generate_headers(LiriWaylandServer_CamelCase_HEADERS
    HEADER_NAMES
        ClientWindow
        ClientWindowQuickItem
    PREFIX
        WaylandServer
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriWaylandServer_HEADERS
    RELATIVE
        shell
)

private_headers(LiriWaylandServer_PRIVATE_HEADERS
    HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/extensions/applicationmanager_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/extensions/screencaster_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/extensions/screenshooter_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/output/outputchangeset_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/output/outputconfiguration_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/output/outputmanagement_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/output/quickoutputconfiguration_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/screen/screenbackend_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/screen/screenmanager_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/shell/clientwindow_p.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/shell/clientwindowquickitem_p.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri-outputmanagement.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri-screencaster.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri-screenshooter.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-gtk.h"
        "${CMAKE_CURRENT_BINARY_DIR}/qwayland-server-liri.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-outputmanagement-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-screencaster-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-liri-screenshooter-server-protocol.h"
        "${CMAKE_CURRENT_BINARY_DIR}/wayland-gtk-shell-server-protocol.h"
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/waylandserver"
)

install(TARGETS LiriWaylandServer
        EXPORT LiriWaylandServerTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${LiriWaylandServer_CamelCase_HEADERS}
        DESTINATION ${LIRI_INCLUDEDIR}/WaylandServer
        COMPONENT Devel)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/waylandserver/liriwaylandserver_export.h"
        ${LiriWaylandServer_HEADERS}
    DESTINATION
        ${LIRI_INCLUDEDIR}/waylandserver
    COMPONENT
        Devel
)

# CMake package version
ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX LIRI
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandServerConfigVersion.cmake"
                  SOVERSION ${PROJECT_SOVERSION})

# CMake package config
set(LiriWaylandServer_CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/LiriWaylandServer")
ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/LiriWaylandServerConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandServerConfig.cmake"
                                  INSTALL_DESTINATION ${LiriWaylandServer_CMAKECONFIG_INSTALL_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandServerConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandServerConfigVersion.cmake"
        DESTINATION "${LiriWaylandServer_CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)
install(EXPORT LiriWaylandServerTargets
        DESTINATION "${LiriWaylandServer_CMAKECONFIG_INSTALL_DIR}"
        FILE LiriWaylandServerTargets.cmake NAMESPACE Liri::)

# qmake pri file
include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME LiriWaylandServer
                      LIB_NAME LiriWaylandServer
                      DEPS "core gui quick waylandcompositor"
                      FILENAME_VAR PRI_FILENAME
                      INCLUDE_INSTALL_DIR "${LIRI_INCLUDEDIR}")
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

# pkg-config file
include(ECMGeneratePkgConfigFile)
#ecm_generate_pkgconfig_file(BASE_NAME LiriWaylandServer
#                            LIB_NAME LiriWaylandServer
#                            DEPS "Qt5Core Qt5Gui Qt5Quick Qt5WaylandCompositor"
#                            FILENAME_VAR PKGCONFIG_FILENAME
#                            INCLUDE_INSTALL_DIR "${LIRI_INCLUDEDIR}"
#                            INSTALL)
