import qbs 1.0

Project {
    name: "Wayland"

    readonly property string version: "0.9.0.1"
    readonly property var versionParts: version.split('.').map(function(part) { return parseInt(part); })

    property bool autotestEnabled: false
    property stringList autotestArguments: []
    property stringList autotestWrapper: []

    condition: qbs.targetOS.contains("linux")

    minimumQbsVersion: "1.8.0"

    references: [
        "imports/imports.qbs",
        "plugins/plugins.qbs",
        "src/deployment.qbs",
        "src/libs.qbs",
        "src/client/client.qbs",
        "src/eglfs/eglfs.qbs",
        "src/libinput/libinput.qbs",
        "src/logind/logind.qbs",
        "src/server/server.qbs",
        "src/udev/udev.qbs",
        //"tests/auto/auto.qbs",
        "tests/manual/manual.qbs",
    ]
}
