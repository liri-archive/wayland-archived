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

#ifndef LIRIWAYLANDCLIENT_CURSORTHEME_P_H
#define LIRIWAYLANDCLIENT_CURSORTHEME_P_H

#include <QtCore/private/qobject_p.h>

#include <LiriWaylandClient/CursorTheme>
#include <LiriWaylandClient/Surface>

#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-server.h>

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

Q_DECLARE_LOGGING_CATEGORY(WLCURSORTHEME)

namespace Liri {

namespace WaylandClient {

class LIRIWAYLANDCLIENT_EXPORT CursorThemePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(CursorTheme)
public:
    CursorThemePrivate();
    ~CursorThemePrivate();

    void fillCursorShapes();

    void loadTheme();
    void destroyTheme();

    wl_cursor *requestCursor(CursorTheme::CursorShape shape);

    Registry *registry;
    Compositor *compositor;
    ShmPool *pool;
    Seat *seat;
    QString cursorThemeName;
    int cursorSize;
    Surface *cursorSurface;
    wl_cursor_theme *theme;
    QMultiMap<CursorTheme::CursorShape, QByteArray> cursorShapes;
    QMap<CursorTheme::CursorShape, wl_cursor *> cursors;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_CURSORTHEME_P_H
