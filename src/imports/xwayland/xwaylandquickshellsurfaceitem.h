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

#ifndef XWAYLANDQUICKSHELLSURFACEITEM_H
#define XWAYLANDQUICKSHELLSURFACEITEM_H

#include <QtWaylandCompositor/QWaylandQuickItem>

class XWaylandQuickShellIntegration;
class XWaylandShellSurface;

class XWaylandQuickShellSurfaceItem : public QWaylandQuickItem
{
    Q_OBJECT
    Q_PROPERTY(XWaylandShellSurface *shellSurface READ shellSurface WRITE setShellSurface NOTIFY shellSurfaceChanged)
    Q_PROPERTY(QQuickItem *moveItem READ moveItem WRITE setMoveItem NOTIFY moveItemChanged)
public:
    XWaylandQuickShellSurfaceItem(QQuickItem *parent = nullptr);
    ~XWaylandQuickShellSurfaceItem();

    XWaylandShellSurface *shellSurface() const;
    void setShellSurface(XWaylandShellSurface *shellSurface);

    QQuickItem *moveItem() const;
    void setMoveItem(QQuickItem *moveItem);

Q_SIGNALS:
    void shellSurfaceChanged();
    void moveItemChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    XWaylandShellSurface *m_shellSurface;
    XWaylandQuickShellIntegration *m_shellIntegration;
    QQuickItem *m_moveItem;
};

#endif // XWAYLANDQUICKSHELLSURFACEITEM_H
