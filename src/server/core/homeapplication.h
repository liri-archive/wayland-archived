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

