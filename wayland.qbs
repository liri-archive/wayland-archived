import qbs 1.0

Project {
    name: "Liri Wayland"

    readonly property string version: "0.9.0.1"

    property bool autotestEnabled: false
    property stringList autotestArguments: []
    property stringList autotestWrapper: []

    condition: qbs.targetOS.contains("linux")

    minimumQbsVersion: "1.6"

    qbsSearchPaths: ["qbs/shared", "qbs/local"]

    references: [
        "decorations/material/material.qbs",
        "imports/platform/platform.qbs",
        "imports/server/server.qbs",
        "imports/server-private/server-private.qbs",
        "imports/xwayland/xwayland.qbs",
        "plugins/egldeviceintegration/kms/kms.qbs",
        "plugins/shellintegrations/fullscreen-shell/fullscreen-shell.qbs",
        "qpa/qpa.qbs",
        "src/client/client.qbs",
        "src/eglfs/eglfs.qbs",
        "src/eglfsxkb/eglfsxkb.qbs",
        "src/libinput/libinput.qbs",
        "src/logind/logind.qbs",
        "src/server/server.qbs",
        "src/udev/udev.qbs",
        "tests/auto/client/display/display.qbs",
        //"tests/auto/client/output/output.qbs",
        //"tests/auto/client/registry/registry.qbs",
        "tests/auto/client/shmpool/shmpool.qbs",
        //"tests/auto/logind/logind.qbs",
        "tests/auto/udev/udev.qbs",
        "tests/manual/libinput/libinput.qbs",
    ]

    AutotestRunner {
        builtByDefault: autotestEnabled
        name: "liri-wayland-autotest"
        arguments: project.autotestArguments
        wrapper: project.autotestWrapper
    }
}
