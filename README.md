Liri Wayland
============

[![ZenHub.io](https://img.shields.io/badge/supercharged%20by-zenhub.io-blue.svg)](https://zenhub.io)

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/wayland.svg)](https://github.com/lirios/wayland)
[![Build Status](https://travis-ci.org/lirios/wayland.svg?branch=develop)](https://travis-ci.org/lirios/wayland)
[![GitHub issues](https://img.shields.io/github/issues/lirios/wayland.svg)](https://github.com/lirios/wayland/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2017.svg)](https://github.com/lirios/wayland/commits/develop)

Wayland client and server libraries for Liri OS.

## Features

This project includes:

* Qt-style API for Wayland clients
* Client and server extensions for screenshots, screencast,
  applicaion manager, task manager and output management
* XWayland support
* Screen configuration
* Qt-style API for udev, logind and libinput
* QPA plugin
* QtWaylandClient fullscreen shell integration
* Pluggable hardware abstraction with support for:
  * DRM/KMS
  * All Raspberry Pi boards
  * Mali
  * Vivante

## Dependencies

Compiler requirements:

* [gcc >= 4.8](https://gcc.gnu.org/gcc-4.8/) or
* [Clang](http://clang.llvm.org/)

Qt >= 5.8.0 with at least the following modules is required:

* [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
* [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
* [qtwayland](http://code.qt.io/cgit/qt/qtwayland.git)

And the following modules:

 * [qbs-shared](https://github.com/lirios/qbs-shared.git)

The following modules and their dependencies are required:

* [udev](http://www.freedesktop.org/software/systemd/libudev/)
* [libinput >= 0.12](http://www.freedesktop.org/wiki/Software/libinput/)

If you enable DRM/KMS device integration you also need:

* [libdrm](https://wiki.freedesktop.org/dri/)
* [gbm](http://www.mesa3d.org)

If you enable Broadcom VideoCore device integration you also need:

* [bcmhost](https://github.com/raspberrypi/firmware)

If you enable Vivante device integration you also need:

* [mx6q](https://community.freescale.com/docs/DOC-95560)

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
qbs -d build -j $(nproc) profile:qt5 # use sudo if necessary
```

On the last `qbs` line, you can specify additional configuration parameters at the end:

 * `qbs.installRoot:/path/to/install` (for example `/`)
 * `qbs.installPrefix:path/to/install` (for example `opt/liri` or `usr`)

The following are only needed if `qbs.installPrefix` is a system-wide path such as `usr`
and the default value doesn't suit your needs. All are relative to `qbs.installRoot`:

 * `modules.lirideployment.libDir:path/to/lib` where libraries are installed (default: `lib`)
 * `modules.lirideployment.qmlDir:path/to/qml` where QML plugins are installed (default: `lib/qml`)
 * `modules.lirideployment.pluginsDir:path/to/plugins` where Qt plugins are installed (default: `lib/plugins`)
 * `modules.lirideployment.qbsModulesDir:path/to/qbs` where Qbs modules are installed (default: `share/qbs/modules`)

See [lirideployment.qbs](https://github.com/lirios/qbs-shared/blob/develop/modules/lirideployment/lirideployment.qbs)
for more deployment-related parameters.

You can also specify the following options:

 * `products.kms.condition:false`: Do not build KMS EGL device integration.
 * `products.lirixwaylandplugin.condition:false`: Do not build XWayland support.
 * `products.fullscreen-shell.condition:false`: Do not build fullscreen-shell integration.
 * `products.materialdecorationplugin.condition:false`: Do not build material decoration.
 * `products.lirieglfs.condition:false`: Do not build QPA plugin.

If you specify `qbs.installRoot` you might need to prefix the entire line with `sudo`,
depending on whether you have permissions to write there or not.

### Environment variables

The liri QPA plugin supports different graphics hardware through EGL
device integration plugins.

There are a number of environment variables that can influence the QPA
plugin:

* **LIRI_QPA_DEBUG:** When set, some debugging information is
  printed on the debug output such as the EGL configuration being used
  while creating a new context.

* **LIRI_QPA_WIDTH** and **LIRI_QPA_HEIGHT:**
  Screen width and height in pixels (screen resolution). This variable
  is used only on platforms where detecting it from the framebuffer device
  /dev/fb0 fails.

* **LIRI_QPA_DEPTH:** Overrides the color depth for the screen.
  On platforms where the framebuffer device /dev/fb0 is not available or
  the query is not successful, the default value of 32 is used.
  Setting this variable can override any such default.
  Please note that this affects only the color depth reported by the
  screen and it is irrelevant to EGL configurations and the color depth
  used for OpenGL rendering.

* **LIRI_QPA_PHYSICAL_WIDTH** and **LIRI_QPA_PHYSICAL_HEIGHT:**
  Physical screen width and height in millimiters. On platforms where the
  /dev/fb0 framebuffer device is not available, or the query is not
  successful, the values are calculated based on a default DPI of 100.
  These variables can be used to override such default values.

* **LIRI_QPA_CONFIG:** Path to the QPA plugin configuration, used
  only by the kms EGL device integration. By default the path is
  "~/.config/liri/platform.json".

* **LIRI_QPA_SWAPINTERVAL:** By default a swap interval of 1 will
  be requested. This enables synchronizing to the display vertical
  refresh. Passing 0 will disable blocking on swap, resulting in running
  as fast as it's possible without any synchronization.

* **LIRI_QPA_FORCEVSYNC:** When set, FBIO_WAITFORVSYNC is
  requested on the framebuffer device.

* **LIRI_QPA_FORCE888:** When set, the red, green and blue color
  channels are ignored whenever creating a new context, window or
  offscreen surface. Instead a configuration of 8 bits per channel is
  requested. Useful on devices where configurations with less than
  24 or 32 bits per channel are chosen by default but are not suitable,
  for example due to banding effects. This variable moves the control
  of color channels from the compositor to a setting that allows a
  more flexible per-device configuration.

* **LIRI_QPA_ENABLE_TERMINAL_KEYBOARD:** By default terminal
  keyboard is disabled on compositor startup by setting tty's keyboard
  mode to K_OFF preventing keystrokes from going to the terminal.

* **LIRI_QPA_INTEGRATION:** EGL device integration to be loaded
  at compositor startup. By default the QPA plugin autodetects which
  plugin needs to be loaded, if for some reason autodetection fails
  a specific plugin can be set with this variable. Set the EGL device
  integration plugin file name without path and extension (for example
  "kms" for the DRM/KMS integration, "brcm" for Broadcom etc...).

* **LIRI_QPA_HIDECURSOR:** Set to 1 in order to hide the hardware cursor.

### Logging categories

Qt 5.2 introduced logging categories and Hawaii takes advantage of
them to make debugging easier.

Please refer to the [Qt](http://doc.qt.io/qt-5/qloggingcategory.html) documentation
to learn how to enable them.

### Available categories

* Server-side:
  * **liri.wlserver:** Core compositor classes
  * **liri.wlserver.screenbackend.native:** Native screen backend
  * **liri.wlserver.screenbackend.fake:** Fake screen backend
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
  * **liri.qpa.logind:** Logind integration
  * **liri.qpa.udev:** udev integration

* EGL Device Integrations:
  * **liri.qpa.kms:** KMS/DRM EGL device integration

## Licensing

Licensed under either of the following licenses:

* GNU Lesser General Public License version 3.0
* GNU General Public License version 2.0 or (at your option)
  version 3.0 or any later version approved by the
  KDE Free Qt Foundation.
