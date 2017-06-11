import qbs 1.0

LiriQmlPlugin {
    name: "liriwaylandserverplugin"
    pluginPath: "Liri/WaylandServer"

    Depends { name: "LiriWaylandServer" }

    condition: {
        if (!project.withLibraries) {
            console.info("Libraries disabled");
            return false;
        }

        return true;
    }

    files: ["*.cpp", "*.h", "qmldir", "*.qml"]
}
