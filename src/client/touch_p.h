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
// This file is not part of the Green Island API.  It exists purely as an
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
