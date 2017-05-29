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

#include <QtCore/QTimer>

#include <QtWaylandCompositor/QWaylandClient>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/private/qwaylandsurface_p.h>

#include "xwayland.h"
#include "xwaylandmanager.h"
#include "xwaylandshellsurface.h"
#include "xwaylandserver.h"

Q_LOGGING_CATEGORY(XWAYLAND, "liri.xwayland")
Q_LOGGING_CATEGORY(XWAYLAND_TRACE, "liri.xwayland.trace")

XWayland::XWayland(QObject *parent)
    : QObject(parent)
    , m_compositor(nullptr)
    , m_enabled(false)
    , m_initialized(false)
    , m_server(nullptr)
    , m_serverThread(new QThread())
    , m_manager(nullptr)
{
}

XWayland::~XWayland()
{
    delete m_server;

    m_serverThread->quit();
    m_serverThread->wait();
    delete m_serverThread;
}

QWaylandCompositor *XWayland::compositor() const
{
    return m_compositor;
}

void XWayland::setCompositor(QWaylandCompositor *compositor)
{
    if (m_compositor) {
        qCWarning(XWAYLAND, "Cannot move XWayland to another compositor");
        return;
    }

    m_compositor = compositor;
    Q_EMIT compositorChanged();
}

bool XWayland::isEnabled() const
{
    return m_enabled;
}

void XWayland::setEnabled(bool enabled)
{
    if (m_initialized) {
        qCWarning(XWAYLAND, "Cannot enable or disable XWayland after initialization");
        return;
    }

    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    Q_EMIT enabledChanged();
}

void XWayland::initialize()
{
    // Try to find a compositor among parents
    if (!m_compositor) {
        for (QObject *p = parent(); p != nullptr; p = p->parent()) {
            if (auto c = qobject_cast<QWaylandCompositor *>(p)) {
                setCompositor(c);
                break;
            }
        }
    }

    // Check whether we have a compositor assigned
    if (!m_compositor) {
        qCWarning(XWAYLAND, "Please assign a compositor to XWayland");
        return;
    }

    m_initialized = true;

    // Do not continue unless enabled
    if (!m_enabled) {
        qCWarning(XWAYLAND, "XWayland is disabled");
        return;
    }

    // Handle surface cration
    connect(m_compositor, &QWaylandCompositor::surfaceCreated,
            this, &XWayland::handleSurfaceCreated);

    // Instantiate the server
    m_server = new XWaylandServer(m_compositor, this);
    connect(m_server, &XWaylandServer::started,
            this, &XWayland::serverStarted);
    m_server->moveToThread(m_serverThread);

    // Window manager
    m_manager = new XWaylandManager(m_server, this);
    connect(m_manager, &XWaylandManager::shellSurfaceAdded,
            this, &XWayland::handleShellSurfaceAdded);
    connect(m_manager, &XWaylandManager::shellSurfaceRemoved,
            this, &XWayland::shellSurfaceClosed);

    // Setup server
    if (!m_server->setup())
        qCWarning(XWAYLAND) << "Failed to setup XWayland";
}

void XWayland::serverStarted()
{
    // Start window management
    m_manager->start(m_server->wmFd());
}

void XWayland::handleSurfaceCreated(QWaylandSurface *surface)
{
    // We are only interested in surfaces from Xwayland
    QWaylandClient *client = surface->client();
    if (client && client->client() != m_server->client())
        return;

    Q_FOREACH (XWaylandShellSurface *window, m_manager->m_unpairedWindows) {
        quint32 id = wl_resource_get_id(QWaylandSurfacePrivate::get(surface)->resource()->handle);

        qCDebug(XWAYLAND, "Unpaired surface %d vs %d", window->surfaceId(), id);
        if (window->surfaceId() == id) {
            window->setSurfaceId(0);
            window->setSurface(surface);
            m_manager->m_unpairedWindows.removeOne(window);
            break;
        }
    }
}

void XWayland::handleShellSurfaceAdded(XWaylandShellSurface *shellSurface)
{
    connect(shellSurface, &XWaylandShellSurface::surfaceChanged, this, [this, shellSurface] {
        if (shellSurface->surface())
            Q_EMIT shellSurfaceCreated(shellSurface);
    });
}

#include "moc_xwayland.cpp"
