import qbs 1.0

Project {
    name: "Plugins"

    references: [
        "shellintegrations/fullscreen-shell/fullscreen-shell.qbs",
        "platforms/eglfs/eglfs.qbs",
    ]
}
