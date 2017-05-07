/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini
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
