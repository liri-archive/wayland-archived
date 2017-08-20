import qbs 1.0

Project {
    name: "eglfs"

    references: [
        "eglfsdeviceintegration.qbs",
        "eglfs-plugin.qbs",
        "deviceintegration/eglfs_kms_support/eglfs_kms_support.qbs",
        "deviceintegration/eglfs_kms/eglfs_kms.qbs",
        "deviceintegration/eglfs_kms_egldevice/eglfs_kms_egldevice.qbs",
    ]
}
