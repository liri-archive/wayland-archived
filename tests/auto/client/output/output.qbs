import qbs 1.0

QtApplication {
    name: "tst_wl_client_output"
    type: base.concat(["autotest"])

    Depends {
        name: "Qt"
        submodules: ["gui", "waylandcompositor", "waylandcompositor-private", "testlib"]
        versionAtLeast: project.minimumQtVersion
    }
    Depends { name: "LiriWaylandClient" }

    files: ["*.cpp"]
}
