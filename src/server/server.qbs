import qbs 1.0
import qbs.FileInfo
import LiriUtils

LiriModuleProject {
    id: root

    name: "LiriWaylandServer"
    moduleName: "LiriWaylandServer"
    description: "Qt API for Wayland compositors"
    pkgConfigDependencies: ["Qt5Core", "Qt5Gui", "Qt5WaylandCompositor"]
    createCMake: false

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") },
                  { name: LiriUtils.quote("Qt.gui") },
                  { name: LiriUtils.quote("Qt.qml") },
                  { name: LiriUtils.quote("Qt.quick") },
                  { name: LiriUtils.quote("Qt.waylandcompositor") }],
        "cpp.defines": ["QT_WAYLAND_COMPOSITOR_QUICK"].map(LiriUtils.quote),
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "compositorsettings.h": ["CompositorSettings"],
            "homeapplication.h": ["HomeApplication"],
            "gtkshell.h": ["GtkShell"],
            "screencaster.h": ["Screencaster", "Screencast"],
            "screenshooter.h": ["Screenshooter", "Screenshot"],
            "outputchangeset.h": ["OutputChangeset"],
            "outputconfiguration.h": ["OutputConfiguration"],
            "outputmanagement.h": ["OutputManagement"],
            "quickoutputconfiguration.h": ["QuickOutputConfiguration"],
        })

        Group {
            name: "Headers"
            files: [
                "*.h",
                "core/*.h",
                "extensions/*.h",
                "output/*.h",
                "*_p.h",
                "core/*_p.h",
                "extensions/*_p.h",
                "output/*_p.h",
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
            submodules: [ "core", "gui", "qml", "quick", "waylandcompositor" ]
            versionAtLeast: project.minimumQtVersion
        }
        Depends { name: "WaylandScanner" }

        cpp.defines: [
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_LIRIWAYLANDSERVER_LIB",
            "QT_WAYLAND_COMPOSITOR_QUICK"
        ]
        cpp.commonCompilerFlags: base.concat(["-Wno-deprecated-declarations"])
        cpp.includePaths: base.concat([product.buildDirectory])

        files: [
            "*.cpp",
            "core/*.cpp",
            "extensions/*.cpp",
            "output/*.cpp",
            "**/*.h"
        ]
        excludeFiles: ["extensions/taskmanager.*"]

        Group {
            name: "Wayland Protocols"
            files: [
                "../../data/protocols/liri/liri-screencaster.xml",
                "../../data/protocols/liri/liri-screenshooter.xml",
                "../../data/protocols/liri/liri-outputmanagement.xml",
                "../../data/protocols/gtk/gtk-shell.xml"
            ]
            fileTags: ["wayland.server.protocol"]
        }

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
                submodules: [ "core", "gui", "qml", "quick", "waylandcompositor" ]
            }

            cpp.defines: base.concat(["QT_WAYLAND_COMPOSITOR_QUICK"])
            cpp.includePaths: base.concat([product.buildDirectory])
        }
    }
}
