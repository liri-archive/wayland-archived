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

#include <QtMath>

#include "materialdecoration.h"

QT_BEGIN_NAMESPACE

inline void initResources() { Q_INIT_RESOURCE(icons); }

namespace QtWaylandClient {

#define BUTTON_WIDTH dp(24)
#define BUTTON_SPACING dp(12)
#define TITLE_BAR_HEIGHT dp(32)
#define TITLE_FONT_SIZE dp(16)
#define WINDOW_BORDER 4 // big enough to resize

QWaylandMaterialDecoration::QWaylandMaterialDecoration()
    : QWaylandAbstractDecoration()
    , m_clicking(None)
    , m_backgroundColor("#455a64")
    , m_iconColor("#b4ffffff")
    , m_textColor("#ffffff")
{
    initResources();

    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::NoWrap);
    m_windowTitle.setTextOption(option);
}

QColor QWaylandMaterialDecoration::backgroundColor() const
{
    return m_backgroundColor;
}

void QWaylandMaterialDecoration::setBackgroundColor(const QColor &color)
{
    if (color == m_backgroundColor)
        return;

    m_backgroundColor = color;
    update();
    Q_EMIT backgroundColorChanged();
}

QColor QWaylandMaterialDecoration::textColor() const
{
    return m_textColor;
}

void QWaylandMaterialDecoration::setTextColor(const QColor &color)
{
    if (color == m_textColor)
        return;

    m_textColor = color;
    update();
    Q_EMIT textColorChanged();
}

QColor QWaylandMaterialDecoration::iconColor() const
{
    return m_iconColor;
}

void QWaylandMaterialDecoration::setIconColor(const QColor &color)
{
    if (color == m_iconColor)
        return;

    m_iconColor = color;
    update();
    Q_EMIT iconColorChanged();
}

QRectF QWaylandMaterialDecoration::closeButtonRect() const
{
    return QRectF(window()->frameGeometry().width() - BUTTON_WIDTH - BUTTON_SPACING,
                  (margins().top() - BUTTON_WIDTH) / 2, BUTTON_WIDTH, BUTTON_WIDTH);
}

QRectF QWaylandMaterialDecoration::maximizeButtonRect() const
{
    return QRectF(window()->frameGeometry().width() - BUTTON_WIDTH * 2 - BUTTON_SPACING * 2,
                  (margins().top() - BUTTON_WIDTH) / 2, BUTTON_WIDTH, BUTTON_WIDTH);
}

QRectF QWaylandMaterialDecoration::minimizeButtonRect() const
{
    return QRectF(window()->frameGeometry().width() - BUTTON_WIDTH * 3 - BUTTON_SPACING * 3,
                  (margins().top() - BUTTON_WIDTH) / 2, BUTTON_WIDTH, BUTTON_WIDTH);
}

QMargins QWaylandMaterialDecoration::margins() const
{
    // Title bar is 32dp plus borders
    if (window() && window()->type() == Qt::Popup)
        return QMargins(0, 0, 0, 0);
    if (waylandWindow() && (waylandWindow()->isMaximized() || waylandWindow()->isFullscreen()))
        return QMargins(0, TITLE_BAR_HEIGHT, 0, 0);
    return QMargins(WINDOW_BORDER, TITLE_BAR_HEIGHT, WINDOW_BORDER, WINDOW_BORDER);
}

