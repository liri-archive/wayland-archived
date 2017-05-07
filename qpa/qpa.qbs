import qbs 1.0

LiriDynamicLibrary {
    name: "QPA"
    targetName: "lirieglfs"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "LiriEglFS" }

    files: ["*.cpp", "*.h"]

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/platforms"
        fileTagsFilter: product.type
    }
}
