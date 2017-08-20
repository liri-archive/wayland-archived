import qbs 1.0

Project {
    name: "Plugins"

    references: [
        "decorations/material/material.qbs",
        "shellintegrations/fullscreen-shell/fullscreen-shell.qbs",
        "platforms/eglfs/eglfs.qbs",
    ]
}
