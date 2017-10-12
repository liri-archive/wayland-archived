import qbs 1.0

LiriDynamicLibrary {
    name: "materialdecorationplugin"
    targetName: "material"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

    Qt.core.resourcePrefix: "/"
    Qt.core.resourceSourceBase: sourceDirectory

    cpp.defines: []
    files: ["*.cpp", "*.h"]

    FileTagger {
        patterns: "*.json"
        fileTags: ["qt_plugin_metadata"]
    }

    Group {
        name: "Resource Data"
        prefix: "icons/"
        files: [
            "window-close.svg",
            "window-maximize.svg",
            "window-minimize.svg",
            "window-restore.svg",
        ]
        fileTags: ["qt.core.resource_data"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/wayland-decoration-client"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
