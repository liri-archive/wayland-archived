/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QByteArray>

#include "lirieglfsfunctions.h"

namespace Liri {

namespace Platform {

QByteArray EglFSFunctions::getPowerStateIdentifier()
{
    return QByteArrayLiteral("LiriEglFSGetPowerState");
}

EglFSFunctions::PowerState EglFSFunctions::getPowerState(QScreen *screen)
{
    GetPowerStateType func = reinterpret_cast<GetPowerStateType>(QGuiApplication::platformFunction(getPowerStateIdentifier()));
    if (func)
        return func(screen);
    return PowerStateOn;
}

QByteArray EglFSFunctions::setPowerStateIdentifier()
{
    return QByteArrayLiteral("LiriEglFSSetPowerState");
}

void EglFSFunctions::setPowerState(QScreen *screen, PowerState state)
{
    SetPowerStateType func = reinterpret_cast<SetPowerStateType>(QGuiApplication::platformFunction(setPowerStateIdentifier()));
    if (func)
        func(screen, state);
}

QByteArray EglFSFunctions::setScreenPositionIdentifier()
{
    return QByteArrayLiteral("LiriEglFSSetScreenPosition");
}

void EglFSFunctions::setScreenPosition(QScreen *screen, const QPoint &pos)
{
    SetScreenPositionType func = reinterpret_cast<SetScreenPositionType>(QGuiApplication::platformFunction(setScreenPositionIdentifier()));
    if (func)
        func(screen, pos);
}

QByteArray EglFSFunctions::setScreenModeIdentifier()
{
    return QByteArrayLiteral("LiriEglFSSetScreenMode");
}

void EglFSFunctions::setScreenMode(QScreen *screen, int modexIndex)
{
    SetScreenModeType func = reinterpret_cast<SetScreenModeType>(QGuiApplication::platformFunction(setScreenModeIdentifier()));
    if (func)
        func(screen, modexIndex);
}

QByteArray EglFSFunctions::setScreenScaleFactorIdentifier()
{
    return QByteArrayLiteral("LiriEglFSSetScreenScaleFactor");
}

void EglFSFunctions::setScreenScaleFactor(QScreen *screen, qreal factor)
{
    SetScreenScaleFactorType func = reinterpret_cast<SetScreenScaleFactorType>(QGuiApplication::platformFunction(setScreenScaleFactorIdentifier()));
    if (func)
        func(screen, factor);
}

QByteArray EglFSFunctions::enableScreenCaptureIdentifier()
{
    return QByteArrayLiteral("LiriEglFSEnableScreenCapture");
}

void EglFSFunctions::enableScreenCapture(QScreen *screen)
{
    EnableScreenCaptureType func = reinterpret_cast<EnableScreenCaptureType>(QGuiApplication::platformFunction(enableScreenCaptureIdentifier()));
    if (func)
        func(screen);
}

QByteArray EglFSFunctions::disableScreenCaptureIdentifier()
{
    return QByteArrayLiteral("LiriEglFSDisableStreaming");
}

void EglFSFunctions::disableScreenCapture(QScreen *screen)
{
    DisableScreenCaptureType func = reinterpret_cast<DisableScreenCaptureType>(QGuiApplication::platformFunction(disableScreenCaptureIdentifier()));
    if (func)
        func(screen);
}

/*
 * Screen capture
 */

QEvent::Type FrameCaptureEvent::eventType = QEvent::None;

FrameCaptureEvent::FrameCaptureEvent(const QImage &image)
    : QEvent(registeredType())
    , capture(image)
{
}

QEvent::Type FrameCaptureEvent::registeredType()
{
    if (eventType == QEvent::None) {
        int generatedType = QEvent::registerEventType();
        eventType = static_cast<QEvent::Type>(generatedType);
    }

    return eventType;
}

} // namespace Platform

} // namespace Liri
