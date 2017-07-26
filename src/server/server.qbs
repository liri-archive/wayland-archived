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
                  { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui") },
                  { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("Qt.qml") },
                  { name: LiriUtils.quote("Qt.quick") },
                  { name: LiriUtils.quote("Qt.quick-private") },
                  { name: LiriUtils.quote("Qt.waylandcompositor") },
                  { name: LiriUtils.quote("Qt.waylandcompositor-private") },
                  { name: LiriUtils.quote("LiriEglFS") }],
        "cpp.defines": ["QT_WAYLAND_COMPOSITOR_QUICK"].map(LiriUtils.quote),
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "compositorsettings.h": ["CompositorSettings"],
            "homeapplication.h": ["HomeApplication"],
            "quickoutput.h": ["QuickOutput"],
            "gtkshell.h": ["GtkShell"],
            "screencaster.h": ["Screencaster", "Screencast"],
            "screenshooter.h": ["Screenshooter", "Screenshot"],
            "outputchangeset.h": ["OutputChangeset"],
            "outputconfiguration.h": ["OutputConfiguration"],
            "outputmanagement.h": ["OutputManagement"],
            "quickoutputconfiguration.h": ["QuickOutputConfiguration"],
            "fakescreenbackend.h": ["FakeScreenBackend"],
            "nativescreenbackend.h": ["NativeScreenBackend"],
            "quickscreenmanager.h": ["QuickScreenManager"],
            "screenbackend.h": ["ScreenBackend", "Screen"],
            "screenmanager.h": ["ScreenManager"],
        })

        Group {
            name: "Headers"
            files: [
                "*.h",
                "core/*.h",
                "extensions/*.h",
                "output/*.h",
                "screen/*.h",
                "*_p.h",
                "core/*_p.h",
                "extensions/*_p.h",
                "output/*_p.h",
                "screen/*_p.h"
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
            submodules: [
                "core", "core-private", "gui", "gui-private",
                "qml", "quick", "quick-private",
                "waylandcompositor", "waylandcompositor-private"
            ]
        }
        Depends { name: "LiriEglFS" }
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
            "screen/*.cpp",
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
                submodules: [
                    "core", "core-private", "gui", "gui-private",
                    "qml", "quick", "quick-private",
                    "waylandcompositor", "waylandcompositor-private"
                ]
            }
            Depends { name: "LiriEglFS" }

            cpp.defines: base.concat(["QT_WAYLAND_COMPOSITOR_QUICK"])
            cpp.includePaths: base.concat([product.buildDirectory])
        }
    }
}
