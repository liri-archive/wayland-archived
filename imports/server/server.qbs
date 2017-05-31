import qbs 1.0

LiriDynamicLibrary {
    name: "liriwaylandserverplugin"
    targetName: "liriwaylandserverplugin"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick"] }
    Depends { name: "LiriWaylandServer" }

    condition: {
        if (!project.withLibraries) {
            console.info("Libraries disabled");
            return false;
        }

        return true;
    }

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
        qbs.installDir: lirideployment.qmlDir + "/Liri/WaylandServer"
        fileTagsFilter: ["dynamiclibrary", "qml"]
    }
}
