import qbs 1.0
import "../../qbs/shared/imports/LiriUtils.js" as LiriUtils

LiriModuleProject {
    id: root

    name: "LiriLogind"
    moduleName: "LiriLogind"
    description: "Qt API for logind"
    pkgConfigDependencies: ["Qt5Core", "Qt5DBus"]
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.dbus") }],
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "logind.h": ["Logind"],
            "vthandler.h": ["VtHandler"],
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
        Depends { name: "Qt"; submodules: ["core", "core-private", "dbus"] }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_LIRILOGIND_LIB"
        ]

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends { name: "Qt"; submodules: ["core", "core-private", "dbus"] }
        }
    }
}
