/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef LIRI_TASKMANAGER_H
#define LIRI_TASKMANAGER_H

#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

namespace Liri {

namespace WaylandServer {

class TaskItem;
class TaskItemPrivate;
class TaskManagerPrivate;

class LIRIWAYLANDSERVER_EXPORT TaskManager : public QWaylandCompositorExtensionTemplate<TaskManager>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TaskManager)
public:
    TaskManager();
    TaskManager(QWaylandCompositor *compositor);

    void initialize() override;

    static const struct wl_interface *interface();
    static QByteArray interfaceName();
};

class LIRIWAYLANDSERVER_EXPORT TaskItem : public QWaylandCompositorExtensionTemplate<TaskItem>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TaskItem)
public:
    static const struct wl_interface *interface();
    static QByteArray interfaceName();

private:
    TaskItem();

    friend class TaskManagerPrivate;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_TASKMANAGER_H
