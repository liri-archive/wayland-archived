import qbs 1.0

Project {
    name: "Liri Wayland"

    readonly property string version: "0.9.0.1"

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
        "imports/platform/platform.qbs",
        "imports/server/server.qbs",
        "imports/server-private/server-private.qbs",
        "imports/xwayland/xwayland.qbs",
        "plugins/decorations/material/material.qbs",
        "plugins/egldeviceintegration/kms/kms.qbs",
        "plugins/shellintegrations/fullscreen-shell/fullscreen-shell.qbs",
        "qpa/qpa.qbs",
        "src/client/client.qbs",
        "src/eglfs/eglfs.qbs",
        "src/eglfsxkb/eglfsxkb.qbs",
        "src/libinput/libinput.qbs",
        "src/logind/logind.qbs",
        "src/materialdecoration/materialdecoration.qbs",
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
        Depends { name: "LiriWaylandClient" }
        Depends { name: "LiriWaylandServer" }

        builtByDefault: autotestEnabled
        name: "liri-wayland-autotest"
        arguments: project.autotestArguments
        wrapper: project.autotestWrapper
    }

    InstallPackage {
        name: "liri-wayland-artifacts"
        targetName: name
        builtByDefault: false

        archiver.type: "tar"
        archiver.outputDirectory: project.buildDirectory

        Depends { name: "Fullscreen Shell Integration" }
        Depends { name: "kms" }
        Depends { name: "liriplatformplugin" }
        Depends { name: "liriwaylandserverplugin" }
        Depends { name: "liriwaylandserverprivateplugin" }
        Depends { name: "lirixwaylandplugin" }
        Depends { name: "LiriEglFS" }
        Depends { name: "LiriLibInput" }
        Depends { name: "LiriLogind" }
        Depends { name: "LiriUDev" }
        Depends { name: "LiriWaylandClient" }
        Depends { name: "LiriWaylandServer" }
        Depends { name: "materialdecorationplugin" }
        Depends { name: "QPA" }
    }
}
