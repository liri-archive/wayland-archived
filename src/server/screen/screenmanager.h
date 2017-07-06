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

#ifndef LIRI_SCREENMANAGER_H
#define LIRI_SCREENMANAGER_H

#include <LiriWaylandServer/Screen>

namespace Liri {

namespace WaylandServer {

class ScreenManagerPrivate;

class LIRIWAYLANDSERVER_EXPORT ScreenManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ScreenManager)
    Q_PROPERTY(Screen *primaryScreen READ primaryScreen NOTIFY primaryScreenChanged)
public:
    ScreenManager(QObject *parent = nullptr);

    Screen *primaryScreen() const;

    Q_INVOKABLE int indexOf(Screen *screen) const;

    virtual void create();

Q_SIGNALS:
    void screenAdded(Screen *screen);
    void screenRemoved(Screen *screen);
    void primaryScreenChanged(Screen *screen);
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENMANAGER_H
