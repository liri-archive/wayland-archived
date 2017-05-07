import qbs 1.0

QtApplication {
    name: "tst_wl_udev"
    type: base.concat(["autotest"])

    Depends { name: "Qt"; submodules: ["gui", "testlib"] }
    Depends { name: "LiriUDev" }

    files: ["*.cpp"]
}
