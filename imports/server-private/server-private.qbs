import qbs 1.0

LiriDynamicLibrary {
    name: "liriwaylandserverprivateplugin"
    targetName: "liriwaylandserverprivateplugin"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "qml", "quick", "waylandcompositor"] }

    condition: {
        if (!project.withLibraries) {
            console.info("Libraries disabled");
            return false;
        }

        return true;
    }

    cpp.defines: base.concat(["QT_WAYLAND_COMPOSITOR_QUICK"])

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
        qbs.installDir: lirideployment.qmlDir + "/Liri/WaylandServer/Private"
        fileTagsFilter: ["dynamiclibrary", "qml"]
    }
}