void QWaylandMaterialDecoration::paint(QPaintDevice *device)
{
    // Set decoration colors of Fluid applications
    if (window()) {
        QVariant bgColor = window()->property("__material_decoration_backgroundColor");
        if (bgColor.isValid()) {
            QColor color = bgColor.value<QColor>();
            if (color != m_backgroundColor) {
                m_backgroundColor = color;
                Q_EMIT backgroundColorChanged();
            }
        }

        QVariant fgColor = window()->property("__material_decoration_foregroundColor");
        if (fgColor.isValid()) {
            QColor color = fgColor.value<QColor>();
            if (color != m_textColor) {
                m_textColor = color;
                Q_EMIT textColorChanged();
                m_iconColor = color;
                Q_EMIT iconColorChanged();
            }
        }
    }

    const QRect frameGeometry = window()->frameGeometry();
    QRect top(QPoint(WINDOW_BORDER, WINDOW_BORDER), QSize(frameGeometry.width(), margins().top()));

    QPainter p(device);
    p.setRenderHint(QPainter::Antialiasing);

    // Title bar
    int radius = waylandWindow()->isMaximized() ? 0 : dp(3);
    QPainterPath roundedRect;
    roundedRect.addRoundedRect(margins().left(), margins().top() - TITLE_BAR_HEIGHT,
                               frameGeometry.width() - margins().left() - margins().right(), TITLE_BAR_HEIGHT + radius * 2,
                               radius, radius);
    p.fillPath(roundedRect.simplified(), m_backgroundColor);

    // Borders (transparent so the border is not noticeable)
    QPainterPath borderPath;
    borderPath.addRect(0, margins().top(), margins().left(), frameGeometry.height() - margins().top());
    borderPath.addRect(0, frameGeometry.height() - margins().bottom(), frameGeometry.width(), margins().bottom());
    borderPath.addRect(frameGeometry.width() - margins().right(), margins().top(), margins().right(), frameGeometry.height() - margins().bottom());
    p.fillPath(borderPath, Qt::transparent);

    // Window title
    QString windowTitleText = window()->title();
    if (!windowTitleText.isEmpty()) {
        if (m_windowTitle.text() != windowTitleText) {
            m_windowTitle.setText(windowTitleText);
            m_windowTitle.prepare();
        }

        QRect titleBar = top;
        titleBar.setLeft(margins().left() + BUTTON_SPACING);
        titleBar.setRight(qFloor(minimizeButtonRect().left()) - BUTTON_SPACING);

        p.save();
        p.setClipRect(titleBar);
        p.setPen(m_textColor);
        QSizeF size = m_windowTitle.size();
        int dx = qFloor((top.width() - size.width()) / 2);
        int dy = qFloor((top.height() - TITLE_FONT_SIZE) / 2);
        QFont font = p.font();
        font.setBold(true);
        font.setFamily("Roboto");
        font.setPixelSize(TITLE_FONT_SIZE);
        p.setFont(font);
        QPoint windowTitlePoint(dx, dy - WINDOW_BORDER / 2);
        p.drawStaticText(windowTitlePoint, m_windowTitle);
        p.restore();
    }

    p.save();
    p.setPen(m_iconColor);

    // Close button
    QBitmap closeIcon = buttonIcon("window-close");
    p.drawPixmap(closeButtonRect(), closeIcon, closeIcon.rect());

    // Maximize button
    if (isMaximizeable()) {
        QBitmap maximizeIcon =
                buttonIcon(waylandWindow()->isMaximized() ? "window-restore" : "window-maximize");
        p.drawPixmap(maximizeButtonRect(), maximizeIcon, maximizeIcon.rect());
    }

    // Minimize button
    if (window()->flags() & Qt::WindowMinimizeButtonHint) {
        QBitmap minimizeIcon = buttonIcon("window-minimize");
        p.drawPixmap(minimizeButtonRect(), minimizeIcon, minimizeIcon.rect());
    }

    p.restore();
}

bool QWaylandMaterialDecoration::clickButton(Qt::MouseButtons b, Button btn)
{
    if (isLeftClicked(b)) {
        m_clicking = btn;
        return false;
    } else if (isLeftReleased(b)) {
        if (m_clicking == btn) {
            m_clicking = None;
            return true;
        } else {
            m_clicking = None;
        }
    }
    return false;
}

bool QWaylandMaterialDecoration::handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local,
                                             const QPointF &global, Qt::MouseButtons b,
                                             Qt::KeyboardModifiers mods)

{
    Q_UNUSED(global);

    // Figure out what area mouse is in
    if (closeButtonRect().contains(local)) {
        if (clickButton(b, Close))
            QWindowSystemInterface::handleCloseEvent(window());
    } else if (isMaximizeable() && maximizeButtonRect().contains(local)) {
        if (clickButton(b, Maximize))
            window()->setWindowState(waylandWindow()->isMaximized() ? Qt::WindowNoState
                                                                    : Qt::WindowMaximized);
    } else if (minimizeButtonRect().contains(local)) {
        if (clickButton(b, Minimize))
            window()->setWindowState(Qt::WindowMinimized);
    } else if (local.y() <= margins().top()) {
        processMouseTop(inputDevice, local, b, mods);
    } else if (local.y() > window()->height() - margins().bottom() + margins().top()) {
        processMouseBottom(inputDevice, local, b, mods);
    } else if (local.x() <= margins().left()) {
        processMouseLeft(inputDevice, local, b, mods);
    } else if (local.x() > window()->width() - margins().right() + margins().left()) {
        processMouseRight(inputDevice, local, b, mods);
    } else {
        waylandWindow()->restoreMouseCursor(inputDevice);
        setMouseButtons(b);
        return false;
    }

    setMouseButtons(b);
    return true;
}

