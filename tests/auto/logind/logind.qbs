import qbs 1.0

QtApplication {
    name: "tst_wl_logind"
    type: base.concat(["autotest"])

    Depends { name: "Qt"; submodules: ["gui", "testlib"] }
    Depends { name: "LiriLogind" }

    files: ["*.cpp", "*.h"]
}
