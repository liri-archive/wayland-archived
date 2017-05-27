import qbs 1.0

LiriModule {
    name: "LiriWaylandClient"
    targetName: "LiriWaylandClient"
    version: "0.0.0"

    Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
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

    create_headers.headersMap: ({
        "buffer.h": "Buffer",
        "clientconnection.h": "ClientConnection",
        "compositor.h": "Compositor",
        "cursortheme.h": "CursorTheme",
        "fullscreenshell.h": "FullScreenShell",
        "keyboard.h": "Keyboard",
        "output.h": "Output",
        "outputconfiguration.h": "OutputConfiguration",
        "outputmanagement.h": "OutputManagement",
        "pointer.h": "Pointer",
        "region.h": "Region",
        "registry.h": "Registry",
        "screencaster.h": "Screencaster",
        "screenshooter.h": "Screenshooter",
        "seat.h": "Seat",
        "shm.h": "Shm",
        "shmpool.h": "ShmPool",
        "surface.h": "Surface",
        "touch.h": "Touch",
    })

    create_pkgconfig.name: "Liri Wayland Client"
    create_pkgconfig.description: "Qt API for Wayland clients"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "Qt5Gui", "wayland-client", "wayland-cursor"]

    create_cmake.version: project.version
    create_cmake.dependencies: ({
        "Qt5Core": "5.8.0",
        "Qt5Gui": "5.8.0"
    })
    create_cmake.linkLibraries: ["Qt5::Core", "Qt5::Gui"]

    files: ["*.cpp"]

    Group {
        name: "Headers"
        files: ["*.h"]
        excludeFiles: ["*_p.h"]
        fileTags: ["public_headers"]
    }

    Group {
        name: "Private Headers"
        files: ["*_p.h"]
        fileTags: ["private_headers"]
    }

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

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "core-private", "gui", "gui-private"] }
        Depends { name: "Wayland"; submodules: ["client", "cursor"] }

        cpp.includePaths: product.generatedHeadersDir
    }
}
