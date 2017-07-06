/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef LIRI_FULLSCREENSHELL_P_H
#define LIRI_FULLSCREENSHELL_P_H

#include <QtCore/QLoggingCategory>
#include <QtCore/private/qobject_p.h>

#include <LiriWaylandClient/FullScreenShell>
#include "qwayland-fullscreen-shell-unstable-v1.h"

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

Q_DECLARE_LOGGING_CATEGORY(FSH_CLIENT_PROTOCOL)

namespace Liri {

namespace WaylandClient {

class LIRIWAYLANDCLIENT_EXPORT FullScreenShellPrivate
        : public QObjectPrivate
        , public QtWayland::zwp_fullscreen_shell_v1
{
    Q_DECLARE_PUBLIC(FullScreenShell)
public:
    FullScreenShellPrivate();

    FullScreenShell::Capabilities capabilities;

    static FullScreenShellPrivate *get(FullScreenShell *fsh) { return fsh->d_func(); }

protected:
    void zwp_fullscreen_shell_v1_capability(uint32_t capabilty) override;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRI_FULLSCREENSHELL_P_H
