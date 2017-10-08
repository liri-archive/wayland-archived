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

#ifndef LIRI_SCREENSHOOTER_H
#define LIRI_SCREENSHOOTER_H

#include <QQuickItem>
#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class QWaylandOutput;
class QWaylandSurface;

namespace Liri {

namespace WaylandServer {

class Screenshot;
class ScreenshotPrivate;
class ScreenshooterPrivate;

class LIRIWAYLANDSERVER_EXPORT Screenshooter : public QWaylandCompositorExtensionTemplate<Screenshooter>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Screenshooter)
public:
    enum Effect {
        EffectPointer = 1,
        EffectBorder
    };
    Q_ENUM(Effect)
    Q_DECLARE_FLAGS(Effects, Effect)

    Screenshooter();
    Screenshooter(QWaylandCompositor *compositor);

    void initialize() override;

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void captureRequested(Screenshot *screenshot);
};

class LIRIWAYLANDSERVER_EXPORT Screenshot : public QWaylandCompositorExtensionTemplate<Screenshot>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Screenshot)
    Q_PROPERTY(CaptureType captureType READ captureType CONSTANT)
    Q_PROPERTY(Screenshooter::Effects effects READ effects CONSTANT)
public:
    enum CaptureType {
        CaptureOutput = 1,
        CaptureActiveWindow,
        CaptureWindow,
        CaptureArea
    };
    Q_ENUM(CaptureType)

    enum Error {
        ErrorBadBuffer = 0
    };
    Q_ENUM(Error)

    CaptureType captureType() const;
    Screenshooter::Effects effects() const;

    Q_INVOKABLE void selectSurface(QWaylandSurface *surface);
    Q_INVOKABLE void selectArea(QQuickItem *area);
    Q_INVOKABLE void setup();

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void done();
    void failed(Error error);

private:
    explicit Screenshot(CaptureType type, Screenshooter::Effects effects);

    friend class ScreenshooterPrivate;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENSHOOTER_H
