import qbs 1.0

QtApplication {
    name: "tst_wl_libinput"

    Depends {
        name: "Qt"
        submodules: ["gui", "gui-private", "testlib"]
        versionAtLeast: project.minimumQtVersion
    }
    Depends { name: "LiriLibInput" }
    Depends { name: "LiriLogind" }

    files: ["*.cpp"]
}
