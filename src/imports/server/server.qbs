import qbs 1.0

LiriQmlPlugin {
    name: "liriwaylandserverplugin"
    pluginPath: "Liri/WaylandServer"

    Depends {
        name: "Qt.gui-private"
        versionAtLeast: project.minimumQtVersion
    }
    Depends { name: "LiriWaylandServer" }

    files: ["*.cpp", "*.h", "qmldir"]
}
