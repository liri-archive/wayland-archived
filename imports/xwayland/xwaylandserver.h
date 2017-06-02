/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef XWAYLANDSERVER_H
#define XWAYLANDSERVER_H

#include <QtCore/QString>
#include <QtCore/QProcess>

#include <QtWaylandCompositor/QWaylandCompositor>

struct wl_client;

class ServerProcess;

class XWaylandServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QWaylandCompositor *compositor READ compositor CONSTANT)
public:
    XWaylandServer(QWaylandCompositor *compositor, QObject *parent = nullptr);
    ~XWaylandServer();

    inline int wmFd() const {
        return m_wmPairFd[0];
    }

    inline wl_client *client() const {
        return m_client;
    }

    QWaylandCompositor *compositor() const;

    bool start();

Q_SIGNALS:
    void started();

private:
    QWaylandCompositor *m_compositor;

    int m_display;
    QString m_displayName;

    ServerProcess *m_process;
    int m_serverPairFd[2];
    int m_wmPairFd[2];

    wl_client *m_client;

    void shutdown();

private Q_SLOTS:
    void handleServerStarted();
};

#endif // XWAYLANDSERVER_H
