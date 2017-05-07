import qbs 1.0

LiriDynamicLibrary {
    name: "kms"
    targetName: "kms"

    Depends { name: "qbsbuildconfig" }
    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFS" }
    Depends { name: "LiriUDev" }
    Depends { name: "LiriLogind" }
    Depends { name: "gbm" }
    Depends { name: "libdrm" }

    condition: {
        if (!qbsbuildconfig.withKmsPlugin) {
            console.info("KMS EGL device integration disabled");
            return false;
        }

        if (!gbm.found)
            throw "gbm is required to build " + targetName;

        if (!libdrm.found)
            throw "libdrm is required to build " + targetName;

        return true;
    }

    files: ["*.cpp", "*.h"]

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/liri/egldeviceintegration"
        fileTagsFilter: ["dynamiclibrary"]
    }
}
