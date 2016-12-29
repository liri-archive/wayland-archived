# All the required XCB components
#find_package(XCB
#    REQUIRED COMPONENTS
#        DAMAGE
#        SYNC
#        RANDR
#        KEYSYMS
#        IMAGE
#        SHM
#        GLX
#    OPTIONAL_COMPONENTS
#        ICCCM
#        CURSOR
#)
find_package(XCB
    REQUIRED COMPONENTS
        XCB
        XFIXES
        CURSOR
        COMPOSITE
        RENDER
        SHAPE
)
set_package_properties(XCB PROPERTIES TYPE REQUIRED)

if(NOT X11_FOUND)
    message(FATAL_ERROR "X11 is required to build XWayland support")
endif()
if(NOT X11_Xcursor_FOUND)
    message(FATAL_ERROR "Xcursor is required to build XWayland support")
endif()


include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../../headers"
    ${Qt5WaylandCompositor_PRIVATE_INCLUDE_DIRS}
)

set(SOURCES
    plugin.cpp
    xcbatom.cpp
    xcbatoms.cpp
    xcbcursors.cpp
    xcbproperties.cpp
    xcbresources.cpp
    xcbwindow.cpp
    xcbwrapper.cpp
    xwayland.cpp
    xwaylandmanager.cpp
    xwaylandshellsurface.cpp
    xwaylandquickshellintegration.cpp
    xwaylandquickshellsurfaceitem.cpp
    xwaylandserver.cpp
    sigwatch.cpp
)

add_library(lirixwaylandplugin SHARED ${SOURCES})
target_link_libraries(lirixwaylandplugin
    Qt5::Qml
    Qt5::Quick
    Qt5::WaylandCompositor
    Liri::WaylandServer
    XCB::XCB
    XCB::XFIXES
    XCB::CURSOR
    XCB::COMPOSITE
    XCB::RENDER
    XCB::SHAPE
    ${X11_Xcursor_LIB}
)

install(TARGETS lirixwaylandplugin LIBRARY
        DESTINATION ${QML_INSTALL_DIR}/Liri/XWayland)

set(MODULE_FILES
    qmldir
)
install(FILES ${MODULE_FILES}
        DESTINATION ${QML_INSTALL_DIR}/Liri/XWayland)
