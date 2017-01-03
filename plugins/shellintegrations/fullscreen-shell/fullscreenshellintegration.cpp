/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "fullscreenshellintegration.h"
#include "fullscreenshellsurface.h"

namespace QtWaylandClient {

FullScreenShellIntegration::FullScreenShellIntegration()
    : QWaylandShellIntegration()
    , m_shell(nullptr)
{
}

bool FullScreenShellIntegration::initialize(QWaylandDisplay *display)
{
    QWaylandShellIntegration::initialize(display);

    for (const QWaylandDisplay::RegistryGlobal &global : display->globals()) {
        if (global.interface == QLatin1String("zwp_fullscreen_shell_v1")) {
            m_shell = new QtWayland::zwp_fullscreen_shell_v1(display->wl_registry(), global.id, 1);
            break;
        }
    }

    return m_shell != nullptr;
}

QWaylandShellSurface *FullScreenShellIntegration::createShellSurface(QWaylandWindow *window)
{
    return new FullScreenShellSurface(m_shell, window);
}

} // namespace QtWaylandClient
