import qbs 1.0

LiriDynamicLibrary {
    name: "liriwaylandserverplugin"
    targetName: "liriwaylandserverplugin"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick"] }
    Depends { name: "LiriWaylandServer" }

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
