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
