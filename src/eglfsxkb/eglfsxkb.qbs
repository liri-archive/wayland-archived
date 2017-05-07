import qbs 1.0

StaticLibrary {
    name: "eglfsxkb"
    targetName: "eglfsxkb"

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "xkbcommon" }

    condition: {
        if (!xkbcommon.found)
            throw "xkbcommon is required to build " + targetName;

        return true;
    }

    files: ["*.cpp", "*.h"]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt.core" }
        Depends { name: "xkbcommon" }

        cpp.includePaths: base.concat([product.sourceDirectory])
    }
}
