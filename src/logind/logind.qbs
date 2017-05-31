import qbs 1.0

LiriModule {
    name: "LiriLogind"
    targetName: "LiriLogind"
    version: "0.0.0"
    createCMake: false

    Depends { name: "Qt"; submodules: ["core", "core-private", "dbus"] }

    condition: {
        if (!project.withLibraries) {
            console.info("Libraries disabled");
            return false;
        }

        return true;
    }

    cpp.defines: [
        'LIRIWAYLAND_VERSION="' + project.version + '"',
        "QT_BUILD_LIRILOGIND_LIB"
    ]

    create_headers.headersMap: ({
        "logind.h": "Logind",
        "vthandler.h": "VtHandler",
    })

    create_pkgconfig.name: "Liri Logind"
    create_pkgconfig.description: "Qt API for logind"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "Qt5DBus"]

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
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "core-private", "dbus"] }

        cpp.includePaths: product.generatedHeadersDir
    }
}
