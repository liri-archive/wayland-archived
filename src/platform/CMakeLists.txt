# udev
find_package(UDev)
set_package_properties(UDev PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required for input handling from the QPA plugin")

# libinput
find_package(Libinput 0.12)
set_package_properties(Libinput PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required for input handling from the QPA plugin")

# glib
find_package(PkgConfig REQUIRED)
pkg_check_modules(glib glib-2.0 gthread-2.0 REQUIRED)

# libdl
find_package(Libdl REQUIRED)
set_package_properties(Libdl PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required to load GL functions")

# Qt5PlatformSupport
find_package(Qt5FontDatabaseSupport REQUIRED)
find_package(Qt5ThemeSupport REQUIRED)
find_package(Qt5ServiceSupport REQUIRED)
find_package(Qt5EventDispatcherSupport REQUIRED)
find_package(Qt5PlatformCompositorSupport REQUIRED)
find_package(Qt5EglSupport REQUIRED)
find_package(Freetype REQUIRED)
set_package_properties(Freetype PROPERTIES
    DESCRIPTION "A font rendering engine"
    URL "http://www.freetype.org"
    TYPE REQUIRED
    PURPOSE "Required by the QPA plugin")
find_package(Fontconfig REQUIRED)
set_package_properties(Fontconfig PROPERTIES
    URL "http://www.freedesktop.org/wiki/Software/fontconfig"
    TYPE REQUIRED
    PURPOSE "Required by the QPA plugin")

set(SOURCES
    logging.cpp
    deviceintegration/deviceintegration_p.cpp
    deviceintegration/egldeviceintegration.cpp
    deviceintegration/eglfscontext.cpp
    deviceintegration/eglfscursor.cpp
    deviceintegration/eglfsintegration.cpp
    deviceintegration/eglfsnativeinterface.cpp
    deviceintegration/eglfsoffscreenwindow.cpp
    deviceintegration/eglfsscreen.cpp
    deviceintegration/eglfswindow.cpp
    deviceintegration/eglfsxkb.cpp
    udev/udev.cpp
    udev/udevdevice.cpp
    udev/udevenumerate.cpp
    udev/udevmonitor.cpp
    logind/logind.cpp
    logind/vthandler.cpp
    libinput/libinputgesture.cpp
    libinput/libinputhandler.cpp
    libinput/libinputmanager_p.cpp
    libinput/libinputkeyboard.cpp
    libinput/libinputpointer.cpp
    libinput/libinputtouch.cpp
)

qt5_add_resources(SOURCES deviceintegration/cursor.qrc)

add_library(LiriPlatform SHARED ${SOURCES})
generate_export_header(LiriPlatform EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/platform/liriplatform_export.h")
add_library(Liri::Platform ALIAS LiriPlatform)

include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    ${xkbcommon_INCLUDE_DIRS}
    ${glib_INCLUDE_DIR}
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Libdl_INCLUDE_DIR}
)

target_link_libraries(LiriPlatform
                      Qt5::Core
                      Qt5::DBus
                      Qt5::Gui
                      EGL::EGL
                      UDev::UDev
                      ${xkbcommon_LIBRARIES}
                      Qt5FontDatabaseSupport::Qt5FontDatabaseSupport
                      Qt5ThemeSupport::Qt5ThemeSupport
                      Qt5ServiceSupport::Qt5ServiceSupport
                      Qt5EventDispatcherSupport::Qt5EventDispatcherSupport
                      Qt5PlatformCompositorSupport::Qt5PlatformCompositorSupport
                      Qt5EglSupport::Qt5EglSupport
                      ${glib_LIBRARIES}
                      Libdl::Libdl
                      Libinput::Libinput
                      Fontconfig::Fontconfig
                      ${FREETYPE_LIBRARIES}
)

if(${Qt5Gui_OPENGL_IMPLEMENTATION} STREQUAL "GL")
    target_link_libraries(LiriPlatform ${OPENGL_LIBRARIES})
else()
    target_link_libraries(LiriPlatform ${OPENGLES_LIBRARIES})
endif()

target_include_directories(
    LiriPlatform
        INTERFACE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../headers>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri>"
	    "$<INSTALL_INTERFACE:${LIRI_INCLUDEDIR}>"
)

set_target_properties(LiriPlatform PROPERTIES
    VERSION     ${PROJECT_VERSION}
    SOVERSION   ${PROJECT_SOVERSION}
    EXPORT_NAME Platform
)

ecm_generate_headers(LiriPlatform_CAMELCASE_HEADERS
    HEADER_NAMES
        EGLDeviceIntegration
        EglFSContext
        EglFSCursor
        EglFSGlobal
        EglFSIntegration
        EglFSNativeInterface
        EglFSOffscreenWindow
        EglFSScreen
        EglFSWindow
        EglFSXkb
    PREFIX
        Platform
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriPlatform_HEADERS
    RELATIVE
        deviceintegration
)

ecm_generate_headers(LiriPlatform_CAMELCASE_HEADERS
    HEADER_NAMES
        Udev
        UdevDevice
        UdevEnumerate
        UdevMonitor
    PREFIX
        Platform
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriPlatform_HEADERS
    RELATIVE
        udev
)

ecm_generate_headers(LiriPlatform_CAMELCASE_HEADERS
    HEADER_NAMES
        Logind
        VtHandler
    PREFIX
        Platform
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriPlatform_HEADERS
    RELATIVE
        logind
)

ecm_generate_headers(LiriPlatform_CAMELCASE_HEADERS
    HEADER_NAMES
        LibInputHandler
    PREFIX
        Platform
    OUTPUT_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri"
    REQUIRED_HEADERS
        LiriPlatform_HEADERS
    RELATIVE
       libinput
)

install(FILES ${LiriPlatform_CAMELCASE_HEADERS}
        DESTINATION ${LIRI_INCLUDEDIR}/Platform
        COMPONENT Devel)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/../../headers/Liri/platform/liriplatform_export.h"
        ${LiriPlatform_HEADERS}
    DESTINATION
        ${LIRI_INCLUDEDIR}/platform
    COMPONENT
        Devel
)

install(TARGETS LiriPlatform
        EXPORT LiriPlatformTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

# CMake package version
ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX LIRI
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LiriPlatformConfigVersion.cmake"
                  SOVERSION ${PROJECT_SOVERSION})

# CMake package config
set(LiriPlatform_CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/LiriPlatform")
ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/LiriPlatformConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/LiriPlatformConfig.cmake"
                                  INSTALL_DESTINATION ${LiriPlatform_CMAKECONFIG_INSTALL_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/LiriPlatformConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/LiriPlatformConfigVersion.cmake"
        DESTINATION "${LiriPlatform_CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)
install(EXPORT LiriPlatformTargets
        DESTINATION "${LiriPlatform_CMAKECONFIG_INSTALL_DIR}"
        FILE LiriPlatformTargets.cmake NAMESPACE Liri::)

# qmake pri file
include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME LiriPlatform
                      LIB_NAME LiriPlatform
                      DEPS "core dbus gui"
                      FILENAME_VAR PRI_FILENAME
                      INCLUDE_INSTALL_DIR "${LIRI_INCLUDEDIR}")
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

# pkg-config file
include(ECMGeneratePkgConfigFile)
#ecm_generate_pkgconfig_file(BASE_NAME LiriPlatform
#                            LIB_NAME LiriPlatform
#                            DEPS "Qt5Core Qt5DBus Qt5Gui xkbcommon"
#                            FILENAME_VAR PKGCONFIG_FILENAME
#                            INCLUDE_INSTALL_DIR "${LIRI_INCLUDEDIR}"
#                            INSTALL)
