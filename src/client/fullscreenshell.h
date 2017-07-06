/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2014-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef LIRIWAYLANDCLIENT_FULLSCREENSHELL_H
#define LIRIWAYLANDCLIENT_FULLSCREENSHELL_H

#include <QtCore/QObject>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

namespace Liri {

namespace WaylandClient {

class FullScreenShellPrivate;
class Output;
class Registry;
class Surface;

class LIRIWAYLANDCLIENT_EXPORT FullScreenShell : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FullScreenShell)
    Q_PROPERTY(Capabilities capabilities READ capabilities NOTIFY capabilitiesChanged)
public:
    enum Capability {
        NoCapability = 0,
        ArbitraryModes,
        CursorPlane
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    enum PresentMethod {
        PresentMethodDefault = 0,
        PresentMethodCenter,
        PresentMethodZoom,
        PresentMethodZoomCrop,
        PresentMethodStretch
    };
    Q_ENUM(PresentMethod)

    Capabilities capabilities() const;

    void presentSurface(Surface *surface, Output *output, PresentMethod method = PresentMethodDefault);
    void hideOutput(Output *output);

Q_SIGNALS:
    void capabilitiesChanged();

private:
    FullScreenShell(QObject *parent = nullptr);

    friend class Registry;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FullScreenShell::Capabilities)

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_FULLSCREENSHELL_H
