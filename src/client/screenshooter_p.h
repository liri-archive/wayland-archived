/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini
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

#ifndef LIRIWAYLANDCLIENT_SCREENSHOOTER_P_H
#define LIRIWAYLANDCLIENT_SCREENSHOOTER_P_H

#include <QtCore/QMutexLocker>
#include <QtCore/private/qobject_p.h>

#include <LiriWaylandClient/Screenshooter>
#include "qwayland-liri-screenshooter.h"

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

class LIRIWAYLANDCLIENT_EXPORT ScreenshooterPrivate
        : public QObjectPrivate
        , public QtWayland::liri_screenshooter
{
    Q_DECLARE_PUBLIC(Screenshooter)
public:
    ScreenshooterPrivate();
    ~ScreenshooterPrivate();

    QByteArray effectsArray(Screenshooter::Effects effects);
    void removeRequest(Screenshot *screenshot);

    Registry *registry;
    Shm *shm;
    ShmPool *shmPool;
    QMutex requestsMutex;
    QVector<Screenshot *> requests;

    static ScreenshooterPrivate *get(Screenshooter *screenshooter) { return screenshooter->d_func(); }
};

class LIRIWAYLANDCLIENT_EXPORT ScreenshotPrivate
        : public QObjectPrivate
        , public QtWayland::liri_screenshot
{
    Q_DECLARE_PUBLIC(Screenshot)
public:
    ScreenshotPrivate(Screenshooter *s);
    ~ScreenshotPrivate();

    Screenshot::CaptureType type;
    Screenshooter::Effects effects;
    Screenshooter *screenshooter;
    Output *output;
    BufferPtr buffer;

    static ScreenshotPrivate *get(Screenshot *screenshot) { return screenshot->d_func(); }

protected:
    void liri_screenshot_setup(int32_t width, int32_t height,
                               int32_t stride, int32_t format) override;
    void liri_screenshot_done() override;
    void liri_screenshot_failed(int32_t error) override;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_SCREENSHOOTER_P_H
