import qbs 1.0

LiriQmlPlugin {
    name: "liriplatformplugin"
    pluginPath: "Liri/Platform"

    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }
    Depends { name: "materialdecoration" }

    cpp.defines: []

    files: ["*.cpp", "*.h", "qmldir", "*.qml"]
}
