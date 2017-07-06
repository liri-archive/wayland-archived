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

#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandSurface>

#include "gtkshell.h"
#include "gtkshell_p.h"
#include "logging_p.h"

namespace Liri {

namespace WaylandServer {

/*
 * GtkShellPrivate
 */

GtkShellPrivate::GtkShellPrivate()
    : QWaylandCompositorExtensionPrivate()
    , QtWaylandServer::gtk_shell()
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;
}

void GtkShellPrivate::gtk_shell_bind_resource(Resource *resource)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    send_capabilities(resource->handle, 0);
}

void GtkShellPrivate::gtk_shell_get_gtk_surface(Resource *resource, uint32_t id, wl_resource *surfaceResource)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_Q(GtkShell);

    QWaylandSurface *surface = QWaylandSurface::fromResource(surfaceResource);

    QWaylandResource gtkSurfaceResource(wl_resource_create(resource->client(), &gtk_surface_interface,
                                                           wl_resource_get_version(resource->handle), id));

    Q_EMIT q->gtkSurfaceRequested(surface, gtkSurfaceResource);

    GtkSurface *gtkSurface = GtkSurface::fromResource(gtkSurfaceResource.resource());
    if (!gtkSurface) {
        // A GtkSurface was not created in response to the createGtkSurface signal,
        // so we have to create a fallback one here
        gtkSurface = new GtkSurface(q, surface, gtkSurfaceResource);
    }

    Q_EMIT q->gtkSurfaceCreated(gtkSurface);
}

/*
 * GtkShell
 */

GtkShell::GtkShell()
    : QWaylandCompositorExtensionTemplate<GtkShell>(*new GtkShellPrivate())
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;
}

GtkShell::GtkShell(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<GtkShell>(compositor, *new GtkShellPrivate())
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;
}

void GtkShell::initialize()
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_D(GtkShell);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qWarning() << "Failed to find QWaylandCompositor when initializing GtkShell";
        return;
    }
    d->init(compositor->display(), QtWaylandServer::gtk_shell::interfaceVersion());
}

const struct wl_interface *GtkShell::interface()
{
    return GtkShellPrivate::interface();
}

QByteArray GtkShell::interfaceName()
{
    return GtkShellPrivate::interfaceName();
}

/*
 * GtkSurfacePrivate
 */

GtkSurfacePrivate::GtkSurfacePrivate()
    : QWaylandCompositorExtensionPrivate()
    , QtWaylandServer::gtk_surface()
    , m_shell(nullptr)
    , m_surface(nullptr)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;
}

GtkSurfacePrivate::~GtkSurfacePrivate()
{
}

void GtkSurfacePrivate::gtk_surface_destroy_resource(Resource *resource)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_UNUSED(resource);

    Q_Q(GtkSurface);
    delete q;
}

void GtkSurfacePrivate::gtk_surface_set_dbus_properties(Resource *resource,
                                                        const QString &application_id,
                                                        const QString &app_menu_path,
                                                        const QString &menubar_path,
                                                        const QString &window_object_path,
                                                        const QString &application_object_path,
                                                        const QString &unique_bus_name)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_UNUSED(resource);

    Q_Q(GtkSurface);

    qCDebug(gLcGtkShell)
            << "Application ID:" << application_id
            << "App menu path:" << app_menu_path
            << "Menubar path:" << menubar_path
            << "Window path:" << window_object_path
            << "Application object path:" << application_object_path
            << "Unique bus name:" << unique_bus_name;

    m_appId = application_id;
    m_appMenuPath = app_menu_path;
    m_windowObjectPath = window_object_path;
    m_appObjectPath = application_object_path;
    m_uniqueBusName = unique_bus_name;

    Q_EMIT q->appIdChanged(m_appId);
    Q_EMIT q->appMenuPathChanged(m_appMenuPath);
    Q_EMIT q->menuBarPathChanged(m_menuBarPath);
    Q_EMIT q->windowObjectPathChanged(m_windowObjectPath);
    Q_EMIT q->appObjectPathChanged(m_appObjectPath);
    Q_EMIT q->uniqueBusNameChanged(m_uniqueBusName);
}

void GtkSurfacePrivate::gtk_surface_set_modal(Resource *resource)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_UNUSED(resource);

    Q_Q(GtkSurface);
    Q_EMIT q->setModal();
}

void GtkSurfacePrivate::gtk_surface_unset_modal(Resource *resource)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_UNUSED(resource);

    Q_Q(GtkSurface);
    Q_EMIT q->unsetModal();
}

/*
 * GtkSurface
 */

GtkSurface::GtkSurface()
    : QWaylandShellSurfaceTemplate<GtkSurface>(*new GtkSurfacePrivate())
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;
}

GtkSurface::GtkSurface(GtkShell *shell, QWaylandSurface *surface,
                       const QWaylandResource &resource)
    : QWaylandShellSurfaceTemplate<GtkSurface>(*new GtkSurfacePrivate())
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    initialize(shell, surface, resource);
}

void GtkSurface::initialize(GtkShell *shell, QWaylandSurface *surface,
                            const QWaylandResource &resource)
{
    qCDebug(gLcGtkShellTrace) << Q_FUNC_INFO;

    Q_D(GtkSurface);
    d->m_shell = shell;
    d->m_surface = surface;
    d->init(resource.resource());
    setExtensionContainer(surface);
    Q_EMIT surfaceChanged();
    Q_EMIT shellChanged();
    QWaylandCompositorExtension::initialize();
}

QWaylandSurface *GtkSurface::surface() const
{
    Q_D(const GtkSurface);
    return d->m_surface;
}

GtkShell *GtkSurface::shell() const
{
    Q_D(const GtkSurface);
    return d->m_shell;
}

QString GtkSurface::appId() const
{
    Q_D(const GtkSurface);
    return d->m_appId;
}

QString GtkSurface::appMenuPath() const
{
    Q_D(const GtkSurface);
    return d->m_appMenuPath;
}

QString GtkSurface::menuBarPath() const
{
    Q_D(const GtkSurface);
    return d->m_menuBarPath;
}

QString GtkSurface::windowObjectPath() const
{
    Q_D(const GtkSurface);
    return d->m_windowObjectPath;
}

QString GtkSurface::appObjectPath() const
{
    Q_D(const GtkSurface);
    return d->m_appObjectPath;
}

QString GtkSurface::uniqueBusName() const
{
    Q_D(const GtkSurface);
    return d->m_uniqueBusName;
}

#ifdef QT_WAYLAND_COMPOSITOR_QUICK
QWaylandQuickShellIntegration *GtkSurface::createIntegration(QWaylandQuickShellSurfaceItem *item)
{
    Q_UNUSED(item);
    return nullptr;
}
#endif

const struct wl_interface *GtkSurface::interface()
{
    return GtkSurfacePrivate::interface();
}

QByteArray GtkSurface::interfaceName()
{
    return GtkSurfacePrivate::interfaceName();
}

GtkSurface *GtkSurface::fromResource(wl_resource *resource)
{
    GtkSurfacePrivate::Resource *res = GtkSurfacePrivate::Resource::fromResource(resource);
    if (res)
        return static_cast<GtkSurfacePrivate *>(res->gtk_surface_object)->q_func();
    return nullptr;
}

void GtkSurface::initialize()
{
    QWaylandCompositorExtension::initialize();
}

} // namespace WaylandServer

} // namespace Liri

#include "moc_gtkshell.cpp"
