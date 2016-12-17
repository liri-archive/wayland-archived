/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2012-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL$
 *
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 or later as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPLv21 included in the packaging of
 * this file.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements will be
 * met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * Alternatively, this file may be used under the terms of the GNU General
 * Public License version 2.0 or later as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPLv2 included in the
 * packaging of this file.  Please review the following information to ensure
 * the GNU General Public License version 2.0 requirements will be
 * met: http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/QQmlComponent>
#include <QtWaylandCompositor/QWaylandQuickExtension>

#include <Liri/WaylandServer/ApplicationManager>
#include <Liri/WaylandServer/ClientWindow>
#include <Liri/WaylandServer/ClientWindowQuickItem>
#include <Liri/WaylandServer/CompositorSettings>
#include <Liri/WaylandServer/OutputChangeset>
#include <Liri/WaylandServer/OutputManagement>
#include <Liri/WaylandServer/QuickOutput>
#include <Liri/WaylandServer/QuickOutputConfiguration>
#include <Liri/WaylandServer/GtkShell>
#include <Liri/WaylandServer/Screen>
#include <Liri/WaylandServer/Screencaster>
#include <Liri/WaylandServer/Screenshooter>
//#include <Liri/WaylandServer/TaskManager>
#include <Liri/WaylandServer/QuickScreenManager>

#include "fpscounter.h"
#include "keyeventfilter.h"

using namespace Liri::WaylandServer;

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(ApplicationManager)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(GtkShell)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(OutputManagement)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(Screencaster)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(Screenshooter)

class LiriWaylandServerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri);
};

void LiriWaylandServerPlugin::registerTypes(const char *uri)
{
    // @uri Liri.WaylandServer
    Q_ASSERT(QLatin1String(uri) == QLatin1String("Liri.WaylandServer"));

    // More specialized output
    qmlRegisterType<QuickOutput>(uri, 1, 0, "ExtendedOutput");

    // gtk-shell
    qmlRegisterType<GtkShellQuickExtension>(uri, 1, 0, "GtkShell");
    qmlRegisterType<GtkSurface>(uri, 1, 0, "GtkSurface");

    // Screen
    qmlRegisterType<QuickScreenManager>(uri, 1, 0, "ScreenManager");
    qmlRegisterUncreatableType<Screen>(uri, 1, 0, "Screen",
                                       QObject::tr("Cannot create instance of Screen"));

    // Output management
    qmlRegisterType<QuickOutputConfiguration>(uri, 1, 0, "OutputConfiguration");
    qmlRegisterUncreatableType<OutputManagement>(uri, 1, 0, "OutputManagementBase",
                                                 QObject::tr("Cannot create instance of OutputManagementBase, use OutputManagement instead"));
    qmlRegisterType<OutputManagementQuickExtension>(uri, 1, 0, "OutputManagement");
    qmlRegisterUncreatableType<OutputChangeset>(uri, 1, 0, "OutputChangeset",
                                                QObject::tr("Cannot create instance of OutputChangeset"));

    // Application manager
    qmlRegisterUncreatableType<ApplicationManager>(uri, 1, 0, "ApplicationManagerBase",
                                                   QObject::tr("Cannot create instance of ApplicationManagerBase, use ApplicationManager instead"));
    qmlRegisterType<ApplicationManagerQuickExtension>(uri, 1, 0, "ApplicationManager");
    qmlRegisterUncreatableType<ClientWindow>(uri, 1, 0, "ClientWindow",
                                             QObject::tr("Cannot create instance of ClientWindow"));
    qmlRegisterType<ClientWindowQuickItem>(uri, 1, 0, "ClientWindowItem");

    // Task manager
    //qmlRegisterType<TaskManager>(uri, 1, 0, "TaskManager");
    //qmlRegisterUncreatableType<TaskItem>(uri, 1, 0, "TaskItem",
                                         //QObject::tr("Cannot create instance of TaskItem"));

    // Screencaster
    qmlRegisterType<ScreencasterQuickExtension>(uri, 1, 0, "Screencaster");
    qmlRegisterUncreatableType<Screencast>(uri, 1, 0, "Screencast",
                                           QObject::tr("Cannot create instance of Screencast"));

    // Screen shooter
    qmlRegisterType<ScreenshooterQuickExtension>(uri, 1, 0, "Screenshooter");
    qmlRegisterUncreatableType<Screenshot>(uri, 1, 0, "Screenshot",
                                           QObject::tr("Cannot create instance of Screenshot"));

    // Key event filter
    qmlRegisterType<KeyEventFilter>(uri, 1, 0, "KeyEventFilter");

    // Settings
    qmlRegisterType<CompositorSettings>(uri, 1, 0, "CompositorSettings");

    // Misc
    qmlRegisterType<FpsCounter>(uri, 1, 0, "FpsCounter");
}

#include "plugin.moc"
