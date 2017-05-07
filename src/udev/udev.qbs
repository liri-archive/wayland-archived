import qbs 1.0

LiriModule {
    name: "LiriUDev"
    targetName: "LiriUDev"
    version: "0.0.0"
    createCMake: false

    Depends { name: "Qt"; submodules: ["core", "core-private"] }
    Depends { name: "libudev" }

    condition: {
        if (!libudev.found)
            throw "libudev is required to build " + targetName;

        return true;
    }

    cpp.defines: base.concat([
        'LIRIWAYLAND_VERSION="' + project.version + '"',
        "QT_BUILD_LIRIUDEV_LIB"
    ])

    create_headers.headersMap: ({
        "udevdevice.h": "UdevDevice",
        "udevenumerate.h": "UdevEnumerate",
        "udev.h": "Udev",
        "udevmonitor.h": "UdevMonitor",
    })

    create_pkgconfig.name: "Liri UDev"
    create_pkgconfig.description: "Qt API for udev"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "libudev"]

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
        Depends { name: "Qt"; submodules: ["core", "core-private"] }
        Depends { name: "libudev" }

        cpp.includePaths: product.generatedHeadersDir
    }
}
