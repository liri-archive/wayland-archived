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

#ifndef LIRIWAYLANDCLIENT_TOUCH_H
#define LIRIWAYLANDCLIENT_TOUCH_H

#include <QtCore/QObject>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

namespace Liri {

namespace WaylandClient {

class Seat;
class Surface;
class TouchPrivate;

class TouchPoint;
class TouchPointPrivate;

class LIRIWAYLANDCLIENT_EXPORT Touch : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Touch)
public:
    Touch(Seat *seat);

    static QByteArray interfaceName();

Q_SIGNALS:
    void sequenceStarted(TouchPoint *firstPoint);
    void sequenceCanceled();
    void sequenceFinished();
    void frameFinished();
    void pointAdded(TouchPoint *point);
    void pointRemoved(TouchPoint *point);
    void pointMoved(TouchPoint *point);
};

class LIRIWAYLANDCLIENT_EXPORT TouchPoint
{
    Q_DECLARE_PRIVATE(TouchPoint)
public:
    TouchPoint();
    ~TouchPoint();

    qint32 id() const;
    quint32 upSerial() const;
    quint32 downSerial() const;
    quint32 timestamp() const;
    QVector<quint32> timestamps() const;
    QPointF position() const;
    QVector<QPointF> positions() const;
    QPointer<Surface> surface() const;
    bool isDown() const;

private:
    TouchPointPrivate *const d_ptr;

    friend class TouchPrivate;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_TOUCH_H
