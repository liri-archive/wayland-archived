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

#ifndef LIRIWAYLANDCLIENT_KEYBOARD_H
#define LIRIWAYLANDCLIENT_KEYBOARD_H

#include <QtCore/QObject>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

namespace Liri {

namespace WaylandClient {

class KeyboardPrivate;
class Seat;
class Surface;

class LIRIWAYLANDCLIENT_EXPORT Keyboard : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Keyboard)
    Q_PROPERTY(quint32 repeatRate READ repeatRate NOTIFY repeatRateChanged)
    Q_PROPERTY(quint32 repeatDelay READ repeatDelay NOTIFY repeatDelayChanged)
public:
    Keyboard(Seat *seat);

    Surface *focusSurface() const;

    quint32 repeatRate() const;
    quint32 repeatDelay() const;

    static QByteArray interfaceName();

Q_SIGNALS:
    void keymapChanged(int fd, quint32 size);
    void enter(quint32 serial);
    void leave(quint32 serial);
    void keyPressed(quint32 time, quint32 key);
    void keyReleased(quint32 time, quint32 key);
    void modifiersChanged(quint32 depressed, quint32 latched, quint32 locked, quint32 group);
    void repeatRateChanged();
    void repeatDelayChanged();
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_KEYBOARD_H
