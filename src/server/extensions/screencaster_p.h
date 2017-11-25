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

#ifndef LIRI_SCREENCASTER_P_H
#define LIRI_SCREENCASTER_P_H

#include <QtQuick/QQuickWindow>

#include <QtWaylandCompositor/QWaylandOutput>

#include <LiriWaylandServer/Screencaster>
#include "qwayland-server-liri-screencaster.h"

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

class LIRIWAYLANDSERVER_EXPORT ScreencasterPrivate : public QtWaylandServer::liri_screencaster
{
    Q_DECLARE_PUBLIC(Screencaster)
public:
    ScreencasterPrivate(Screencaster *self);

    QMutex requestsMutex;
    QMultiHash<QQuickWindow *, Screencast *> requests;

    void addRequest(QQuickWindow *window, Screencast *screencast);
    void removeRequest(QQuickWindow *window, Screencast *screencast);

    static ScreencasterPrivate *get(Screencaster *screencaster) { return screencaster->d_func(); }

protected:
    Screencaster *q_ptr;

    void liri_screencaster_bind_resource(Resource *resource) override;
    void liri_screencaster_destroy_resource(Resource *resource) override;
    void liri_screencaster_capture(Resource *resource,
                                   uint32_t id,
                                   struct ::wl_resource *outputResource) override;
};

class LIRIWAYLANDSERVER_EXPORT ScreencastPrivate : public QtWaylandServer::liri_screencast
{
    Q_DECLARE_PUBLIC(Screencast)
public:
    ScreencastPrivate(Screencast *self);

    bool valid;
    Screencaster *screencaster;
    QWaylandOutput *output;
    QQuickWindow *window;
    wl_resource *bufferResource;
    wl_shm_buffer *buffer;

    void frameRecording(Screencast *screencast, uint time,
                        const QImage &image);

    static ScreencastPrivate *get(Screencast *screencast) { return screencast->d_func(); }

protected:
    Screencast *q_ptr;

    void liri_screencast_destroy(Resource *resource) override;
    void liri_screencast_record(Resource *resource,
                                struct ::wl_resource *bufferResource) override;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENCASTER_P_H
