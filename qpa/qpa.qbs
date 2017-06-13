import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "lirieglfs"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFS" }

    condition: {
        if (!project.withQPA) {
            console.info("QPA plugin disabled");
            return false;
        }

        return true;
    }

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
