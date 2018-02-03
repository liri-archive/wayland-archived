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

#include <QtCore/QSocketNotifier>
#include <QtCore/private/qobject_p.h>

#pragma once

namespace Liri {

class Logind;

namespace Platform {

class VtHandler;

class VtHandlerPrivate
{
    Q_DECLARE_PUBLIC(VtHandler)
public:
    explicit VtHandlerPrivate(VtHandler *self);
    ~VtHandlerPrivate();

    void setup(int nr);

    bool installSignalHandler();
    void toggleTtyCursor(bool enable);
    void toggleKeyboard(bool enable);

    void closeFd();

    void setActive(bool v);

    static bool isValidVt(int fd);
    static void signalHandler(int sigNo);

    Liri::Logind *logind;

    int signalFd[2];
    QSocketNotifier *notifier;

    int vtFd;
    int vtNumber;
    QString devName;

    int kbMode;

    bool active;

protected:
    VtHandler *q_ptr;
};

} // namespace Platform

} // namespace Liri
