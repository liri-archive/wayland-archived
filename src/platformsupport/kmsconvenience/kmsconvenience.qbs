import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: moduleName
    moduleName: "LiriKmsSupport"
    description: "KMS shared code"
    createPkgConfig: false
    createCMake: false
    conditionFunction: (function() {
        if (!libdrm.found) {
            console.error("libdrm is required to build " + targetName);
            return false;
        }

        return true;
    })

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

    LiriPrivateModule {
        targetName: project.targetName

        Depends { name: project.headersName }
        Depends { name: "Qt"; submodules: ["core-private", "gui-private"] }
        Depends { name: "libdrm" }

        cpp.defines: base.concat([
            'LIRIWAYLAND_VERSION="' + project.version + '"',
        ])

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends { name: "Qt"; submodules: ["core-private", "gui-private"] }
            Depends { name: "libdrm" }

            cpp.defines: project.defines
            cpp.includePaths: product.sourceDirectory
        }
    }
}