bool QWaylandMaterialDecoration::handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local,
                                             const QPointF &global, Qt::TouchPointState state,
                                             Qt::KeyboardModifiers mods)
{
    Q_UNUSED(inputDevice);
    Q_UNUSED(global);
    Q_UNUSED(mods);

    bool handled = state == Qt::TouchPointPressed;
    if (handled) {
        if (closeButtonRect().contains(local))
            QWindowSystemInterface::handleCloseEvent(window());
        else if (isMaximizeable() && maximizeButtonRect().contains(local))
            window()->setWindowState(waylandWindow()->isMaximized() ? Qt::WindowNoState
                                                                    : Qt::WindowMaximized);
        else if (minimizeButtonRect().contains(local))
            window()->setWindowState(Qt::WindowMinimized);
        else if (local.y() <= margins().top())
            waylandWindow()->shellSurface()->move(inputDevice);
        else
            handled = false;
    }

    return handled;
}

void QWaylandMaterialDecoration::processMouseTop(QWaylandInputDevice *inputDevice,
                                                 const QPointF &local, Qt::MouseButtons b,
                                                 Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);
    if (local.y() <= margins().bottom()) {
        if (local.x() <= margins().left()) {
            // top left bit
            waylandWindow()->setMouseCursor(inputDevice, Qt::SizeFDiagCursor);
            startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_TOP_LEFT, b);
        } else if (local.x() > window()->width() - margins().right()) {
            // top right bit
            waylandWindow()->setMouseCursor(inputDevice, Qt::SizeBDiagCursor);
            startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_TOP_RIGHT, b);
        } else {
            // top reszie bit
            waylandWindow()->setMouseCursor(inputDevice, Qt::SplitVCursor);
            startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_TOP, b);
        }
    } else {
        waylandWindow()->restoreMouseCursor(inputDevice);
        startMove(inputDevice, b);
    }
}

void QWaylandMaterialDecoration::processMouseBottom(QWaylandInputDevice *inputDevice,
                                                    const QPointF &local, Qt::MouseButtons b,
                                                    Qt::KeyboardModifiers mods)
{
    Q_UNUSED(mods);

    if (local.x() <= margins().left()) {
        // bottom left bit
        waylandWindow()->setMouseCursor(inputDevice, Qt::SizeBDiagCursor);
        startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_BOTTOM_LEFT, b);
    } else if (local.x() > window()->width() - margins().right()) {
        // bottom right bit
        waylandWindow()->setMouseCursor(inputDevice, Qt::SizeFDiagCursor);
        startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_BOTTOM_RIGHT, b);
    } else {
        // bottom bit
        waylandWindow()->setMouseCursor(inputDevice, Qt::SplitVCursor);
        startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_BOTTOM, b);
    }
}

void QWaylandMaterialDecoration::processMouseLeft(QWaylandInputDevice *inputDevice,
                                                  const QPointF &local, Qt::MouseButtons b,
                                                  Qt::KeyboardModifiers mods)
{
    Q_UNUSED(local);
    Q_UNUSED(mods);

    waylandWindow()->setMouseCursor(inputDevice, Qt::SplitHCursor);
    startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_LEFT, b);
}

void QWaylandMaterialDecoration::processMouseRight(QWaylandInputDevice *inputDevice,
                                                   const QPointF &local, Qt::MouseButtons b,
                                                   Qt::KeyboardModifiers mods)
{
    Q_UNUSED(local);
    Q_UNUSED(mods);
    waylandWindow()->setMouseCursor(inputDevice, Qt::SplitHCursor);
    startResize(inputDevice, WL_SHELL_SURFACE_RESIZE_RIGHT, b);
}

int QWaylandMaterialDecoration::dp(int dp) const
{
    return qRound(dp * pixelDensity());
}

QBitmap QWaylandMaterialDecoration::buttonIcon(const QString &name) const
{
    QIcon icon(":/icons/" + name + ".svg");
    QPixmap pixmap = icon.pixmap(QSize(BUTTON_WIDTH, BUTTON_WIDTH));
    return pixmap.createMaskFromColor(QColor("black"), Qt::MaskOutColor);
}

qreal QWaylandMaterialDecoration::pixelDensity() const
{
    QScreen *screen = window()->screen();
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    return screen->devicePixelRatio();
}

bool QWaylandMaterialDecoration::isMaximizeable() const
{
    return window()->flags() & Qt::WindowMaximizeButtonHint || (
                (window()->maximumSize().width() > window()->minimumSize().width()) &&
                (window()->maximumSize().height() > window()->minimumSize().height())
                );
}

}

QT_END_NAMESPACE
