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

#ifndef LIRIWAYLANDCLIENT_OUTPUTCONFIGURATION_H
#define LIRIWAYLANDCLIENT_OUTPUTCONFIGURATION_H

#include <QtCore/QObject>

#include <LiriWaylandClient/Output>

namespace Liri {

namespace WaylandClient {

class OutputConfigurationPrivate;
class OutputManagement;

class LIRIWAYLANDCLIENT_EXPORT OutputConfiguration : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OutputConfiguration)
public:
    void setEnabled(Output *output, bool value);
    void setPrimary(Output *output, bool value);
    void setModeId(Output *output, int modeId);
    void setTransform(Output *output, Output::Transform transform);
    void setPosition(Output *output, const QPoint &position);
    void setScaleFactor(Output *output, qint32 scaleFactor);

    void apply();

private:
    explicit OutputConfiguration(QObject *parent = nullptr);

    friend class OutputManagement;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_OUTPUTCONFIGURATION_H
