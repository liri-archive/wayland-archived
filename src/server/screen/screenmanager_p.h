/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef LIRI_SCREENMANAGER_P_H
#define LIRI_SCREENMANAGER_P_H

#include <QtCore/QCoreApplication>
#include <QtCore/private/qobject_p.h>

#include <LiriWaylandServer/ScreenManager>

#include "screen/fakescreenbackend.h"
#include "screen/nativescreenbackend.h"

namespace Liri {

namespace WaylandServer {

class LIRIWAYLANDSERVER_EXPORT ScreenManagerPrivate : public QObjectPrivate
{
public:
    ScreenManagerPrivate()
        : QObjectPrivate()
        , primaryScreen(nullptr)
    {
        // Determine the backend to use from the application
        const QString backendName = QCoreApplication::instance()->property("__liri_screen_backend").toString();
        const QString fileName = QCoreApplication::instance()->property("__liri_screen_configuration").toString();
        if (backendName == QLatin1String("fake") && !fileName.isEmpty()) {
            backend = new FakeScreenBackend();
            static_cast<FakeScreenBackend *>(backend)->setConfiguration(fileName);
        } else {
            backend = new NativeScreenBackend();
        }
    }

    ~ScreenManagerPrivate()
    {
        delete backend;
    }

    ScreenBackend *backend;
    Screen *primaryScreen;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENMANAGER_P_H

