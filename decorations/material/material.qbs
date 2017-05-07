import qbs 1.0

LiriDynamicLibrary {
    name: "Material Decoration"
    targetName: "material"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

    cpp.defines: []
    files: ["*.cpp", "*.h", "*.qrc"]

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.pluginsDir + "/wayland-decoration-client"
        fileTagsFilter: ["dynamiclibrary"]
    }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

        cpp.includePaths: product.sourceDirectory
    }
}
