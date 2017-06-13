import qbs 1.0

LiriDynamicLibrary {
    name: targetName
    targetName: "kms"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFS" }
    Depends { name: "LiriUDev" }
    Depends { name: "LiriLogind" }
    Depends { name: "gbm" }
    Depends { name: "libdrm" }

    condition: {
        if (!gbm.found) {
            console.error("gbm is required to build " + targetName);
            return false;
        }

        if (!libdrm.found) {
            console.error("libdrm is required to build " + targetName);
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
        qbs.installDir: lirideployment.pluginsDir + "/liri/egldeviceintegration"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
