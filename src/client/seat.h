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

#ifndef LIRIWAYLANDCLIENT_SEAT_H
#define LIRIWAYLANDCLIENT_SEAT_H

#include <LiriWaylandClient/Registry>

namespace Liri {

namespace WaylandClient {

class Keyboard;
class Pointer;
class SeatPrivate;
class Touch;

class LIRIWAYLANDCLIENT_EXPORT Seat : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Seat)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
public:
    QString name() const;
    quint32 version() const;

    Keyboard *keyboard() const;
    Pointer *pointer() const;
    Touch *touch() const;

    static QByteArray interfaceName();

Q_SIGNALS:
    void nameChanged();
    void keyboardAdded();
    void keyboardRemoved();
    void pointerAdded();
    void pointerRemoved();
    void touchAdded();
    void touchRemoved();

private:
    Seat(QObject *parent = nullptr);

    friend class Registry;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_SEAT_H
