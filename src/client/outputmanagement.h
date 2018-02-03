/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini
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

#ifndef LIRIWAYLANDCLIENT_OUTPUTMANAGEMENT_H
#define LIRIWAYLANDCLIENT_OUTPUTMANAGEMENT_H

#include <QtCore/QObject>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

namespace Liri {

namespace WaylandClient {

class OutputConfiguration;
class OutputManagementPrivate;
class Registry;

class LIRIWAYLANDCLIENT_EXPORT OutputManagement : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OutputManagement)
public:
    OutputConfiguration *createConfiguration(QObject *parent = nullptr);

    static QByteArray interfaceName();

private:
    explicit OutputManagement(QObject *parent = nullptr);

    friend class Registry;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_OUTPUTMANAGEMENT_H
