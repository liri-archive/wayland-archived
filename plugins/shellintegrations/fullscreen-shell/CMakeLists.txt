include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../../headers"
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5WaylandClient_PRIVATE_INCLUDE_DIRS}
)

set(SOURCES
    main.cpp
    fullscreenshellintegration.cpp
    fullscreenshellsurface.cpp
)

ecm_add_qtwayland_client_protocol(SOURCES
    PROTOCOL "${CMAKE_CURRENT_SOURCE_DIR}/../../../data/protocols/wayland/fullscreen-shell-unstable-v1.xml"
    BASENAME fullscreen-shell-unstable-v1
    PREFIX zwp_
)

add_library(fullscreen-shell SHARED ${SOURCES})
target_link_libraries(fullscreen-shell
                      Qt5::WaylandClient
                      Wayland::Client
)

install(TARGETS fullscreen-shell
        DESTINATION ${PLUGIN_INSTALL_DIR}/wayland-shell-integration)
