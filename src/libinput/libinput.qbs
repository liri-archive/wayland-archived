import qbs 1.0
import "../../qbs/shared/imports/LiriUtils.js" as LiriUtils

LiriModuleProject {
    id: root

    name: "LiriLibInput"
    moduleName: "LiriLibInput"
    description: "Qt API for libinput"
    pkgConfigDependencies: ["Qt5Core", "libinput", "LiriUDev"]
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("libinput") },
                  { name: LiriUtils.quote("LiriUDev") },
                  { name: LiriUtils.quote("LiriLogind") },
                  { name: LiriUtils.quote("Qt.service_support-private") },
                  { name: LiriUtils.quote("Qt.theme_support-private") },
                  { name: LiriUtils.quote("Qt.eventdispatcher_support-private") }],
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "libinputgesture.h": ["LibInputGestore"],
            "libinputhandler.h": ["LibInputHandler"],
            "libinputkeyboard.h": ["LibInputKeyboard"],
            "libinputpointer.h": ["LibInputPointer"],
            "libinputtouch.h": ["LibInputTouch"],
        })

        Group {
            name: "Headers"
            files: "**/*.h"
            fileTags: ["hpp_syncable"]
        }
    }

    LiriModule {
        name: "LiriLibInput"
        targetName: "LiriLibInput"
        version: "0.0.0"

        Depends { name: root.headersName }
        Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
        Depends { name: "libinput" }
        Depends { name: "LiriUDev" }
        Depends { name: "LiriLogind" }
        Depends { name: "eglfsxkb" }

        condition: {
            if (!libinput.found) {
                console.error("libinput is required to build " + targetName);
                return false;
            }

            return true;
        }

        cpp.defines: base.concat([
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_LIRILIBINPUT_LIB"
        ])

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
            Depends { name: "libinput" }
            Depends { name: "LiriUDev" }
            Depends { name: "LiriLogind" }
        }
    }
}
