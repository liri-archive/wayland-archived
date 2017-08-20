import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "lirieglfs"

    Depends { name: "lirideployment" }
    Depends {
        name: "Qt"
        submodules: ["core", "core-private", "gui", "gui-private"]
        versionAtLeast: project.minimumQtVersion
    }
    Depends { name: "LiriEglFSDeviceIntegration" }

    files: ["*.cpp", "*.h", "*.json"]

    FileTagger {
        patterns: "liri.json"
        fileTags: ["qt_plugin_metadata"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/platforms"
        fileTagsFilter: product.type
    }
}
