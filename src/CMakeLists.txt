include(GenerateExportHeader)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX LIRIWAYLAND
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/../headers/Liri/liriwayland_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandConfigVersion.cmake"
                  SOVERSION ${PROJECT_SOVERSION})

set(LIRI_INCLUDEDIR "${CMAKE_INSTALL_FULL_INCLUDEDIR}/Liri")

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/LiriWayland")

ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/LiriWaylandConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandConfig.cmake"
                                  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/LiriWaylandConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/../headers/Liri/liriwayland_version.h"
        DESTINATION ${LIRI_INCLUDEDIR} COMPONENT Devel)

include(GenerateHeaders)
include(PrivateHeaders)

add_subdirectory(client)
add_subdirectory(platform)
add_subdirectory(server)
