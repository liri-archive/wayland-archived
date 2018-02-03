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

#ifndef LIRIWAYLANDCLIENT_TOUCH_P_H
#define LIRIWAYLANDCLIENT_TOUCH_P_H

#include <QtCore/private/qobject_p.h>

#include <LiriWaylandClient/Seat>
#include <LiriWaylandClient/Surface>
#include <LiriWaylandClient/Touch>
#include "qwayland-wayland.h"

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

namespace WaylandClient {

class LIRIWAYLANDCLIENT_EXPORT TouchPrivate
        : public QObjectPrivate
        , public QtWayland::wl_touch
{
    Q_DECLARE_PUBLIC(Touch)
public:
    TouchPrivate();
    ~TouchPrivate();

    Seat *seat;
    quint32 seatVersion;
    bool active;
    QVector<TouchPoint *> points;

    TouchPoint *getPressedPoint(qint32 id) const;

    static Touch *fromWlTouch(struct ::wl_touch *touch);
    static TouchPrivate *get(Touch *touch) { return touch->d_func(); }

protected:
    void touch_down(uint32_t serial, uint32_t time,
                    struct ::wl_surface *surface, int32_t id,
                    wl_fixed_t x, wl_fixed_t y) override;
    void touch_up(uint32_t serial, uint32_t time, int32_t id) override;
    void touch_motion(uint32_t time, int32_t id,
                      wl_fixed_t x, wl_fixed_t y) override;
    void touch_frame() override;
    void touch_cancel() override;
};

class LIRIWAYLANDCLIENT_EXPORT TouchPointPrivate
{
    Q_DECLARE_PUBLIC(TouchPoint)
public:
    TouchPointPrivate();

    qint32 id;
    quint32 upSerial, downSerial;
    QPointer<Surface> surface;
    QVector<QPointF> positions;
    QVector<quint32> timestamps;
    bool down;

private:
    TouchPoint *q_ptr;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_TOUCH_P_H
