include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers/GreenIsland"
)

set(SOURCES
    plugin.cpp
    fpscounter.cpp
    keyeventfilter.cpp
)

add_library(greenislandplugin SHARED ${SOURCES})
target_link_libraries(greenislandplugin
    Qt5::Qml
    Qt5::Quick
    Qt5::WaylandCompositor
    GreenIsland::Server
)

install(TARGETS greenislandplugin LIBRARY
    DESTINATION ${QML_INSTALL_DIR}/GreenIsland)

set(MODULE_FILES
    qmldir
    WaylandOutputWindow.qml
    WindowChrome.qml
)
install(FILES ${MODULE_FILES}
    DESTINATION ${QML_INSTALL_DIR}/GreenIsland)
