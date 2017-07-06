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

#ifndef LIRI_QUICKOUTPUTCONFIGURATION_H
#define LIRI_QUICKOUTPUTCONFIGURATION_H

#include <QtQml/QQmlListProperty>

#include <LiriWaylandServer/OutputConfiguration>

namespace Liri {

namespace WaylandServer {

class QuickOutputConfigurationPrivate;

class LIRIWAYLANDSERVER_EXPORT QuickOutputConfiguration : public OutputConfiguration
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QuickOutputConfiguration)
    Q_PRIVATE_PROPERTY(QuickOutputConfiguration::d_func(), QQmlListProperty<Liri::WaylandServer::OutputChangeset> changes READ changes NOTIFY changesChanged)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    QuickOutputConfiguration();

    QQmlListProperty<QObject> data();

Q_SIGNALS:
    void changesChanged();
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_QUICKOUTPUTCONFIGURATION_H
