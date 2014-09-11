/****************************************************************************
 * This file is part of Green Island.
 *
 * Copyright (C) 2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef GREENISLAND_WINDOWVIEW_H
#define GREENISLAND_WINDOWVIEW_H

#include <QtCompositor/QWaylandSurfaceItem>

#include "output.h"

class WindowView : public QWaylandSurfaceItem
{
    Q_OBJECT
    Q_PROPERTY(Output *output READ output CONSTANT)
    Q_PROPERTY(Output *mainOutput READ mainOutput CONSTANT)
    Q_PROPERTY(QRectF globalGeometry READ globalGeometry WRITE setGlobalGeometry NOTIFY globalGeometryChanged)
public:
    explicit WindowView(QWaylandQuickSurface *surface, Output *output,
                        QQuickItem *parent = 0);

    Output *output() const;

    Output *mainOutput() const;

    QRectF globalGeometry() const;
    void setGlobalGeometry(const QRectF &g);

Q_SIGNALS:
    void globalGeometryChanged();

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Output *m_output;
    QRectF m_globalGeometry;

    void sendEnter(Output *output);
    void sendLeave(Output *output);
};

#endif // GREENISLAND_WINDOWVIEW_H
