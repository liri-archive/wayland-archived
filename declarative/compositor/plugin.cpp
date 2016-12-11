/****************************************************************************
 * This file is part of Hawaii.
 *
 * Copyright (C) 2012-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
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

#include <GreenIsland/Server/ApplicationManager>
#include <GreenIsland/Server/ClientWindow>
#include <GreenIsland/Server/ClientWindowQuickItem>
#include <GreenIsland/Server/CompositorSettings>
#include <GreenIsland/Server/OutputChangeset>
#include <GreenIsland/Server/OutputManagement>
#include <GreenIsland/Server/QuickOutput>
#include <GreenIsland/Server/QuickOutputConfiguration>
#include <GreenIsland/Server/GtkShell>
#include <GreenIsland/Server/Screen>
#include <GreenIsland/Server/Screencaster>
#include <GreenIsland/Server/Screenshooter>
//#include <GreenIsland/Server/TaskManager>
#include <GreenIsland/Server/QuickScreenManager>

#include "fpscounter.h"
#include "keyeventfilter.h"

using namespace GreenIsland::Server;

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(ApplicationManager)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(GtkShell)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(OutputManagement)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(Screencaster)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(Screenshooter)

class GreenIslandPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri);
};

void GreenIslandPlugin::registerTypes(const char *uri)
{
    // @uri GreenIsland
    Q_ASSERT(QLatin1String(uri) == QLatin1String("GreenIsland"));

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
