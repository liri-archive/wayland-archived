/****************************************************************************
 * This file is part of Green Island.
 *
 * Copyright (C) 2012-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef COMPOSITOR_P_H
#define COMPOSITOR_P_H

#include <QtQml/QQmlListProperty>
#include <QtCompositor/QWaylandSurface>

#include "applicationmanager.h"
#include "compositorsettings.h"
#include "screenmanager.h"

//  W A R N I N G
//  -------------
//
// This file is not part of the Green Island API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

typedef QList<QWaylandSurface *> QWaylandSurfaceList;

namespace GreenIsland {

class Compositor;
class ClientWindow;
class ShellWindow;

class CompositorPrivate
{
public:
    CompositorPrivate(Compositor *self);

    QQmlListProperty<ClientWindow> windows();
    QQmlListProperty<ShellWindow> shellWindows();

    void dpms(bool on);

    void mapWindow(ClientWindow *window);
    void unmapWindow(ClientWindow *window);
    void destroyWindow(ClientWindow *window);

    void mapShellWindow(ShellWindow *window);
    void unmapShellWindow(ShellWindow *window);
    void destroyShellWindow(ShellWindow *window);

    void _q_updateCursor(bool hasBuffer);

    bool running;

    Compositor::State state;
    int idleInterval;
    int idleInhibit;

    bool locked;

    // Cursor
    QWaylandSurface *cursorSurface;
    int cursorHotspotX;
    int cursorHotspotY;

    QWaylandSurface *lastKeyboardFocus;

    CompositorSettings *settings;
    ScreenManager *screenManager;

    // Application management
    ApplicationManager *appManager;
    QHash<QString, QWaylandSurfaceList> appSurfaces;
    QList<ClientWindow *> clientWindowsList;
    QList<ShellWindow *> shellWindowsList;

protected:
    Q_DECLARE_PUBLIC(Compositor)
    Compositor *const q_ptr;
};

}

#endif // COMPOSITOR_P_H
