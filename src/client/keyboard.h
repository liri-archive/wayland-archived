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

#ifndef LIRIWAYLANDCLIENT_KEYBOARD_H
#define LIRIWAYLANDCLIENT_KEYBOARD_H

#include <QtCore/QObject>

#include <Liri/waylandclient/liriwaylandclient_export.h>

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
