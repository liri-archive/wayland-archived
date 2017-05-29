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

#include "outputconfiguration.h"
#include "outputconfiguration_p.h"
#include "outputmanagement.h"
#include "outputmanagement_p.h"

namespace Liri {

namespace WaylandClient {

/*
 * OutputManagementPrivate
 */

OutputManagementPrivate::OutputManagementPrivate()
    : QtWayland::liri_outputmanagement()
    , registry(nullptr)
{
}

/*
 * OutputManagement
 */

OutputManagement::OutputManagement(QObject *parent)
    : QObject(*new OutputManagementPrivate(), parent)
{
}

OutputConfiguration *OutputManagement::createConfiguration(QObject *parent)
{
    Q_D(OutputManagement);

    OutputConfiguration *config = new OutputConfiguration(parent);
    OutputConfigurationPrivate::get(config)->init(d->create_configuration());
    return config;
}

QByteArray OutputManagement::interfaceName()
{
    return QByteArrayLiteral("liri_outputmanagement");
}

} // namespace WaylandClient

} // namespace Liri

#include "moc_outputmanagement.cpp"
