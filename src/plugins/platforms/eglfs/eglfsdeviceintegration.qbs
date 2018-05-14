import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: "LiriEglFSDeviceIntegration"
    moduleName: "LiriEglFSDeviceIntegration"
    description: "EGL device integration"
    createPkgConfig: false
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("Qt.eventdispatcher_support-private") },
                  { name: LiriUtils.quote("Qt.service_support-private") },
                  { name: LiriUtils.quote("Qt.theme_support-private") },
                  { name: LiriUtils.quote("Qt.fontdatabase_support-private") },
                  { name: LiriUtils.quote("Qt.fb_support-private") },
                  { name: LiriUtils.quote("Qt.egl_support-private") },
                  { name: LiriUtils.quote("Qt.platformcompositor_support-private") }],
        "cpp.defines": [LiriUtils.quote("QT_EGL_NO_X11")],
    })

    LiriHeaders {
        Group {
            name: "Headers"
            files: "api/*.h"
            fileTags: ["hpp_syncable"]
        }
    }

    LiriModule {
        name: project.moduleName
        targetName: project.targetName
        version: "0.0.0"

        Depends { name: project.headersName }
        Depends {
            name: "Qt"
            submodules: [
                "core-private", "gui-private", "eventdispatcher_support-private",
                "service_support-private", "theme_support-private",
                "fontdatabase_support-private", "fb_support-private",
                "egl_support-private", "platformcompositor_support-private"
            ]
        }
        Depends { name: "Qt5Udev" }
        Depends { name: "LiriLibInput-private" }
        Depends { name: "LiriLogind" }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_EGL_DEVICE_LIB",
            "QT_EGL_NO_X11",
        ]

        files: ["cursor.qrc"]

        Group {
            name: "API"
            prefix: "api/"
            files: [
                "libinputmanager.cpp",
                "libinputmanager_p.h",
                "qeglfslogindhandler.cpp",
                "qeglfswindow.cpp",
                "qeglfsscreen.cpp",
                "qeglfshooks.cpp",
                "qeglfsdeviceintegration.cpp",
                "qeglfsintegration.cpp",
                "qeglfsoffscreenwindow.cpp",
                "qeglfslogindhandler_p.h",
                "qeglfswindow_p.h",
                "qeglfsscreen_p.h",
                "qeglfshooks_p.h",
                "qeglfsdeviceintegration_p.h",
                "qeglfsintegration_p.h",
                "qeglfsoffscreenwindow_p.h",
                "qeglfsglobal_p.h",
                "vthandler.cpp",
                "vthandler.h",
                "vthandler_p.h",
            ]

            Group {
                name: "API (OpenGL)"
                prefix: "api/"
                files: [
                    "qeglfscursor.cpp",
                    "qeglfscontext.cpp",
                    "qeglfscursor_p.h",
                    "qeglfscontext_p.h",
                ]
            }
        }

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends {
                name: "Qt"
                submodules: [
                    "core-private", "gui-private", "eventdispatcher_support-private",
                    "service_support-private", "theme_support-private",
                    "fontdatabase_support-private", "fb_support-private",
                    "egl_support-private", "platformcompositor_support-private"
                ]
            }
            Depends { name: "Qt5Udev" }
            Depends { name: "LiriLibInput-private" }
            Depends { name: "LiriLogind" }

            cpp.defines: ["QT_EGL_NO_X11"]
            cpp.includePaths: product.sourceDirectory + "/api"
        }
    }
}
