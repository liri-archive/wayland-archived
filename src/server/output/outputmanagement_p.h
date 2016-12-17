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

#ifndef LIRI_OUTPUTMANAGEMENT_P_H
#define LIRI_OUTPUTMANAGEMENT_P_H

#include <QtWaylandCompositor/private/qwaylandcompositorextension_p.h>

#include <Liri/WaylandServer/OutputManagement>
#include <Liri/waylandserver/private/qwayland-server-liri-outputmanagement.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Green Island API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Liri {

namespace WaylandServer {

class LIRIWAYLANDSERVER_EXPORT OutputManagementPrivate
        : public QWaylandCompositorExtensionPrivate
        , public QtWaylandServer::liri_outputmanagement
{
    Q_DECLARE_PUBLIC(OutputManagement)
public:
    OutputManagementPrivate();

    void removeConfiguration(OutputConfiguration *configuration);

    QVector<OutputConfiguration *> configurations;

    static OutputManagementPrivate *get(OutputManagement *management) { return management->d_func(); }

protected:
    virtual void outputmanagement_create_configuration(Resource *resource, uint32_t id) Q_DECL_OVERRIDE;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_OUTPUTMANAGEMENT_P_H
