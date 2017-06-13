import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "fullscreen-shell"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }
    Depends { name: "WaylandScanner" }

    condition: {
        if (!project.withFullscreenShellIntegration) {
            console.info("fullscreen-shell integration disabled");
            return false;
        }

        return true;
    }

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

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/wayland-shell-integration"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
