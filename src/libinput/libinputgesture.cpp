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

#include <QtCore/QPointF>
#include <QtGui/qpa/qwindowsysteminterface.h>

#include "libinputhandler.h"
#include "libinputgesture.h"

#include <libinput.h>

namespace Liri {

namespace Platform {

LibInputGesture::LibInputGesture(LibInputHandler *handler)
{
    Q_UNUSED(handler);
}

void LibInputGesture::handlePinchBegin(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

    QWindowSystemInterface::handleGestureEvent(
                Q_NULLPTR, timestamp, Qt::BeginNativeGesture,
                pos, pos);
}

void LibInputGesture::handlePinchEnd(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

    QWindowSystemInterface::handleGestureEvent(
                Q_NULLPTR, timestamp, Qt::EndNativeGesture,
                pos, pos);
}

void LibInputGesture::handlePinchUpdate(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    const double scale = libinput_event_gesture_get_scale(event);
    const double angle = libinput_event_gesture_get_angle_delta(event);
    QPointF pos(libinput_event_gesture_get_dx(event),
                libinput_event_gesture_get_dy(event));

    QWindowSystemInterface::handleGestureEventWithRealValue(
                Q_NULLPTR, timestamp, Qt::ZoomNativeGesture,
                scale, pos, pos);
    if (angle != 0)
        QWindowSystemInterface::handleGestureEventWithRealValue(
                    Q_NULLPTR, timestamp, Qt::RotateNativeGesture,
                    angle, pos, pos);
}

void LibInputGesture::handleSwipeBegin(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

    QWindowSystemInterface::handleGestureEvent(
                Q_NULLPTR, timestamp, Qt::BeginNativeGesture,
                pos, pos);
}

void LibInputGesture::handleSwipeEnd(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

    QWindowSystemInterface::handleGestureEvent(
                Q_NULLPTR, timestamp, Qt::EndNativeGesture,
                pos, pos);
}

void LibInputGesture::handleSwipeUpdate(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(libinput_event_gesture_get_dx(event),
                libinput_event_gesture_get_dy(event));

    QWindowSystemInterface::handleGestureEvent(
                Q_NULLPTR, timestamp, Qt::SwipeNativeGesture,
                pos, pos);
}

} // namespace Platform

} // namespace Liri