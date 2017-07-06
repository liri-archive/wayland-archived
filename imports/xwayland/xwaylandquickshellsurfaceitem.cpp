/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini
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

#include "xwaylandquickshellintegration.h"
#include "xwaylandquickshellsurfaceitem.h"
#include "xwaylandshellsurface.h"

XWaylandQuickShellSurfaceItem::XWaylandQuickShellSurfaceItem(QQuickItem *parent)
    : QWaylandQuickItem(parent)
    , m_shellSurface(nullptr)
    , m_shellIntegration(nullptr)
    , m_moveItem(nullptr)
{
}

XWaylandQuickShellSurfaceItem::~XWaylandQuickShellSurfaceItem()
{
    delete m_shellIntegration;
}

XWaylandShellSurface *XWaylandQuickShellSurfaceItem::shellSurface() const
{
    return m_shellSurface;
}

void XWaylandQuickShellSurfaceItem::setShellSurface(XWaylandShellSurface *shellSurface)
{
    if (m_shellSurface == shellSurface)
        return;

    m_shellSurface = shellSurface;

    if (m_shellIntegration) {
        delete m_shellIntegration;
        m_shellIntegration = nullptr;
    }

    if (m_shellSurface)
        m_shellIntegration = shellSurface->createIntegration(this);

    Q_EMIT shellSurfaceChanged();
}

QQuickItem *XWaylandQuickShellSurfaceItem::moveItem() const
{
    return m_moveItem ? m_moveItem : const_cast<XWaylandQuickShellSurfaceItem *>(this);
}

void XWaylandQuickShellSurfaceItem::setMoveItem(QQuickItem *moveItem)
{
    moveItem = moveItem ? moveItem : this;

    if (this->moveItem() == moveItem)
        return;

    m_moveItem = moveItem;
    Q_EMIT moveItemChanged();
}

void XWaylandQuickShellSurfaceItem::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_shellIntegration->mouseMoveEvent(event))
        QWaylandQuickItem::mouseMoveEvent(event);
}

void XWaylandQuickShellSurfaceItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_shellIntegration->mouseReleaseEvent(event))
        QWaylandQuickItem::mouseReleaseEvent(event);
}

#include "moc_xwaylandquickshellsurfaceitem.cpp"
