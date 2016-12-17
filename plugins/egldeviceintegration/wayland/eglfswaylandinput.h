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

#ifndef EGLFSWAYLANDINPUT_H
#define EGLFSWAYLANDINPUT_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGui/qpa/qwindowsysteminterface.h>

#include <Liri/WaylandClient/Touch>

#include <xkbcommon/xkbcommon.h>

namespace Liri {

namespace Platform {

class EglFSWaylandInput : public QObject
{
    Q_OBJECT
public:
    EglFSWaylandInput(WaylandClient::Seat *seat, QObject *parent = Q_NULLPTR);

    void setTouchDevice(QTouchDevice *td);

public Q_SLOTS:
    void keymapChanged(int fd, quint32 size);
    void keyPressed(quint32 time, quint32 key);
    void keyReleased(quint32 time, quint32 key);
    void keyboardModifiersChanged(quint32 depressed, quint32 latched, quint32 locked, quint32 group);
    void pointerEnter(quint32 serial, const QPointF &pos);
    void pointerLeave(quint32 serial);
    void pointerMotion(quint32 time, const QPointF &pos);
    void pointerButtonPressed(quint32 serial, quint32 time, const Qt::MouseButton &button);
    void pointerButtonReleased(quint32 serial, quint32 time, const Qt::MouseButton &button);
    void pointerAxisChanged(quint32 time, const Qt::Orientation &orientation, qreal value);
    void touchSequenceStarted(WaylandClient::TouchPoint *p);
    void touchSequenceFinished();
    void touchSequenceCanceled();
    void touchPointAdded(WaylandClient::TouchPoint *p);
    void touchPointRemoved(WaylandClient::TouchPoint *p);
    void touchPointMoved(WaylandClient::TouchPoint *p);

private:
    WaylandClient::Seat *m_seat;
    Qt::KeyboardModifiers m_modifiers;
    quint32 m_nativeModifiers;
    Qt::MouseButtons m_mouseButtons;
    QPointF m_mouseLocalPos, m_mouseGlobalPos;
    QTouchDevice *m_touchDevice;
    QList<QWindowSystemInterface::TouchPoint> m_touchPoints;

    xkb_context *m_xkbContext;
    xkb_keymap *m_xkbKeymap;
    xkb_state *m_xkbState;

    int m_repeatKey;
    quint32 m_repeatCode, m_repeatTime;
    QString m_repeatText;
    xkb_keysym_t m_repeatSym;
    QTimer m_repeatTimer;

    bool createDefaultKeymap();
    void releaseKeymap();
    void processKeyEvent(QEvent::Type type, quint32 time, quint32 key);

private Q_SLOTS:
    void repeatKey();
};

} // namespace Platform

} // namespace Liri

#endif // EGLFSWAYLANDINPUT_H
