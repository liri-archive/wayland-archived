/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Robin Burchell <robin.burchell@viroteck.net>
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (C) 2017 Michael Spencer <sonrisesoftware@gmail.com>
 * Copyright (C) 2016 The Qt Company Ltd.
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

#include <QBitmap>
#include <QtGui/QCursor>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>
#include <QtGui/QPalette>

#include <QtGui/qpa/qwindowsysteminterface.h>

#include <QtWaylandClient/private/qwaylandabstractdecoration_p.h>
#include <QtWaylandClient/private/qwaylanddecorationplugin_p.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>

QT_BEGIN_NAMESPACE

namespace QtWaylandClient
{

enum Button
{
    None,
    Close,
    Maximize,
    Minimize
};

class Q_WAYLAND_CLIENT_EXPORT QWaylandMaterialDecoration : public QWaylandAbstractDecoration
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged)
public:
    QWaylandMaterialDecoration();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    QColor iconColor() const;
    void setIconColor(const QColor &color);

Q_SIGNALS:
    void backgroundColorChanged();
    void textColorChanged();
    void iconColorChanged();

protected:
    QMargins margins() const override;
    void paint(QPaintDevice *device) override;
    bool handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::MouseButtons b, Qt::KeyboardModifiers mods) override;
    bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                     Qt::TouchPointState state, Qt::KeyboardModifiers mods) override;

private:
    void processMouseTop(QWaylandInputDevice *inputDevice, const QPointF &local, Qt::MouseButtons b,
                         Qt::KeyboardModifiers mods);
    void processMouseBottom(QWaylandInputDevice *inputDevice, const QPointF &local,
                            Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseLeft(QWaylandInputDevice *inputDevice, const QPointF &local,
                          Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    void processMouseRight(QWaylandInputDevice *inputDevice, const QPointF &local,
                           Qt::MouseButtons b, Qt::KeyboardModifiers mods);
    bool clickButton(Qt::MouseButtons b, Button btn);

    QRectF closeButtonRect() const;
    QRectF maximizeButtonRect() const;
    QRectF minimizeButtonRect() const;

    int dp(int dp) const;
    qreal pixelDensity() const;

    QBitmap buttonIcon(const QString &name) const;

    bool isMaximizeable() const;

    Button m_clicking;
    QColor m_backgroundColor;
    QColor m_iconColor;
    QColor m_textColor;
    QStaticText m_windowTitle;
};
}
