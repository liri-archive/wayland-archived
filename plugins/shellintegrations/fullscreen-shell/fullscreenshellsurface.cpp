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
