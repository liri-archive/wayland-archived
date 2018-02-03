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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QLoggingCategory>

#include "qeglfsglobal_p.h"

Q_DECLARE_LOGGING_CATEGORY(lcVtHandler)

namespace Liri {

namespace Platform {

class VtHandlerPrivate;

class Q_EGLFS_EXPORT VtHandler : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(VtHandler)
public:
    explicit VtHandler(QObject *parent = nullptr);
    ~VtHandler();

    bool isActive() const;

    void activate(quint32 nr);

Q_SIGNALS:
    void created();
    void activeChanged(bool active);
    void interrupted();
    void aboutToSuspend();
    void resumed();

private:
    VtHandlerPrivate *const d_ptr;
};

} // namespace Platform

} // namespace Liri
