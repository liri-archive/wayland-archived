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
// This file is not part of the Green Island API.  It exists purely as an
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
    void windows_bind_resource(Resource *resource) Q_DECL_OVERRIDE;
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
