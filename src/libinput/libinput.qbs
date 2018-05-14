import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: moduleName
    moduleName: "LiriLibInput"
    description: "Qt API for libinput"
    createPkgConfig: false
    createCMake: false
    conditionFunction: (function() {
        if (!libinput.found) {
            console.error("libinput is required to build " + targetName);
            return false;
        }

        return true;
    })

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("libinput") },
                  { name: LiriUtils.quote("Qt5Udev") },
                  { name: LiriUtils.quote("LiriLogind") },
                  { name: LiriUtils.quote("Qt.service_support-private") },
                  { name: LiriUtils.quote("Qt.theme_support-private") },
                  { name: LiriUtils.quote("Qt.eventdispatcher_support-private") }],
    })

    LiriHeaders {
        name: project.headersName
        sync.module: project.moduleName
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

    LiriPrivateModule {
        targetName: project.targetName

        Depends { name: project.headersName }
        Depends {
            name: "Qt"
            submodules: ["core", "core-private", "gui", "gui-private"]
            versionAtLeast: project.minimumQtVersion
        }
        Depends { name: "libinput" }
        Depends { name: "Qt5Udev" }
        Depends { name: "LiriLogind" }
        Depends { name: "eglfsxkb" }

        cpp.defines: base.concat([
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "LIRI_BUILD_LIRILIBINPUT_LIB",
            "LIRILIBINPUT_STATIC_LIB"
        ])

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
            Depends { name: "libinput" }
            Depends { name: "Qt5Udev" }
            Depends { name: "LiriLogind" }

            cpp.defines: project.defines
            cpp.includePaths: product.sourceDirectory
        }
    }
}
