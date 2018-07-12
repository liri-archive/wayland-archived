import qbs 1.0
import qbs.FileInfo
import LiriUtils

LiriModuleProject {
    id: root

    name: "LiriPlatformHeaders"
    moduleName: "LiriPlatformHeaders"
    createPkgConfig: false
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.gui") }],
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName

        Group {
            name: "Headers"
            files: [
                "lirieglfsfunctions.h",
                "liriplatformheadersglobal.h",
            ]
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
            submodules: [ "gui" ]
            versionAtLeast: project.minimumQtVersion
        }

        cpp.defines: base.concat(["LIRI_BUILD_PLATFORMHEADERS"])

        files: [
            "lirieglfsfunctions.cpp",
        ]

        Group {
            qbs.install: true
            qbs.installDir: FileInfo.joinPaths(lirideployment.includeDir, root.moduleName, project.version, root.moduleName, "private")
            fileTagsFilter: ["hpp_private"]
        }

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends {
                name: "Qt"
                submodules: [ "gui" ]
                versionAtLeast: project.minimumQtVersion
            }

            cpp.includePaths: base.concat([product.buildDirectory])
        }
    }
}
