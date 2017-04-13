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

#ifndef LIRI_UDEVMONITOR_H
#define LIRI_UDEVMONITOR_H

#include <QtCore/QObject>

#include <Liri/platform/liriplatform_export.h>

namespace Liri {

namespace Platform {

class Udev;
class UdevDevice;
class UdevMonitorPrivate;

class LIRIPLATFORM_EXPORT UdevMonitor : public QObject
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
