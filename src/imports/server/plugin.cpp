/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/QQmlComponent>
#include <QtWaylandCompositor/QWaylandQuickExtension>

#include <LiriWaylandServer/CompositorSettings>
#include <LiriWaylandServer/OutputChangeset>
#include <LiriWaylandServer/OutputManagement>
#include <LiriWaylandServer/QuickOutput>
#include <LiriWaylandServer/QuickOutputConfiguration>
#include <LiriWaylandServer/GtkShell>
#include <LiriWaylandServer/Screen>
#include <LiriWaylandServer/Screencaster>
#include <LiriWaylandServer/Screenshooter>
//#include <LiriWaylandServer/TaskManager>
#include <LiriWaylandServer/QuickScreenManager>

#include "chromeitem.h"
#include "fpscounter.h"
#include "hotspot.h"
#include "keyeventfilter.h"

using namespace Liri::WaylandServer;

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

    // Chrome container
    qmlRegisterType<ChromeItem>(uri, 1, 0, "ChromeItem");

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

    // Hotspot
    qmlRegisterType<HotSpot>(uri, 1, 0, "HotSpot");

    // Settings
    qmlRegisterType<CompositorSettings>(uri, 1, 0, "CompositorSettings");

    // Misc
    qmlRegisterType<FpsCounter>(uri, 1, 0, "FpsCounter");
}

#include "plugin.moc"
