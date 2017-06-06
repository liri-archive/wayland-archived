/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef XWAYLAND_H
#define XWAYLAND_H

#include <QtCore/QObject>
#include <QtCore/QLoggingCategory>
#include <QtQml/QQmlParserStatus>

#include <QtWaylandCompositor/QWaylandSurface>

Q_DECLARE_LOGGING_CATEGORY(XWAYLAND)
Q_DECLARE_LOGGING_CATEGORY(XWAYLAND_TRACE)

class XWaylandManager;
class XWaylandServer;
class XWaylandShellSurface;

class XWayland : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QWaylandCompositor *compositor READ compositor WRITE setCompositor NOTIFY compositorChanged)
    Q_PROPERTY(XWaylandManager *manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_INTERFACES(QQmlParserStatus)
public:
    XWayland(QObject *parent = nullptr);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QWaylandCompositor *compositor() const;
    void setCompositor(QWaylandCompositor *compositor);

    XWaylandManager *manager() const;
    void setManager(XWaylandManager *manager);

    Q_INVOKABLE bool startServer();

    void classBegin() override {}
    void componentComplete() override { initialize(); }

Q_SIGNALS:
    void enabledChanged();
    void compositorChanged();
    void managerChanged();
    void serverStarted();

private Q_SLOTS:
    void handleServerStarted();
    void handleSurfaceCreated(QWaylandSurface *surface);

private:
    QWaylandCompositor *m_compositor;
    bool m_enabled;
    bool m_initialized;
    XWaylandServer *m_server;
    XWaylandManager *m_manager;

    void initialize();
};

#endif // XWAYLAND_H
