import qbs 1.0

LiriDynamicLibrary {
    name: "Liri.XWayland"
    targetName: "lirixwaylandplugin"

    Depends { name: "qbsbuildconfig" }
    Depends { name: "lirideployment" }
    Depends { name: "LiriWaylandServer" }
    Depends { name: "XCB"; submodules: ["xfixes", "cursor", "composite", "render", "shape"] }
    Depends { name: "X11.xcursor" }

    condition: {
        if (!qbsbuildconfig.withXWayland) {
            console.info("XWayland support disabled");
            return false;
        }

        if (!XCB.xcb.found || !XCB.xfixes.found || !XCB.cursor.found || !XCB.composite.found || !XCB.render.found || !XCB.shape.found) {
            throw "Unable to find at least one of the following dependencies: xcb, " +
                  "xcb-xfixes, xcb-cursor, xcb-composite, xcb-render, xcb-shape.";
        }
        if (!X11.x11.found || !X11.xcursor.found) {
            throw "Unable to find at least one of the following dependencies: x11, xcursor.";
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
