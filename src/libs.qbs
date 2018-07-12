import qbs 1.0

Project {
    name: "Libraries"

    references: [
        "eglfsxkb/eglfsxkb.qbs",
        "libinput/libinput.qbs",
        "platformheaders/platformheaders.qbs",
        "platformsupport/edid/edid.qbs",
        "platformsupport/kmsconvenience/kmsconvenience.qbs",
    ]
}
