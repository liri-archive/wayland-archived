import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: "LiriEdidSupport"
    moduleName: "LiriEdidSupport"
    description: "EDID parser for EGL device integration"
    createPkgConfig: false
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core-private") }],
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
        Depends { name: "Qt.core-private" }

        cpp.defines: base.concat([
            'LIRIWAYLAND_VERSION="' + project.version + '"',
        ])

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends { name: "Qt.core-private" }

            cpp.defines: project.defines
            cpp.includePaths: product.sourceDirectory
        }
    }
}
