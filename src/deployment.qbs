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
        Depends { name: "lirixwaylandplugin"; required: false }
        Depends { name: "LiriEglFSDeviceIntegration"; required: false }
        Depends { name: "LiriEglFSDeviceIntegration-qbs"; required: false }
        Depends { name: "LiriLibInput"; required: false }
        Depends { name: "LiriLibInput-pkgconfig"; required: false }
        Depends { name: "LiriLibInput-qbs"; required: false }
        Depends { name: "LiriPlatformHeaders"; required: false }
        Depends { name: "LiriPlatformHeaders-qbs"; required: false }
        Depends { name: "lirieglfs"; required: false }
        Depends { name: "eglfs-kms-egldevice-integration"; required: false }
        Depends { name: "eglfs-kms-integration"; required: false }
    }
}
