import qbs 1.0

LiriDynamicLibrary {
    name: "lirixwaylandplugin"
    targetName: "lirixwaylandplugin"

    Depends { name: "lirideployment" }
    Depends { name: "LiriWaylandServer" }
    Depends { name: "XCB"; submodules: ["xfixes", "cursor", "composite", "render", "shape"] }
    Depends { name: "X11.xcursor" }

    condition: {
        if (!project.withXWayland) {
            console.info("XWayland support disabled");
            return false;
        }

        if (!XCB.xcb.found) {
            console.error("xcb is required to build " + targetName);
            return false;
        }
        if (!XCB.xfixes.found) {
            console.error("xcb-xfixes is required to build " + targetName);
            return false;
        }
        if (!XCB.cursor.found) {
            console.error("xcb-cursor is required to build " + targetName);
            return false;
        }
        if (!XCB.composite.found) {
            console.error("xcb-composite is required to build " + targetName);
            return false;
        }
        if (!XCB.render.found) {
            console.error("xcb-render is required to build " + targetName);
            return false;
        }
        if (!XCB.shape.found) {
            console.error("xcb-shape is required to build " + targetName);
            return false;
        }

        if (!X11.x11.found || !X11.xcursor.found) {
            console.error("x11 is required to build " + targetName);
            return false;
        }
        if (!X11.xcursor.found) {
            console.error("xcursor is required to build " + targetName);
            return false;
        }

        return true;
    }

    cpp.defines: ["QT_WAYLAND_COMPOSITOR_QUICK"]

    files: ["*.cpp", "*.h"]

    Group {
        name: "QML Files"
        files: [
            "*.qml",
            "qmldir",
        ]
        fileTags: ["qml"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.qmlDir + "/Liri/XWayland"
        fileTagsFilter: ["dynamiclibrary", "qml"]
    }
}
