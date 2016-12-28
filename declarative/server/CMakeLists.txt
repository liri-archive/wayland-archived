include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
)

set(SOURCES
    plugin.cpp
    fpscounter.cpp
    keyeventfilter.cpp
)

add_library(liriwaylandserverplugin SHARED ${SOURCES})
target_link_libraries(liriwaylandserverplugin
                      Qt5::Qml
                      Qt5::Quick
                      Qt5::WaylandCompositor
                      Liri::WaylandServer
)

install(TARGETS liriwaylandserverplugin LIBRARY
        DESTINATION ${QML_INSTALL_DIR}/Liri/WaylandServer)

set(MODULE_FILES
    qmldir
    WindowChrome.qml
)
install(FILES ${MODULE_FILES}
        DESTINATION ${QML_INSTALL_DIR}/Liri/WaylandServer)
