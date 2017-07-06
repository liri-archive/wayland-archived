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

#ifndef LIRI_QUICKOUTPUT_H
#define LIRI_QUICKOUTPUT_H

#include <QtQml/QQmlListProperty>

#include <QtWaylandCompositor/QWaylandQuickOutput>
#include <LiriWaylandServer/Screen>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

namespace Liri {

namespace WaylandServer {

class QuickOutputPrivate;

class LIRIWAYLANDSERVER_EXPORT QuickOutput : public QWaylandQuickOutput
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QuickOutput)
    Q_PROPERTY(Liri::WaylandServer::Screen *nativeScreen READ nativeScreen WRITE setNativeScreen NOTIFY nativeScreenChanged DESIGNABLE false)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(PowerState powerState READ powerState WRITE setPowerState NOTIFY powerStateChanged)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    enum PowerState {
        PowerStateOn,
        PowerStateStandby,
        PowerStateSuspend,
        PowerStateOff
    };
    Q_ENUM(PowerState)

    QuickOutput();
    QuickOutput(QWaylandCompositor *compositor);

    QQmlListProperty<QObject> data();

    Liri::WaylandServer::Screen *nativeScreen() const;
    void setNativeScreen(Screen *screen);

    bool isEnabled() const;
    void setEnabled(bool value);

    PowerState powerState() const;
    void setPowerState(PowerState state);

    static QuickOutput *fromResource(wl_resource *resource);

protected:
    void initialize() override;

Q_SIGNALS:
    void nativeScreenChanged();
    void enabledChanged();
    void powerStateChanged();

private:
    QuickOutputPrivate *const d_ptr;

private Q_SLOTS:
    void readContent();
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_QUICKOUTPUT_H
