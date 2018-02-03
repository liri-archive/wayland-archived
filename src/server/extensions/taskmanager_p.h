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

#ifndef LIRI_TASKMANAGER_P_H
#define LIRI_TASKMANAGER_P_H

#include <QtWaylandCompositor/private/qwaylandcompositorextension_p.h>

#include <LiriWaylandServer/ClientWindow>
#include <LiriWaylandServer/TaskManager>
#include <LiriWaylandServer/private/qwayland-server-liri.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Liri {

namespace WaylandServer {

class LIRIWAYLANDSERVER_EXPORT TaskManagerPrivate
        : public QWaylandCompositorExtensionPrivate
        , public QtWaylandServer::liri_windows
{
    Q_DECLARE_PUBLIC(TaskManager)
public:
    TaskManagerPrivate();
    ~TaskManagerPrivate();

    bool initialized;
    Resource *boundResource;
    QMap<ClientWindow *, TaskItem *> map;

    void windowMapped(ClientWindow *window);
    void windowUnmapped(ClientWindow *window);

    static TaskManagerPrivate *get(TaskManager *tm) { return tm->d_func(); }

protected:
    void windows_bind_resource(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT TaskItemPrivate
        : public QWaylandCompositorExtensionPrivate
        , public QtWaylandServer::liri_window
{
    Q_DECLARE_PUBLIC(TaskItem)
public:
    TaskItemPrivate(ClientWindow *w);

    ClientWindow *window;
    uint32_t windowType;
    uint32_t windowState;

    void determineType();
    void determineState();

    static TaskItemPrivate *get(TaskItem *t) { return t->d_func(); }
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_TASKMANAGER_P_H
