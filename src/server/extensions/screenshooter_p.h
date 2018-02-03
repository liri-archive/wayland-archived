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

#ifndef LIRI_SCREENSHOOTER_P_H
#define LIRI_SCREENSHOOTER_P_H

#include <QtQuick/QQuickItem>

#include <QtWaylandCompositor/QWaylandOutput>
#include <QtWaylandCompositor/QWaylandSurface>

#include <LiriWaylandServer/Screenshooter>
#include "qwayland-server-liri-screenshooter.h"

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

class LIRIWAYLANDSERVER_EXPORT ScreenshooterPrivate : public QtWaylandServer::liri_screenshooter
{
    Q_DECLARE_PUBLIC(Screenshooter)
public:
    ScreenshooterPrivate(Screenshooter *self);

    static ScreenshooterPrivate *get(Screenshooter *screenshooter) { return screenshooter->d_func(); }

protected:
    Screenshooter *q_ptr;

    void liri_screenshooter_capture_output(Resource *resource,
                                           uint32_t id,
                                           struct ::wl_resource *outputResource,
                                           wl_array *effects) override;
    void liri_screenshooter_capture_active(Resource *resource,
                                           uint32_t id,
                                           wl_array *effects) override;
    void liri_screenshooter_capture_surface(Resource *resource,
                                            uint32_t id,
                                            wl_array *effects) override;
    void liri_screenshooter_capture_area(Resource *resource,
                                         uint32_t id,
                                         wl_array *effects) override;
};

class LIRIWAYLANDSERVER_EXPORT ScreenshotPrivate : public QtWaylandServer::liri_screenshot
{
    Q_DECLARE_PUBLIC(Screenshot)
public:
    ScreenshotPrivate(Screenshot *self);

    QImage grabItem(QQuickItem *item);

    Screenshot::CaptureType captureType;
    Screenshooter::Effects effects;
    QWaylandOutput *output;
    QWaylandSurface *selectedSurface;
    QQuickItem *selectedArea;

    static ScreenshotPrivate *get(Screenshot *screenshot) { return screenshot->d_func(); }

protected:
    Screenshot *q_ptr;

    void liri_screenshot_destroy(Resource *resource) override;
    void liri_screenshot_record(Resource *resource,
                                struct ::wl_resource *bufferResource) override;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENSHOOTER_P_H
