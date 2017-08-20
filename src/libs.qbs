import qbs 1.0

Project {
    name: "Libraries"

    references: [
        "eglfsxkb/eglfsxkb.qbs",
        "platformsupport/edid/edid.qbs",
        "platformsupport/kmsconvenience/kmsconvenience.qbs",
    ]
}
