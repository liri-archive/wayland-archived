/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2014-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL$
 *
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 or later as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPLv21 included in the packaging of
 * this file.  Please review the following information to ensure the
 * GNU Lesser General Public License version 2.1 requirements will be
 * met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * Alternatively, this file may be used under the terms of the GNU General
 * Public License version 2.0 or later as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPLv2 included in the
 * packaging of this file.  Please review the following information to ensure
 * the GNU General Public License version 2.0 requirements will be
 * met: http://www.gnu.org/licenses/gpl-2.0.html.
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
