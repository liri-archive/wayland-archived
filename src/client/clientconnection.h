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

#ifndef LIRIWAYLANDCLIENT_CLIENTCONNECTION_H
#define LIRIWAYLANDCLIENT_CLIENTCONNECTION_H

#include <QtCore/QObject>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

struct wl_display;

namespace Liri {

namespace WaylandClient {

class ClientConnectionPrivate;

class LIRIWAYLANDCLIENT_EXPORT ClientConnection : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ClientConnection)
public:
    ClientConnection(QObject *parent = nullptr);

    bool isConnected() const;

    wl_display *display() const;
    void setDisplay(wl_display *display);

    int socketFd() const;
    void setSocketFd(int fd);

    QString socketName() const;
    void setSocketName(const QString &socketName);

    void initializeConnection();
    void synchronousConnection();

    static ClientConnection *fromQt(QObject *parent = nullptr);

public Q_SLOTS:
    void forceRoundTrip();
    void flush();
    void flushRequests();
    void blockingReadEvents();

Q_SIGNALS:
    void connected();
    void failed();
    void eventsDispatched();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_initConnection())
};

} // namespace WaylandClient

} // namespace GreenIslanmd

#endif // LIRIWAYLANDCLIENT_CLIENTCONNECTION_H
