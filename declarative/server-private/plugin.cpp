/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(QWaylandWlShellSurface)
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

    qmlRegisterType<QWaylandWlShellSurfaceQuickExtensionChildren>(uri, 1, 0, "WlShellSurface");
    qmlRegisterUncreatableType<QWaylandXdgShellV5>(uri, 1, 0, "XdgShellV5Base", QObject::tr("Cannot create instance of XdgShellV5Base"));
    qmlRegisterType<QWaylandXdgSurfaceV5QuickExtensionChildren>(uri, 1, 0, "XdgSurfaceV5");
    qmlRegisterType<QWaylandXdgPopupV5QuickExtensionChildren>(uri, 1, 0, "XdgPopupV5");
}

#include "plugin.moc"
