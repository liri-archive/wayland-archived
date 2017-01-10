include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
)

add_definitions(-DQT_WAYLAND_COMPOSITOR_QUICK)

set(SOURCES
    plugin.cpp
)

add_library(liriwaylandserverprivateplugin SHARED ${SOURCES})
target_link_libraries(liriwaylandserverprivateplugin
                      Qt5::Qml
                      Qt5::Quick
                      Qt5::WaylandCompositor
)

install(TARGETS liriwaylandserverprivateplugin LIBRARY
        DESTINATION ${QML_INSTALL_DIR}/Liri/WaylandServer/Private)
install(FILES qmldir
        DESTINATION ${QML_INSTALL_DIR}/Liri/WaylandServer/Private)
