import qbs 1.0
import "../../qbs/shared/imports/LiriUtils.js" as LiriUtils

LiriModuleProject {
    id: root

    name: "LiriEglFS"
    moduleName: "LiriEglFS"
    description: "EGL device integration"
    pkgConfigDependencies: ["Qt5Core", "Qt5Gui", "LiriUDev", "LiriLibInput"]
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("Qt.egl_support-private") },
                  { name: LiriUtils.quote("Qt.platformcompositor_support-private") },
                  { name: LiriUtils.quote("Qt.fontdatabase_support-private") },
                  { name: LiriUtils.quote("Qt.service_support-private") },
                  { name: LiriUtils.quote("Qt.theme_support-private") },
                  { name: LiriUtils.quote("Qt.eventdispatcher_support-private") }],
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "egldeviceintegration.h": ["EglDeviceIntegration"],
            "eglfscontext.h": ["EglFSContext"],
            "eglfscursor.h": ["EglFSCursor"],
            "eglfsglobal.h": ["EglFSGlobal"],
            "eglfsintegration.h": ["EglFSIntegration"],
            "eglfsnativeinterface.h": ["EglFSNativeInterface"],
            "eglfsoffscreenwindow.h": ["EglFSOffscreenWindow"],
            "eglfsscreen.h": ["EglFSScreen"],
            "eglfswindow.h": ["EglFSWindow"],
        })

        Group {
            name: "Headers"
            files: "**/*.h"
            fileTags: ["hpp_syncable"]
        }
    }

    LiriModule {
        name: root.moduleName
        targetName: root.targetName
        version: "0.0.0"

        Depends { name: root.headersName }
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

        condition: {
            if (!project.withQPA) {
                console.info("QPA plugin disabled");
                return false;
            }

            return true;
        }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_LIRIEGLFS_LIB"
        ]

        files: ["*.cpp", "*.h", "*.qrc"]

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
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
        }
    }
}
