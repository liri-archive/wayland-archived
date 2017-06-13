import qbs 1.0

LiriQmlPlugin {
    name: "liriwaylandserverprivateplugin"
    pluginPath: "Liri/WaylandServer/Private"

    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandcompositor"] }

    cpp.defines: base.concat(["QT_WAYLAND_COMPOSITOR_QUICK"])

    files: ["*.cpp", "*.h", "qmldir", "*.qml"]
}
