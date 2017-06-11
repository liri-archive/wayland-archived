import qbs 1.0

Project {
    name: "QML Plugins"

    references: [
        "platform/platform.qbs",
        "server/server.qbs",
        "server-private/server-private.qbs",
        "xwayland/xwayland.qbs",
    ]
}
