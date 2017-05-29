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

#ifndef LIRI_OUTPUTMANAGEMENT_H
#define LIRI_OUTPUTMANAGEMENT_H

#include <QtCore/QObject>

#include <QtWaylandCompositor/QWaylandCompositorExtension>
#include <QtWaylandCompositor/QWaylandResource>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

namespace Liri {

namespace WaylandServer {

class OutputConfiguration;
class OutputManagementPrivate;

class LIRIWAYLANDSERVER_EXPORT OutputManagement : public QWaylandCompositorExtensionTemplate<OutputManagement>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OutputManagement)
public:
    OutputManagement();
    explicit OutputManagement(QWaylandCompositor *compositor);

    void initialize() override;

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void createOutputConfiguration(const QWaylandResource &resource);
    void outputConfigurationCreated(Liri::WaylandServer::OutputConfiguration *configuration);
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_OUTPUTMANAGEMENT_H
