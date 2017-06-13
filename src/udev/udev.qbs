import qbs 1.0
import "../../qbs/shared/imports/LiriUtils.js" as LiriUtils

LiriModuleProject {
    id: root

    name: "LiriUDev"
    moduleName: "LiriUDev"
    description: "Qt API for udev"
    pkgConfigDependencies: ["Qt5Core", "libudev"]
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("libudev") }],
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "udevdevice.h": ["UdevDevice"],
            "udevenumerate.h": ["UdevEnumerate"],
            "udev.h": ["Udev"],
            "udevmonitor.h": ["UdevMonitor"],
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
        Depends { name: "Qt"; submodules: ["core", "core-private"] }
        Depends { name: "libudev" }

        condition: {
            if (!libudev.found) {
                console.error("libudev is required to build " + targetName);
                return false;
            }

            return true;
        }

        cpp.defines: base.concat([
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_LIRIUDEV_LIB"
        ])

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends { name: "Qt"; submodules: ["core", "core-private"] }
            Depends { name: "libudev" }
        }
    }
}
