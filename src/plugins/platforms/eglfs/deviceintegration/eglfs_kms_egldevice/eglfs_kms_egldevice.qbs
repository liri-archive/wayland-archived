import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "eglfs-kms-egldevice-integration"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFSDeviceIntegration" }
    Depends { name: "LiriEglFSKmsSupport" }

    files: ["*.cpp", "*.h", "*.json"]

    FileTagger {
        patterns: "*.json"
        fileTags: ["qt_plugin_metadata"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/liri/egldeviceintegrations"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
