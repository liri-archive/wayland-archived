import qbs 1.0

StaticLibrary {
    name: "materialdecoration"
    targetName: "materialdecoration"

    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

    cpp.defines: []
    files: ["*.cpp", "*.h", "*.qrc"]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

        cpp.includePaths: product.sourceDirectory
    }
}
