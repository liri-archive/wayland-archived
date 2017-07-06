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

#include "screenmanager.h"
#include "screenmanager_p.h"

namespace Liri {

namespace WaylandServer {

ScreenManager::ScreenManager(QObject *parent)
    : QObject(*new ScreenManagerPrivate(), parent)
{
    Q_D(ScreenManager);

    connect(d->backend, &ScreenBackend::screenAdded,
            this, &ScreenManager::screenAdded);
    connect(d->backend, &ScreenBackend::screenRemoved,
            this, &ScreenManager::screenRemoved);
    connect(d->backend, &ScreenBackend::primaryScreenChanged, this,
            [this, d](Screen *screen) {
        d->primaryScreen = screen;
        Q_EMIT primaryScreenChanged(screen);
    });
}

Screen *ScreenManager::primaryScreen() const
{
    Q_D(const ScreenManager);
    return d->primaryScreen;
}

int ScreenManager::indexOf(Screen *screen) const
{
    Q_D(const ScreenManager);
    return d->backend->screens().indexOf(screen);
}

void ScreenManager::create()
{
    Q_D(ScreenManager);
    d->backend->acquireConfiguration();
}

} // namespace WaylandServer

} // namespace Liri

#include "moc_screenmanager.cpp"
