import qbs 1.0

Project {
    name: "Liri Wayland"

    readonly property string version: "0.9.0.1"
    readonly property var versionParts: version.split('.').map(function(part) { return parseInt(part); })

    property bool autotestEnabled: false
    property stringList autotestArguments: []
    property stringList autotestWrapper: []

    property bool withKmsPlugin: true
    property bool withXWayland: true
    property bool withFullscreenShellIntegration: true
    property bool withMaterialDecoration: true
    property bool withLibraries: true
    property bool withQPA: true

    condition: qbs.targetOS.contains("linux")

    minimumQbsVersion: "1.6"

    qbsSearchPaths: ["qbs/shared"]

    references: [
        "imports/imports.qbs",
        "plugins/plugins.qbs",
        "src/deployment.qbs",
        "src/client/client.qbs",
        "src/eglfs/eglfs.qbs",
        "src/eglfsxkb/eglfsxkb.qbs",
        "src/libinput/libinput.qbs",
        "src/logind/logind.qbs",
        "src/materialdecoration/materialdecoration.qbs",
        "src/server/server.qbs",
        "src/udev/udev.qbs",
        //"tests/auto/auto.qbs",
        "tests/manual/libinput/libinput.qbs",
    ]
}
