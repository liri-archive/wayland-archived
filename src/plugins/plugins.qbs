import qbs 1.0

Project {
    name: "Plugins"

    references: [
        "decorations/material/material.qbs",
        "egldeviceintegration/kms/kms.qbs",
        "qpa/qpa.qbs",
        "shellintegrations/fullscreen-shell/fullscreen-shell.qbs",
    ]
}
