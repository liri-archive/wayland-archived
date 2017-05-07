import qbs 1.0

LiriModule {
    name: "LiriEglFS"
    targetName: "LiriEglFS"
    version: "0.0.0"
    createCMake: false

    Depends {
        name: "Qt"
        submodules: [
            "core", "core-private", "gui", "gui-private",
            "egl_support-private", "platformcompositor_support-private",
            "fontdatabase_support-private", "service_support-private",
            "theme_support-private", "eventdispatcher_support-private"
        ]
    }
    Depends { name: "LiriUDev" }
    Depends { name: "LiriLibInput" }
    Depends { name: "LiriLogind" }
    Depends { name: "eglfsxkb" }

    cpp.defines: [
        'LIRIWAYLAND_VERSION="' + project.version + '"',
        "QT_BUILD_LIRIEGLFS_LIB"
    ]

    create_headers.headersMap: ({
        "egldeviceintegration.h": "EglDeviceIntegration",
        "eglfscontext.h": "EglFSContext",
        "eglfscursor.h": "EglFSCursor",
        "eglfsglobal.h": "EglFSGlobal",
        "eglfsintegration.h": "EglFSIntegration",
        "eglfsnativeinterface.h": "EglFSNativeInterface",
        "eglfsoffscreenwindow.h": "EglFSOffscreenWindow",
        "eglfsscreen.h": "EglFSScreen",
        "eglfswindow.h": "EglFSWindow",
    })

    create_pkgconfig.name: "Liri EglFS"
    create_pkgconfig.description: "EGL device integration"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "Qt5Gui", "LiriUDev", "LiriLibInput"]

    files: ["*.cpp", "*.qrc"]

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
        Depends {
            name: "Qt"
            submodules: [
                "core", "core-private", "gui", "gui-private",
                "egl_support-private", "platformcompositor_support-private",
                "fontdatabase_support-private", "service_support-private",
                "theme_support-private", "eventdispatcher_support-private"
            ]
        }
        Depends { name: "LiriUDev" }
        Depends { name: "LiriLibInput" }
        Depends { name: "LiriLogind" }

        cpp.includePaths: product.generatedHeadersDir
    }
}
