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

#ifndef LIRI_SCREENCASTER_H
#define LIRI_SCREENCASTER_H

#include <QtCore/QObject>
#include <QtCore/QMultiHash>
#include <QtCore/QMutex>

#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QQuickWindow);

namespace Liri {

namespace WaylandServer {

class Screencast;
class ScreencastPrivate;
class ScreencasterPrivate;

class LIRIWAYLANDSERVER_EXPORT Screencaster : public QWaylandCompositorExtensionTemplate<Screencaster>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Screencaster)
public:
    Screencaster();
    Screencaster(QWaylandCompositor *compositor);

    Q_INVOKABLE void addWindow(QQuickWindow *window);
    Q_INVOKABLE void removeWindow(QQuickWindow *window);

    void recordFrame(QQuickWindow *window);

    static const struct wl_interface* interface();
    static QByteArray interfaceName();

protected:
    void initialize() override;

Q_SIGNALS:
    void captureRequested(Screencast *screencast);

private Q_SLOTS:
    void readContent();
};

class LIRIWAYLANDSERVER_EXPORT Screencast : public QWaylandCompositorExtensionTemplate<Screencast>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Screencast)
public:
    ~Screencast();

    static const struct wl_interface* interface();
    static QByteArray interfaceName();

protected:
    bool event(QEvent *event) override;

private:
    explicit Screencast();

    friend class ScreencasterPrivate;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENCASTER_H
