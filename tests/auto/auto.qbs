import qbs 1.0

Project {
    name: "Autotests"

    references: [
        "client/display/display.qbs",
        //"client/output/output.qbs",
        //"client/registry/registry.qbs",
        "client/shmpool/shmpool.qbs",
        //"logind/logind.qbs",
        //"udev/udev.qbs",
    ]

    AutotestRunner {
        Depends { name: "LiriWaylandClient" }
        Depends { name: "LiriWaylandServer" }

        builtByDefault: project.autotestEnabled
        name: "liri-wayland-autotest"
        arguments: project.autotestArguments
        wrapper: project.autotestWrapper
        environment: {
            var env = base;
            var found = false;
            for (var i in env) {
                if (env[i].startsWith("XDG_RUNTIME_DIR=")) {
                    found = true;
                    break;
                }
            }
            if (!found)
                env.push("XDG_RUNTIME_DIR=/tmp");
            return env;
        }
    }
}
