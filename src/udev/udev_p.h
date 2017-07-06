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

#ifndef LIRI_UDEV_P_H
#define LIRI_UDEV_P_H

#include <LiriUDev/liriudevglobal.h>

extern "C" {
#include <libudev.h>
}

namespace Liri {

namespace Platform {

class Udev;

class LIRIUDEV_EXPORT UdevPrivate
{
public:
    UdevPrivate();
    ~UdevPrivate();

    static UdevPrivate *get(Udev *u);

    struct udev *udev;
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_UDEV_P_H

