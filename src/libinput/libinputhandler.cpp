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

#include <QtCore/QSocketNotifier>
#include <QtGui/private/qguiapplication_p.h>
#include <qplatformdefs.h>

#include <Qt5Udev/private/udev_p.h>
#include <LiriLogind/Logind>

#include "logging_p.h"
#include "libinputhandler.h"
#include "libinputhandler_p.h"

namespace Liri {

namespace Platform {

/*
 * HandlerPrivate
 */

const struct libinput_interface LibInputHandlerPrivate::liInterface = {
    LibInputHandlerPrivate::restrictedOpenCallback,
    LibInputHandlerPrivate::restrictedCloseCallback
};

LibInputHandlerPrivate::LibInputHandlerPrivate()
    : initialized(false)
    , udev(nullptr)
    , li(nullptr)
    , keyboard(nullptr)
    , keyboardCount(0)
    , pointer(nullptr)
    , pointerCount(0)
    , touch(nullptr)
    , touchCount(0)
    , gesture(nullptr)
    , gestureCount(0)
    , suspended(false)
{
}

LibInputHandlerPrivate::~LibInputHandlerPrivate()
{
    delete keyboard;
    delete pointer;
    delete touch;
    delete gesture;

    if (li)
        libinput_unref(li);

    delete udev;
}

void LibInputHandlerPrivate::setup()
{
    Q_Q(LibInputHandler);

    // Initialize
    initialize();
    qCDebug(lcInput) << "Setting up libinput";

    // Receive events
    int fd = libinput_get_fd(li);
    QSocketNotifier *notifier = new QSocketNotifier(fd, QSocketNotifier::Read, q);
    q->connect(notifier, SIGNAL(activated(int)), q, SLOT(handleEvents()));

    // Suspend/resume when the session is activated or deactivated
    Logind *logind = Logind::instance();
    q->connect(logind, &Logind::sessionActiveChanged, q, [this, q](bool active) {
        if (active) {
            if (!suspended)
                return;
            q->resume();
        } else if (!suspended) {
            q->suspend();
            q->handleEvents();
        }
    });

    // Pick up the initial events for devices being added
    q->handleEvents();
}

void LibInputHandlerPrivate::initialize()
{
    Q_Q(LibInputHandler);

    qCDebug(lcInput) << "Initializing libinput";

    // Create context
    udev = new QtUdev::Udev;
    li = libinput_udev_create_context(&liInterface, nullptr,
                                      QtUdev::UdevPrivate::get(udev)->udev);
    if (Q_UNLIKELY(!li)) {
        qFatal("Unable to get libinput context");
        return;
    }

    // Setup log handler
    libinput_log_set_handler(li, logHandler);
    if (lcInput().isDebugEnabled())
        libinput_log_set_priority(li, LIBINPUT_LOG_PRIORITY_DEBUG);
    else if (lcInput().isInfoEnabled())
        libinput_log_set_priority(li, LIBINPUT_LOG_PRIORITY_INFO);
    else if (lcInput().isWarningEnabled())
        libinput_log_set_priority(li, LIBINPUT_LOG_PRIORITY_ERROR);

    // Assign current seat, relies on XDG_SEAT being set correctly as
    // this would be the case of a session initiated by pam_systemd
    if (Q_UNLIKELY(libinput_udev_assign_seat(li, qgetenv("XDG_SEAT").constData()) != 0)) {
        qFatal("Failed to assign seat to libinput");
        return;
    }
    qCDebug(lcInput, "Assigned seat \"%s\" to udev", qgetenv("XDG_SEAT").constData());

    keyboard = new LibInputKeyboard(q);
    pointer = new LibInputPointer(q);
    touch = new LibInputTouch(q);
    gesture = new LibInputGesture(q);

    initialized = true;
}

void LibInputHandlerPrivate::logHandler(libinput *handle, libinput_log_priority priority,
                                        const char *format, va_list args)
{
    Q_UNUSED(handle);

    char buffer[512];
    int n = vsnprintf(buffer, sizeof(buffer), format, args);
    if (n > 0) {
        // Remove newline
        if (buffer[n - 1] == '\n')
            buffer[n - 1] = '\0';

        // Log the message prefixing "libinput" so that messages from the
        // library can be dinstinguished from ours
        switch (priority) {
        case LIBINPUT_LOG_PRIORITY_DEBUG:
            qCDebug(lcInput, "%s", buffer);
            break;
        case LIBINPUT_LOG_PRIORITY_ERROR:
            qCWarning(lcInput, "%s", buffer);
            break;
        case LIBINPUT_LOG_PRIORITY_INFO:
            qCInfo(lcInput, "%s", buffer);
            break;
        default:
            break;
        }
    }
}

int LibInputHandlerPrivate::restrictedOpenCallback(const char *path, int flags, void *user_data)
{
    return static_cast<LibInputHandlerPrivate *>(user_data)->restrictedOpen(path, flags);
}

void LibInputHandlerPrivate::restrictedCloseCallback(int fd, void *user_data)
{
    static_cast<LibInputHandlerPrivate *>(user_data)->restrictedClose(fd);
}

int LibInputHandlerPrivate::restrictedOpen(const char *path, int flags)
{
    Logind *logind = Logind::instance();

    // Take the device
    int fd = logind->takeDevice(QString::fromLatin1(path));
    if (fd < 0)
        return -1;

    auto closeFileDescriptor = [fd, this]() {
        close(fd);
        restrictedClose(fd);
    };

    // Adjust the flags
    int fl = ::fcntl(fd, F_GETFL);
    if (fl < 0) {
        closeFileDescriptor();
        return -1;
    }

    if (flags & O_NONBLOCK)
        fl |= O_NONBLOCK;

    if (::fcntl(fd, F_SETFL, fl) < 0) {
        closeFileDescriptor();
        return -1;
    }

    fl = ::fcntl(fd, F_GETFD);
    if (fl < 0) {
        closeFileDescriptor();
        return -1;
    }

    if (!(flags & O_CLOEXEC))
        fl &= ~FD_CLOEXEC;

    if (::fcntl(fd, F_SETFD, fl) < 0) {
        closeFileDescriptor();
        return -1;
    }

    return fd;
}

void LibInputHandlerPrivate::restrictedClose(int fd)
{
    Logind *logind = Logind::instance();
    logind->releaseDevice(fd);
}

/*
 * Handler
 */

LibInputHandler::LibInputHandler(QObject *parent)
    : QObject(*new LibInputHandlerPrivate, parent)
{
    Q_D(LibInputHandler);

    if (Logind::instance()->isConnected())
        qCDebug(lcInput) << "logind connection established";
    else
        qCDebug(lcInput) << "logind connection not yet established";

    // Setup libinput if we are already connected to logind or
    // do it when we connect
    if (Logind::instance()->isConnected())
        d->setup();
    else
        connect(Logind::instance(), &Logind::connectedChanged, this, [d](bool connected) {
            // Setup libinput the first time we connect
            if (!connected || d->initialized)
                return;

            // Initialize
            d->setup();
        });
}

bool LibInputHandler::isSuspended() const
{
    Q_D(const LibInputHandler);
    return d->suspended;
}

LibInputHandler::Capabilities LibInputHandler::capabilities() const
{
    Q_D(const LibInputHandler);

    LibInputHandler::Capabilities caps = 0;

    if (d->keyboardCount > 0)
        caps &= LibInputHandler::Keyboard;
    if (d->pointerCount > 0)
        caps &= LibInputHandler::Pointer;
    if (d->touchCount > 0)
        caps &= LibInputHandler::Touch;
    if (d->gestureCount > 0)
        caps &= LibInputHandler::Touch;

    return caps;
}

void LibInputHandler::setPointerPosition(const QPoint &pos)
{
    Q_D(LibInputHandler);
    d->pointer->setPosition(pos);
}

void LibInputHandler::suspend()
{
    Q_D(LibInputHandler);

    if (d->suspended)
        return;

    qCInfo(lcInput, "Suspend monitoring for new devices");
    libinput_suspend(d->li);
    d->suspended = true;
    Q_EMIT suspendedChanged(true);
}

void LibInputHandler::resume()
{
    Q_D(LibInputHandler);

    if (!d->suspended)
        return;

    if (libinput_resume(d->li) == 0) {
        qCInfo(lcInput, "Re-enable device monitoring");
        d->suspended = false;
        Q_EMIT suspendedChanged(false);
    } else {
        qCWarning(lcInput, "Failed to re-enable device monitoring");
    }
}

void LibInputHandler::handleEvents()
{
    Q_D(LibInputHandler);

    if (libinput_dispatch(d->li) != 0) {
        qCWarning(lcInput) << "Failed to dispatch libinput events";
        return;
    }

    libinput_event *event;
    while ((event = libinput_get_event(d->li)) != nullptr) {
        libinput_event_type type = libinput_event_get_type(event);
        libinput_device *device = libinput_event_get_device(event);

        switch (type) {
        // Devices
        case LIBINPUT_EVENT_DEVICE_ADDED:
            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_KEYBOARD)) {
                ++d->keyboardCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT keyboardCountChanged(d->keyboardCount);
            }

            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_POINTER)) {
                ++d->pointerCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT pointerCountChanged(d->pointerCount);
            }

            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_TOUCH)) {
                QTouchDevice *td = d->touch->registerDevice(device);
                Q_EMIT touchDeviceRegistered(td);

                ++d->touchCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT touchCountChanged(d->touchCount);
            }

            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_GESTURE)) {
                ++d->gestureCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT gestureCountChanged(d->gestureCount);
            }
            break;
        case LIBINPUT_EVENT_DEVICE_REMOVED:
            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_KEYBOARD)) {
                --d->keyboardCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT keyboardCountChanged(d->keyboardCount);
            }

            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_POINTER)) {
                --d->pointerCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT pointerCountChanged(d->pointerCount);
            }

            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_TOUCH)) {
                QTouchDevice *td = nullptr;
                d->touch->unregisterDevice(device, &td);
                Q_EMIT touchDeviceUnregistered(td);

                --d->touchCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT touchCountChanged(d->touchCount);
            }

            if (libinput_device_has_capability(device, LIBINPUT_DEVICE_CAP_GESTURE)) {
                --d->gestureCount;
                Q_EMIT capabilitiesChanged();
                Q_EMIT gestureCountChanged(d->gestureCount);
            }
            break;
            // Keyboard
        case LIBINPUT_EVENT_KEYBOARD_KEY:
            d->keyboard->handleKey(libinput_event_get_keyboard_event(event));
            break;
            // Pointer
        case LIBINPUT_EVENT_POINTER_BUTTON:
            d->pointer->handleButton(libinput_event_get_pointer_event(event));
            break;
        case LIBINPUT_EVENT_POINTER_AXIS:
            d->pointer->handleAxis(libinput_event_get_pointer_event(event));
            break;
        case LIBINPUT_EVENT_POINTER_MOTION:
            d->pointer->handleMotion(libinput_event_get_pointer_event(event));
            break;
        case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
            d->pointer->handleAbsoluteMotion(libinput_event_get_pointer_event(event));
            break;
            // Touch
        case LIBINPUT_EVENT_TOUCH_UP:
            d->touch->handleTouchUp(libinput_event_get_touch_event(event));
            break;
        case LIBINPUT_EVENT_TOUCH_DOWN:
            d->touch->handleTouchDown(libinput_event_get_touch_event(event));
            break;
        case LIBINPUT_EVENT_TOUCH_FRAME:
            d->touch->handleTouchFrame(libinput_event_get_touch_event(event));
            break;
        case LIBINPUT_EVENT_TOUCH_MOTION:
            d->touch->handleTouchMotion(libinput_event_get_touch_event(event));
            break;
        case LIBINPUT_EVENT_TOUCH_CANCEL:
            d->touch->handleTouchCancel(libinput_event_get_touch_event(event));
            break;
            // Gesture
        case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
            d->gesture->handlePinchBegin(libinput_event_get_gesture_event(event));
            break;
        case LIBINPUT_EVENT_GESTURE_PINCH_END:
            d->gesture->handlePinchEnd(libinput_event_get_gesture_event(event));
            break;
        case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE:
            d->gesture->handlePinchUpdate(libinput_event_get_gesture_event(event));
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
            d->gesture->handleSwipeBegin(libinput_event_get_gesture_event(event));
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_END:
            d->gesture->handleSwipeEnd(libinput_event_get_gesture_event(event));
            break;
        case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
            d->gesture->handleSwipeUpdate(libinput_event_get_gesture_event(event));
            break;
        default:
            break;
        }

        libinput_event_destroy(event);
    }
}

} // namespace Platform

} // namespace Liri
