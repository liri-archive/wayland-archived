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

    LiriModule {
        name: project.moduleName
        targetName: project.targetName
        version: "0.0.0"
        type: ["staticlibrary"]

        Depends { name: project.headersName }
        Depends { name: "Qt.core-private" }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_NO_CAST_FROM_ASCII",
        ]

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends { name: "Qt.core-private" }

            cpp.includePaths: product.sourceDirectory
        }
    }
}
