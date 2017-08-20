import qbs 1.0
import LiriUtils

LiriModuleProject {
    name: "LiriEglFSKmsSupport"
    moduleName: "LiriEglFSKmsSupport"
    description: "KMS support for EGL device integration"
    createPkgConfig: false
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("LiriEglFSDeviceIntegration") },
                  { name: LiriUtils.quote("LiriEdidSupport") },
                  { name: LiriUtils.quote("LiriKmsSupport") }],
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

        Depends { name: project.headersName }
        Depends {
            name: "Qt"
            submodules: ["core-private", "gui-private"]
        }
        Depends { name: "LiriEglFSDeviceIntegration" }
        Depends { name: "LiriEdidSupport" }
        Depends { name: "LiriKmsSupport" }
        Depends { name: "gbm" }
        Depends { name: "libdrm" }

        condition: {
            if (!gbm.found) {
                console.error("gbm is required to build " + targetName);
                return false;
            }

            if (!libdrm.found) {
                console.error("libdrm is required to build " + targetName);
                return false;
            }

            return true;
        }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
        ]

        files: ["*.cpp", "*.h"]

        Export {
            Depends { name: "cpp" }
            Depends { name: project.headersName }
            Depends {
                name: "Qt"
                submodules: ["core-private", "gui-private"]
            }
            Depends { name: "LiriEglFSDeviceIntegration" }
            Depends { name: "LiriEdidSupport" }
            Depends { name: "LiriKmsSupport" }
            Depends { name: "gbm" }
            Depends { name: "libdrm" }

            cpp.includePaths: product.sourceDirectory
        }
    }
}
