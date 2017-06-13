import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "fullscreen-shell"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }
    Depends { name: "WaylandScanner" }

    cpp.defines: []
    cpp.includePaths: base.concat([product.buildDirectory])

    files: ["*.cpp", "*.h"]

    Group {
        name: "Wayland Protocols"
        files: [
            "../../../data/protocols/wayland/fullscreen-shell-unstable-v1.xml"
        ]
        fileTags: ["wayland.client.protocol"]
    }

    FileTagger {
        patterns: "*.json"
        fileTags: ["qt_plugin_metadata"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/wayland-shell-integration"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
