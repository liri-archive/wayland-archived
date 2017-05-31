import qbs 1.0

LiriModule {
    name: "LiriWaylandServer"
    targetName: "LiriWaylandServer"
    version: "0.0.0"
    createCMake: false

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

    condition: {
        if (!project.withLibraries) {
            console.info("Libraries disabled");
            return false;
        }

        return true;
    }

    cpp.defines: [
        'LIRIWAYLAND_VERSION="' + project.version + '"',
        "QT_BUILD_LIRIWAYLANDSERVER_LIB",
        "QT_WAYLAND_COMPOSITOR_QUICK"
    ]
    cpp.commonCompilerFlags: base.concat(["-Wno-deprecated-declarations"])

    create_headers.headersMap: ({
        "compositorsettings.h": "CompositorSettings",
        "homeapplication.h": "HomeApplication",
        "quickoutput.h": "QuickOutput",
        "gtkshell.h": "GtkShell",
        "screencaster.h": "Screencaster,Screencast",
        "screenshooter.h": "Screenshooter,Screenshot",
        "outputchangeset.h": "OutputChangeset",
        "outputconfiguration.h": "OutputConfiguration",
        "outputmanagement.h": "OutputManagement",
        "quickoutputconfiguration.h": "QuickOutputConfiguration",
        "fakescreenbackend.h": "FakeScreenBackend",
        "nativescreenbackend.h": "NativeScreenBackend",
        "quickscreenmanager.h": "QuickScreenManager",
        "screenbackend.h": "ScreenBackend,Screen",
        "screenmanager.h": "ScreenManager",
    })

    create_pkgconfig.name: "Liri Wayland Server"
    create_pkgconfig.description: "Qt API for Wayland compositors"
    create_pkgconfig.version: project.version
    create_pkgconfig.dependencies: ["Qt5Core", "Qt5Gui", "Qt5WaylandCompositor"]

    files: [
        "*.cpp",
        "core/*.cpp",
        "extensions/*.cpp",
        "output/*.cpp",
        "screen/*.cpp"
    ]
    excludeFiles: ["extensions/taskmanager.cpp"]

    Group {
        name: "Headers"
        files: [
            "*.h",
            "core/*.h",
            "extensions/*.h",
            "output/*.h",
            "screen/*.h"
        ]
        excludeFiles: [
            "*_p.h",
            "core/*_p.h",
            "extensions/*_p.h",
            "output/*_p.h",
            "screen/*_p.h",
            "extensions/taskmanager.h"
        ]
        fileTags: ["public_headers"]
    }

    Group {
        name: "Private Headers"
        files: [
            "*_p.h",
            "core/*_p.h",
            "extensions/*_p.h",
            "output/*_p.h",
            "screen/*_p.h"
        ]
        excludeFiles: ["logging_p.h", "extensions/taskmanager_p.h"]
        fileTags: ["private_headers"]
    }

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

    Export {
        Depends { name: "cpp" }
        Depends {
            name: "Qt"
            submodules: [
                "core", "core-private", "gui", "gui-private",
                "qml", "quick", "quick-private",
                "waylandcompositor", "waylandcompositor-private"
            ]
        }
        Depends { name: "LiriEglFS" }

        cpp.defines: ["QT_WAYLAND_COMPOSITOR_QUICK"]
        cpp.includePaths: product.generatedHeadersDir
    }
}
