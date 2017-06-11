import qbs 1.0

Project {
    name: "Plugins"

    references: [
        "decorations/material/material.qbs",
        "egldeviceintegration/kms/kms.qbs",
        "shellintegrations/fullscreen-shell/fullscreen-shell.qbs",
        "../qpa/qpa.qbs",
    ]
}
