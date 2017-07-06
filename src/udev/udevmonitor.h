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

#ifndef LIRI_UDEVMONITOR_H
#define LIRI_UDEVMONITOR_H

#include <QtCore/QObject>

#include <LiriUDev/liriudevglobal.h>

namespace Liri {

namespace Platform {

class Udev;
class UdevDevice;
class UdevMonitorPrivate;

class LIRIUDEV_EXPORT UdevMonitor : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UdevMonitor)
public:
    UdevMonitor(Udev *udev, QObject *parent = 0);

    bool isValid() const;

    void filterSubSystemDevType(const QString &subSystem, const QString &devType);
    void filterTag(const QString &tag);

Q_SIGNALS:
    void deviceAdded(UdevDevice *device);
    void deviceRemoved(UdevDevice *device);
    void deviceChanged(UdevDevice *device);
    void deviceOnlined(UdevDevice *device);
    void deviceOfflined(UdevDevice *device);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_udevEventHandler())
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_UDEVMONITOR_H
