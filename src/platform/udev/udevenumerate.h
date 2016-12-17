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

#ifndef LIRI_UDEVENUMERATE_H
#define LIRI_UDEVENUMERATE_H

#include <Liri/Platform/UdevDevice>

namespace Liri {

namespace Platform {

class UdevEnumeratePrivate;

class LIRIPLATFORM_EXPORT UdevEnumerate
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
