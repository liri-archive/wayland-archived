/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
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

#ifndef LIRI_TASKMANAGER_H
#define LIRI_TASKMANAGER_H

#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <Liri/waylandserver/liriwaylandserver_export.h>

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

    void initialize() Q_DECL_OVERRIDE;

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
