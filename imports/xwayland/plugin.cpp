/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "xwayland.h"
#include "xwaylandmanager.h"
#include "xwaylandserver.h"
#include "xwaylandshellsurface.h"
#include "xwaylandquickshellsurfaceitem.h"

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

LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(XWayland)
LIRI_DECLARE_QUICK_EXTENSION_CHILDREN_CLASS(XWaylandShellSurface)

class LiriXWaylandPluging : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri);
};

void LiriXWaylandPluging::registerTypes(const char *uri)
{
    // @uri Liri.XWayland
    Q_ASSERT(QLatin1String(uri) == QLatin1String("Liri.XWayland"));

    qmlRegisterType<XWaylandQuickExtensionChildren>(uri, 1, 0, "XWayland");
    qmlRegisterUncreatableType<XWaylandManager>(uri, 1, 0, "XWaylandManager",
                                                QLatin1String("Cannot instantiate XWaylandManager"));
    qmlRegisterUncreatableType<XWaylandShellSurface>(uri, 1, 0, "XWaylandShellSurfaceBase",
                                                     QLatin1String("Cannot instantiate XWaylandShellSurfaceBase"));
    qmlRegisterType<XWaylandShellSurfaceQuickExtensionChildren>(uri, 1, 0, "XWaylandShellSurface");
    qmlRegisterType<XWaylandQuickShellSurfaceItem>(uri, 1, 0, "XWaylandShellSurfaceItem");
}

#include "plugin.moc"
