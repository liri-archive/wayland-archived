/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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
#include <QtWaylandCompositor/QWaylandWlShell>
#include <QtWaylandCompositor/QWaylandXdgShellV5>

#define LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(className) \
    class Q_WAYLAND_COMPOSITOR_EXPORT className##QuickExtensionChildren : public className \
    { \
/* qmake ignore Q_OBJECT */ \
        Q_OBJECT \
        Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false) \
        Q_CLASSINFO("DefaultProperty", "data") \
    public: \
        QQmlListProperty<QObject> data() \
        { \
            return QQmlListProperty<QObject>(this, m_objects); \
        } \
    private: \
        QList<QObject *> m_objects; \
    };

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(QWaylandWlShell)
LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(QWaylandWlShellSurface)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(QWaylandXdgShellV5)
LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(QWaylandXdgSurfaceV5)
LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(QWaylandXdgPopupV5)

class LiriWaylandServerPrivatePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri);
};

void LiriWaylandServerPrivatePlugin::registerTypes(const char *uri)
{
    // @uri Liri.WaylandServer.Private
    Q_ASSERT(QLatin1String(uri) == QLatin1String("Liri.WaylandServer.Private"));

    qmlRegisterType<QWaylandWlShellQuickExtension>(uri, 1, 0, "WlShell");
    qmlRegisterType<QWaylandWlShellSurfaceQuickExtensionChildren>(uri, 1, 0, "WlShellSurface");
    qmlRegisterType<QWaylandXdgShellV5QuickExtension>(uri, 1, 0, "XdgShellV5");
    qmlRegisterUncreatableType<QWaylandXdgShellV5>(uri, 1, 0, "XdgShellV5Base", QObject::tr("Cannot create instance of XdgShellV5Base"));
    qmlRegisterType<QWaylandXdgSurfaceV5QuickExtensionChildren>(uri, 1, 0, "XdgSurfaceV5");
    qmlRegisterType<QWaylandXdgPopupV5QuickExtensionChildren>(uri, 1, 0, "XdgPopupV5");
}

#include "plugin.moc"
