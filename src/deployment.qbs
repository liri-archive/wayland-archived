import qbs 1.0

Project {
    name: "Deployment"

    InstallPackage {
        name: "liri-wayland-artifacts"
        targetName: name
        builtByDefault: false

        archiver.type: "tar"
        archiver.outputDirectory: project.buildDirectory

        Depends { name: "fullscreen-shell" }
        Depends { name: "kms" }
        Depends { name: "liriplatformplugin" }
        Depends { name: "liriwaylandserverplugin" }
        Depends { name: "liriwaylandserverprivateplugin" }
        Depends { name: "lirixwaylandplugin" }
        Depends { name: "LiriEglFS" }
        Depends { name: "LiriEglFS-pkgconfig" }
        Depends { name: "LiriEglFS-qbs" }
        Depends { name: "LiriLibInput" }
        Depends { name: "LiriLibInput-pkgconfig" }
        Depends { name: "LiriLibInput-qbs" }
        Depends { name: "LiriLogind" }
        Depends { name: "LiriLogind-pkgconfig" }
        Depends { name: "LiriLogind-qbs" }
        Depends { name: "LiriUDev" }
        Depends { name: "LiriUDev-pkgconfig" }
        Depends { name: "LiriUDev-qbs" }
        Depends { name: "LiriWaylandClient" }
        Depends { name: "LiriWaylandClient-cmake" }
        Depends { name: "LiriWaylandClient-pkgconfig" }
        Depends { name: "LiriWaylandClient-qbs" }
        Depends { name: "LiriWaylandServer" }
        Depends { name: "LiriWaylandServer-pkgconfig" }
        Depends { name: "LiriWaylandServer-qbs" }
        Depends { name: "materialdecorationplugin" }
        Depends { name: "QPA" }
    }
}
