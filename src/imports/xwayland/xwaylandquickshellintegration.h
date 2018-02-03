/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini
 * Copyright (C) 2016 The Qt Company Ltd.
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:QTLGPL$
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General
 * Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPLv3 included in the
 * packaging of this file. Please review the following information to
 * ensure the GNU Lesser General Public License version 3 requirements
 * will be met: https://www.gnu.org/licenses/lgpl.html.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 2.0 or (at your option) the GNU General
 * Public license version 3 or any later version approved by the KDE Free
 * Qt Foundation. The licenses are as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPLv2 and LICENSE.GPLv3
 * included in the packaging of this file. Please review the following
 * information to ensure the GNU General Public License requirements will
 * be met: https://www.gnu.org/licenses/gpl-2.0.html and
 * https://www.gnu.org/licenses/gpl-3.0.html.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef XWAYLANDQUICKSHELLINTEGRATION_H
#define XWAYLANDQUICKSHELLINTEGRATION_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>

#include "xwaylandshellsurface.h"

class XWaylandQuickShellSurfaceItem;

class XWaylandQuickShellIntegration : public QObject
{
    Q_OBJECT
public:
    XWaylandQuickShellIntegration(XWaylandQuickShellSurfaceItem *item);

    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);

private Q_SLOTS:
    void handleStartMove();
    void handleStartResize(XWaylandShellSurface::ResizeEdge edges);
    void handleMapped();
    void handleActivatedChanged();

private:
    XWaylandQuickShellSurfaceItem *m_item;
    XWaylandShellSurface *m_shellSurface;

    enum class GrabberState {
        Default,
        Resize,
        Move
    };

    GrabberState grabberState;

    struct {
        QPointF initialOffset;
        bool initialized;
    } moveState;

    struct {
        XWaylandShellSurface::ResizeEdge resizeEdges;
        QSizeF initialSize;
        QPointF initialMousePos;
        bool initialized;
    } resizeState;
};

#endif // XWAYLANDQUICKSHELLINTEGRATION_H
