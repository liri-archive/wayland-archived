import qbs 1.0

StaticLibrary {
    name: "materialdecoration"
    targetName: "materialdecoration"

    Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

    condition: {
        if (!project.withMaterialDecoration) {
            console.info("Material decoration disabled");
            return false;
        }

        return true;
    }

    cpp.defines: []
    files: ["*.cpp", "*.h", "*.qrc"]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["gui", "gui-private", "waylandclient", "waylandclient-private"] }

        cpp.includePaths: product.sourceDirectory
    }
}
