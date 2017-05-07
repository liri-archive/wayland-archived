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
