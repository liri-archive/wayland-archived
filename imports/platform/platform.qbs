import qbs 1.0

LiriDynamicLibrary {
    name: "liriplatformplugin"
    targetName: "liriplatformplugin"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "qml", "quick", "waylandclient", "waylandclient-private"] }
    Depends { name: "materialdecoration" }

    cpp.defines: []

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
        qbs.installDir: lirideployment.qmlDir + "/Liri/Platform"
        fileTagsFilter: ["dynamiclibrary", "qml"]
    }
}
