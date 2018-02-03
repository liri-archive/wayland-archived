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

#ifndef LIRI_SERVERLOGGING_H
#define LIRI_SERVERLOGGING_H

#include <QtCore/QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(gLcCore)
Q_DECLARE_LOGGING_CATEGORY(gLcOutputManagement)
Q_DECLARE_LOGGING_CATEGORY(gLcGtkShell)
Q_DECLARE_LOGGING_CATEGORY(gLcGtkShellTrace)
Q_DECLARE_LOGGING_CATEGORY(gLcScreencaster)
Q_DECLARE_LOGGING_CATEGORY(gLcScreenshooter)
Q_DECLARE_LOGGING_CATEGORY(gLcTaskManager)
Q_DECLARE_LOGGING_CATEGORY(gLcXdgShell)
Q_DECLARE_LOGGING_CATEGORY(gLcXdgShellTrace)
Q_DECLARE_LOGGING_CATEGORY(gLcScreenBackend)
Q_DECLARE_LOGGING_CATEGORY(gLcFakeScreenBackend)
Q_DECLARE_LOGGING_CATEGORY(gLcNativeScreenBackend)

#endif // LIRI_SERVERLOGGING_H
