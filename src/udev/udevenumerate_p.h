/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef UDEVENUMERATE_P_H
#define UDEVENUMERATE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "udevdevice.h"

extern "C" {
#include <libudev.h>
}

namespace Liri {

namespace Platform {

class UdevEnumeratePrivate
{
public:
    UdevEnumeratePrivate(UdevDevice::DeviceTypes t, Udev *u);
    ~UdevEnumeratePrivate();

    UdevDevice::DeviceTypes types;
    Udev *udev;
    udev_enumerate *enumerate;
};

} // namespace Platform

} // namespace Liri

#endif // UDEVENUMERATE_P_H
