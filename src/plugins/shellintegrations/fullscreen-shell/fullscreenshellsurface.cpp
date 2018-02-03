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

#include <QtWaylandClient/private/qwaylandscreen_p.h>

#include "fullscreenshellsurface.h"

namespace QtWaylandClient {

FullScreenShellSurface::FullScreenShellSurface(QtWayland::zwp_fullscreen_shell_v1 *shell, QWaylandWindow *window)
    : QWaylandShellSurface(window)
    , m_shell(shell)
    , m_window(window)
{
}

void FullScreenShellSurface::setType(Qt::WindowType type, QWaylandWindow *transientParent)
{
    Q_UNUSED(type);
    Q_UNUSED(transientParent);

    // setType is called after the wl_surface is initialized by QWaylandWindow,
    // we should be safe calling present_surface() here
    m_shell->present_surface(m_window->object(),
                             QtWayland::zwp_fullscreen_shell_v1::present_method_default,
                             m_window->screen()->output());
}

} // namespace QtWaylandClient
