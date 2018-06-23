import qbs 1.0

Project {
    name: "Wayland"

    readonly property string version: "0.9.1"
    readonly property var versionParts: version.split('.').map(function(part) { return parseInt(part); })

    readonly property string minimumQtVersion: "5.9.0"

    property bool useStaticAnalyzer: false
    property bool autotestEnabled: false
    property stringList autotestArguments: []
    property stringList autotestWrapper: []

    condition: qbs.targetOS.contains("linux") && !qbs.targetOS.contains("android")

    minimumQbsVersion: "1.8.0"

    references: [
        "src/deployment.qbs",
        "src/libs.qbs",
        "src/imports/imports.qbs",
        "src/plugins/plugins.qbs",
        "tests/manual/manual.qbs",
    ]
}
