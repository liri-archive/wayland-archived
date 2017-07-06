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

#ifndef LIRIWAYLANDCLIENT_CURSORTHEME_H
#define LIRIWAYLANDCLIENT_CURSORTHEME_H

#include <QtCore/QLoggingCategory>
#include <QtGui/QCursor>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

struct wl_cursor_image;

namespace Liri {

namespace WaylandClient {

class Compositor;
class CursorThemePrivate;
class Registry;
class Seat;
class ShmPool;

class LIRIWAYLANDCLIENT_EXPORT CursorTheme : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CursorTheme)
public:
    enum CursorShape {
        ArrowCursor = Qt::ArrowCursor,
        UpArrowCursor,
        CrossCursor,
        WaitCursor,
        IBeamCursor,
        SizeVerCursor,
        SizeHorCursor,
        SizeBDiagCursor,
        SizeFDiagCursor,
        SizeAllCursor,
        BlankCursor,
        SplitVCursor,
        SplitHCursor,
        PointingHandCursor,
        ForbiddenCursor,
        WhatsThisCursor,
        BusyCursor,
        OpenHandCursor,
        ClosedHandCursor,
        DragCopyCursor,
        DragMoveCursor,
        DragLinkCursor,
        ResizeNorthCursor = Qt::CustomCursor + 1,
        ResizeSouthCursor,
        ResizeEastCursor,
        ResizeWestCursor,
        ResizeNorthWestCursor,
        ResizeSouthEastCursor,
        ResizeNorthEastCursor,
        ResizeSouthWestCursor
    };
    Q_ENUM(CursorShape)

    CursorTheme(Registry *registry, Compositor *compositor, ShmPool *pool, Seat *seat);

    wl_cursor_image *cursorImage(CursorShape shape);

    void changeCursor(CursorShape shape);
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_CURSORTHEME_H
