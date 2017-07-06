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

#ifndef LIRIWAYLANDCLIENT_SCREENCASTER_P_H
#define LIRIWAYLANDCLIENT_SCREENCASTER_P_H

#include <QtCore/QMutexLocker>
#include <QtCore/QThread>
#include <QtCore/private/qobject_p.h>

#include <LiriWaylandClient/Screencaster>
#include "qwayland-liri-screencaster.h"

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

namespace WaylandClient {

class ScreencastBuffer;
class ScreencastHandler;

class LIRIWAYLANDCLIENT_EXPORT ScreencasterPrivate
        : public QObjectPrivate
        , public QtWayland::liri_screencaster
{
    Q_DECLARE_PUBLIC(Screencaster)
public:
    ScreencasterPrivate();
    ~ScreencasterPrivate();

    Registry *registry;
    Shm *shm;
    QVector<Screencast *> requests;

    static ScreencasterPrivate *get(Screencaster *screencaster) { return screencaster->d_func(); }
};

class LIRIWAYLANDCLIENT_EXPORT ScreencastPrivate
        : public QObjectPrivate
        , public QtWayland::liri_screencast
{
    Q_DECLARE_PUBLIC(Screencast)
public:
    ScreencastPrivate(Screencaster *s);
    ~ScreencastPrivate();

    ShmPool *shmPool;
    Screencaster *screencaster;
    QMutex recordMutex;
    QVector<ScreencastBuffer *> buffers;
    bool starving;

    QThread *thread;
    ScreencastHandler *handler;

    void recordFrame();

    static ScreencastPrivate *get(Screencast *screencast) { return screencast->d_func(); }

protected:
    void liri_screencast_setup(int32_t width, int32_t height,
                               int32_t stride, int32_t format) override;
    void liri_screencast_frame(struct ::wl_buffer *buffer,
                               uint32_t time, int32_t transform) override;
    void liri_screencast_failed(int32_t error,
                                struct ::wl_buffer *buffer) override;
    void liri_screencast_cancelled(struct ::wl_buffer *buffer) override;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_SCREENCASTER_P_H
