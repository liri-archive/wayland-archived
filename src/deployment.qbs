import qbs 1.0

Project {
    name: "Deployment"

    InstallPackage {
        name: "liri-wayland-artifacts"
        targetName: name
        builtByDefault: false

        archiver.type: "tar"
        archiver.outputDirectory: project.buildDirectory

        Depends { name: "fullscreen-shell"; required: false }
        Depends { name: "kms"; required: false }
        Depends { name: "liriplatformplugin"; required: false }
        Depends { name: "lirixwaylandplugin"; required: false }
        Depends { name: "LiriEglFS"; required: false }
        Depends { name: "LiriEglFS-pkgconfig"; required: false }
        Depends { name: "LiriEglFS-qbs"; required: false }
        Depends { name: "LiriLibInput"; required: false }
        Depends { name: "LiriLibInput-pkgconfig"; required: false }
        Depends { name: "LiriLibInput-qbs"; required: false }
        Depends { name: "LiriLogind"; required: false }
        Depends { name: "LiriLogind-pkgconfig"; required: false }
        Depends { name: "LiriLogind-qbs"; required: false }
        Depends { name: "LiriWaylandServer"; required: false }
        Depends { name: "LiriWaylandServer-pkgconfig"; required: false }
        Depends { name: "LiriWaylandServer-qbs"; required: false }
        Depends { name: "materialdecorationplugin"; required: false }
        Depends { name: "lirieglfs"; required: false }
    }
}
