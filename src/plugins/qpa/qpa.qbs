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
    Depends { name: "LiriEglFS" }

    files: ["*.cpp", "*.h"]

    FileTagger {
        patterns: "*.json"
        fileTags: ["qt_plugin_metadata"]
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/platforms"
        fileTagsFilter: product.type
    }
}
