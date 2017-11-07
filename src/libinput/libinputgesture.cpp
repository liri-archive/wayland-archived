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

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QWindowSystemInterface::handleGestureEvent(
                nullptr, nullptr, timestamp, Qt::BeginNativeGesture,
                pos, pos);
#else
    QWindowSystemInterface::handleGestureEvent(
                nullptr, timestamp, Qt::BeginNativeGesture,
                pos, pos);
#endif
}

void LibInputGesture::handlePinchEnd(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QWindowSystemInterface::handleGestureEvent(
                nullptr, nullptr, timestamp, Qt::EndNativeGesture,
                pos, pos);
#else
    QWindowSystemInterface::handleGestureEvent(
                nullptr, timestamp, Qt::EndNativeGesture,
                pos, pos);
#endif
}

void LibInputGesture::handlePinchUpdate(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    const double scale = libinput_event_gesture_get_scale(event);
    const double angle = libinput_event_gesture_get_angle_delta(event);
    QPointF pos(libinput_event_gesture_get_dx(event),
                libinput_event_gesture_get_dy(event));

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QWindowSystemInterface::handleGestureEventWithRealValue(
                nullptr, nullptr, timestamp, Qt::ZoomNativeGesture,
                scale, pos, pos);
    if (angle != 0.0)
        QWindowSystemInterface::handleGestureEventWithRealValue(
                    nullptr, nullptr, timestamp, Qt::RotateNativeGesture,
                    angle, pos, pos);
#else
    QWindowSystemInterface::handleGestureEventWithRealValue(
                nullptr, timestamp, Qt::ZoomNativeGesture,
                scale, pos, pos);
    if (angle != 0.0)
        QWindowSystemInterface::handleGestureEventWithRealValue(
                    nullptr, timestamp, Qt::RotateNativeGesture,
                    angle, pos, pos);
#endif
}

void LibInputGesture::handleSwipeBegin(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QWindowSystemInterface::handleGestureEvent(
                nullptr, nullptr, timestamp, Qt::BeginNativeGesture,
                pos, pos);
#else
    QWindowSystemInterface::handleGestureEvent(
                nullptr, timestamp, Qt::BeginNativeGesture,
                pos, pos);
#endif
}

void LibInputGesture::handleSwipeEnd(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(0, 0);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QWindowSystemInterface::handleGestureEvent(
                nullptr, nullptr, timestamp, Qt::EndNativeGesture,
                pos, pos);
#else
    QWindowSystemInterface::handleGestureEvent(
                nullptr, timestamp, Qt::EndNativeGesture,
                pos, pos);
#endif
}

void LibInputGesture::handleSwipeUpdate(libinput_event_gesture *event)
{
    const quint64 timestamp = libinput_event_gesture_get_time_usec(event);
    QPointF pos(libinput_event_gesture_get_dx(event),
                libinput_event_gesture_get_dy(event));

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QWindowSystemInterface::handleGestureEvent(
                nullptr, nullptr, timestamp, Qt::SwipeNativeGesture,
                pos, pos);
#else
    QWindowSystemInterface::handleGestureEvent(
                nullptr, timestamp, Qt::SwipeNativeGesture,
                pos, pos);
#endif
}

} // namespace Platform

} // namespace Liri
