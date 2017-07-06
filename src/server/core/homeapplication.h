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

#ifndef LIRI_HOMEAPPLICATION_H
#define LIRI_HOMEAPPLICATION_H

#include <QtQml/QQmlApplicationEngine>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

namespace Liri {

namespace WaylandServer {

class HomeApplicationPrivate;

class LIRIWAYLANDSERVER_EXPORT HomeApplication : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HomeApplication)
    Q_DECLARE_PRIVATE(HomeApplication)
    Q_PROPERTY(QString screenConfiguration READ screenConfiguration WRITE setScreenConfiguration NOTIFY screenConfigurationChanged)
public:
    HomeApplication(QObject *parent = nullptr);

    QString screenConfiguration() const;
    void setScreenConfiguration(const QString &fileName);

    QList<QObject *> rootObjects() const;

    void setContextProperty(const QString &name, const QVariant &value);
    void setContextProperty(const QString &name, QObject *object);

    bool loadUrl(const QUrl &url);

Q_SIGNALS:
    void screenConfigurationChanged(const QString &fileName);
    void contextPropertyChanged(const QString &name, const QVariant &value);
    void contextPropertyChanged(const QString &name, QObject *object);
    void objectCreated(QObject *object, const QUrl &url);
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_HOMEAPPLICATION_H

