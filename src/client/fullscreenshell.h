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
    FullScreenShell(QObject *parent = Q_NULLPTR);

    friend class Registry;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FullScreenShell::Capabilities)

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_FULLSCREENSHELL_H
