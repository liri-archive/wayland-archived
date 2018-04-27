Liri Wayland
============

[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/wayland.svg)](https://github.com/lirios/wayland)
[![Build Status](https://travis-ci.org/lirios/wayland.svg?branch=develop)](https://travis-ci.org/lirios/wayland)
[![GitHub issues](https://img.shields.io/github/issues/lirios/wayland.svg)](https://github.com/lirios/wayland/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2018.svg)](https://github.com/lirios/wayland/commits/develop)

Wayland client and server libraries for Liri OS.

## Features

This project includes:

* Qt-style API for Wayland clients
* Client and server extensions for screenshots, screencast,
  applicaion manager, task manager and output management
* XWayland support
* Screen configuration
* Qt-style API for libinput
* QPA plugin
* QtWaylandClient fullscreen shell integration
* Pluggable hardware abstraction with support for:
  * DRM/KMS

## Dependencies

Compiler requirements:

* [gcc](https://gcc.gnu.org/gcc-4.8/) >= 4.8 or
* [Clang](http://clang.llvm.org/)

Qt >= 5.9.0 with at least the following modules is required:

* [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
* [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
* [qtwayland](http://code.qt.io/cgit/qt/qtwayland.git)

And the following modules:

 * [qbs-shared](https://github.com/lirios/qbs-shared.git) >= 1.2.0
 * [libliri](https://github.com/lirios/libliri.git)
 * [qtudev](https://github.com/lirios/qtudev.git) >= 1.0.0

The following modules and their dependencies are required:

* [libinput](http://www.freedesktop.org/wiki/Software/libinput/) >= 0.12

If you enable DRM/KMS device integration you also need:

* [libdrm](https://wiki.freedesktop.org/dri/)
* [gbm](http://www.mesa3d.org)

If you enable XWayland support you also need:

* [xcb-util-cursor](http://cgit.freedesktop.org/xcb/util-cursor)

## Installation

Qbs is a new build system that is much easier to use compared to qmake or CMake.

If you want to learn more, please read the [Qbs manual](http://doc.qt.io/qbs/index.html),
especially the [setup guide](http://doc.qt.io/qbs/configuring.html) and how to install artifacts
from the [installation guide](http://doc.qt.io/qbs/installing-files.html).

From the root of the repository, run:

```sh
qbs setup-toolchains --type gcc /usr/bin/g++ gcc
qbs setup-qt /usr/bin/qmake-qt5 qt5
qbs config profiles.qt5.baseProfile gcc
```

Then, from the root of the repository, run:

```sh
qbs -d build -j $(nproc) profile:qt5 # use sudo if necessary
```

To the `qbs` call above you can append additional configuration parameters:

 * `modules.lirideployment.prefix:/path/to/prefix` where most files are installed (default: `/usr/local`)
 * `modules.lirideployment.dataDir:path/to/lib` where data files are installed (default: `/usr/local/share`)
 * `modules.lirideployment.libDir:path/to/lib` where libraries are installed (default: `/usr/local/lib`)
 * `modules.lirideployment.qmlDir:path/to/qml` where QML plugins are installed (default: `/usr/local/lib/qml`)
 * `modules.lirideployment.pluginsDir:path/to/plugins` where Qt plugins are installed (default: `/usr/local/lib/plugins`)
 * `modules.lirideployment.qbsModulesDir:path/to/qbs` where Qbs modules are installed (default: `/usr/local/share/qbs/modules`)

See [lirideployment.qbs](https://github.com/lirios/qbs-shared/blob/develop/modules/lirideployment/lirideployment.qbs)
for more deployment-related parameters.

You can also specify the following options:

 * `products.kms.condition:false`: Do not build KMS EGL device integration.
 * `products.lirixwaylandplugin.condition:false`: Do not build XWayland support.
 * `products.fullscreen-shell.condition:false`: Do not build fullscreen-shell integration.
 * `products.materialdecorationplugin.condition:false`: Do not build material decoration.
 * `products.lirieglfs.condition:false`: Do not build QPA plugin.

### Environment variables

The liri QPA plugin supports different graphics hardware through EGL
device integration plugins.

The environment variables that can influence the QPA plugin
are the same as those of [eglfs](http://doc.qt.io/qt-5/embedded-linux.html#eglfs).

### Logging categories

Qt 5.2 introduced logging categories and Liri takes advantage of
them to make debugging easier.

Please refer to the [Qt](http://doc.qt.io/qt-5/qloggingcategory.html) documentation
to learn how to enable them.

### Available categories

* Server-side:
  * **liri.wlserver:** Core compositor classes
  * **liri.wlserver.qml:** Liri.WaylandServer QML plugin
  * **liri.wlserver.outputmanagement:** The liri_outputmanagement protocol
  * **liri.wlserver.gtkshell:** The gtk_shell protocol
  * **liri.wlserver.gtkshell.trace:** The gtk_shell protocol trace
  * **liri.wlserver.screencaster:** The liri_screencaster protocol
  * **liri.wlserver.screenshooter:** The liri_screenshooter protocol
  * **liri.wlserver.taskmanager:** The liri_windows
  * **liri.xwayland:** xwayland
  * **liri.xwayland.trace:** xwayland protocol trace

* Client-side:
  * **liri.wlclient.registry:** Registry
  * **liri.wlclient.shmpool:** Shared memory pool
  * **liri.wlcient.cursortheme:** Cursor theme
  * **liri.wlclient.fullscreenshell:** Full screen shell

* Platform:
  * **liri.qpa.deviceintegration:** Device integration
  * **liri.qpa.input:** Input management on device integration plugins

* EGL Device Integrations:
  * **liri.qpa.kms:** KMS/DRM EGL device integration
  * **liri.qpa.vthandler:** Vt handler

## Licensing

Licensed under the terms of the GNU General Public License version 3.
