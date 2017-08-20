import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: moduleName
    moduleName: "LiriKmsSupport"
    description: "KMS shared code"
    createPkgConfig: false
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("libdrm") }],
    })

    LiriHeaders {
        Group {
            name: "Headers"
            files: "**/*.h"
            fileTags: ["hpp_syncable"]
        }
    }

    LiriModule {
        name: project.moduleName
        targetName: project.targetName
        version: "0.0.0"
        type: ["staticlibrary"]

        Depends { name: project.headersName }
        Depends { name: "Qt"; submodules: ["core-private", "gui-private"] }
        Depends { name: "libdrm" }

        condition: {
            if (!libdrm.found) {
                console.error("libdrm is required to build " + targetName);
                return false;
            }

            return true;
        }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_NO_CAST_FROM_ASCII",
        ]

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends { name: "Qt"; submodules: ["core-private", "gui-private"] }
            Depends { name: "libdrm" }

            cpp.includePaths: product.sourceDirectory
        }
    }
}
