import qbs 1.0

LiriDynamicLibrary {
    name: "Fullscreen Shell Integration"
    targetName: "fullscreen-shell"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }
    Depends { name: "WaylandScanner" }

    cpp.defines: []

    files: ["*.cpp", "*.h"]

    Group {
        name: "Wayland Protocols"
        files: [
            "../../../data/protocols/wayland/fullscreen-shell-unstable-v1.xml"
        ]
        fileTags: ["wayland.client.protocol"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/wayland-shell-integration"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
