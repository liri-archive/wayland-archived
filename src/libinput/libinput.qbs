import qbs 1.0

LiriModule {
    name: "LiriLibInput"
    targetName: "LiriLibInput"
    version: "0.0.0"
    createCMake: false

    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
    Depends { name: "libinput" }
    Depends { name: "LiriUDev" }
    Depends { name: "LiriLogind" }
    Depends { name: "eglfsxkb" }

    condition: {
        if (!libinput.found)
            throw "libinput is required to build " + targetName;

        return true;
    }

    cpp.defines: base.concat([
        'LIRIWAYLAND_VERSION="' + project.version + '"',
        "QT_BUILD_LIRILIBINPUT_LIB"
    ])

    create_headers.headersMap: ({
        "libinputgesture.h": "LibInputGestore",
        "libinputhandler.h": "LibInputHandler",
        "libinputkeyboard.h": "LibInputKeyboard",
        "libinputpointer.h": "LibInputPointer",
        "libinputtouch.h": "LibInputTouch",
    })

    create_pkgconfig.name: "Liri LibInput"
    create_pkgconfig.description: "Qt API for libinput"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "libinput", "LiriUDev"]

    files: ["*.cpp"]

    Group {
        name: "Headers"
        files: ["*.h"]
        excludeFiles: ["*_p.h"]
        fileTags: ["public_headers"]
    }

    Group {
        name: "Private Headers"
        files: ["*_p.h"]
        excludeFiles: ["logging_p.h"]
        fileTags: ["private_headers"]
    }

    Export {
        property bool found: true

        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
        Depends { name: "libinput" }
        Depends { name: "LiriUDev" }
        Depends { name: "LiriLogind" }

        cpp.includePaths: product.generatedHeadersDir
    }
}
