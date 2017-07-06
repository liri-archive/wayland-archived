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

#ifndef LIRI_UDEVENUMERATE_H
#define LIRI_UDEVENUMERATE_H

#include <LiriUDev/UdevDevice>

namespace Liri {

namespace Platform {

class UdevEnumeratePrivate;

class LIRIUDEV_EXPORT UdevEnumerate
{
    Q_DECLARE_PRIVATE(UdevEnumerate)
public:
    UdevEnumerate(UdevDevice::DeviceTypes types, Udev *udev);
    ~UdevEnumerate();

    QList<UdevDevice *> scan() const;

private:
    UdevEnumeratePrivate *const d_ptr;
};

} // namespace Platform

} // namespace Liri

#endif // UDEVENUMERATE_H
