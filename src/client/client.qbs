import qbs 1.0
import qbs.FileInfo
import LiriUtils

LiriModuleProject {
    id: root

    name: "LiriWaylandClient"
    moduleName: "LiriWaylandClient"
    description: "Qt API for Wayland clients"
    pkgConfigDependencies: ["Qt5Core", "Qt5Gui", "wayland-client", "wayland-cursor"]
    cmakeDependencies: ({"Qt5Core": "5.8.0", "Qt5Gui": "5.8.0"})
    cmakeLinkLibraries: ["Qt5::Core", "Qt5::Gui"]

    resolvedProperties: ({
        Depends: [{ name: LiriUtils.quote("Qt.core") }, { name: LiriUtils.quote("Qt.core-private") },
                  { name: LiriUtils.quote("Qt.gui") }, { name: LiriUtils.quote("Qt.gui-private") },
                  { name: LiriUtils.quote("Wayland.client") }, { name: LiriUtils.quote("Wayland.cursor") }],
    })

    LiriHeaders {
        name: root.headersName
        sync.module: root.moduleName
        sync.classNames: ({
            "buffer.h": ["Buffer"],
            "clientconnection.h": ["ClientConnection"],
            "compositor.h": ["Compositor"],
            "cursortheme.h": ["CursorTheme"],
            "fullscreenshell.h": ["FullScreenShell"],
            "keyboard.h": ["Keyboard"],
            "output.h": ["Output"],
            "outputconfiguration.h": ["OutputConfiguration"],
            "outputmanagement.h": ["OutputManagement"],
            "pointer.h": ["Pointer"],
            "region.h": ["Region"],
            "registry.h": ["Registry"],
            "screencaster.h": ["Screencaster"],
            "screenshooter.h": ["Screenshooter"],
            "seat.h": ["Seat"],
            "shm.h": ["Shm"],
            "shmpool.h": ["ShmPool"],
            "surface.h": ["Surface"],
            "touch.h": ["Touch"],
        })

        Group {
            name: "Headers"
            files: "**/*.h"
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
            submodules: ["core", "core-private", "gui", "gui-private"]
            versionAtLeast: project.minimumQtVersion
        }
        Depends { name: "Wayland"; submodules: ["client", "cursor"] }
        Depends { name: "WaylandScanner" }

        condition: {
            if (!Wayland.client.found) {
                console.error("wayland-client is required to build " + targetName);
                return false;
            }

            if (!Wayland.cursor.found) {
                console.error("wayland-cursor is required to build " + targetName);
                return false;
            }

            return true;
        }

        cpp.defines: base.concat([
            'LIRIWAYLAND_VERSION="' + project.version + '"',
            "QT_BUILD_LIRIWAYLANDCLIENT_LIB"
        ])
        cpp.commonCompilerFlags: base.concat(["-Wno-deprecated-declarations"])
        cpp.includePaths: base.concat([product.buildDirectory])

        files: ["*.cpp", "*.h"]

        Group {
            name: "Wayland Protocols"
            files: [
                "../../data/protocols/wayland/wayland.xml",
                "../../data/protocols/wayland/fullscreen-shell-unstable-v1.xml",
                "../../data/protocols/liri/liri-screencaster.xml",
                "../../data/protocols/liri/liri-screenshooter.xml",
                "../../data/protocols/liri/liri-outputmanagement.xml"
            ]
            fileTags: ["wayland.client.protocol"]
        }

        Group {
            qbs.install: true
            qbs.installDir: FileInfo.joinPaths(lirideployment.includeDir, root.moduleName, project.version, root.moduleName, "private")
            fileTagsFilter: ["hpp_private"]
        }

        Export {
            Depends { name: "cpp" }
            Depends { name: root.headersName }
            Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
            Depends { name: "Wayland"; submodules: ["client", "cursor"] }

            cpp.includePaths: base.concat([product.buildDirectory])
        }
    }
}
