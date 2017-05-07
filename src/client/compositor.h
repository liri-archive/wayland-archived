/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
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

#ifndef LIRIWAYLANDCLIENT_COMPOSITOR_H
#define LIRIWAYLANDCLIENT_COMPOSITOR_H

#include <QtCore/QObject>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

namespace Liri {

namespace WaylandClient {

class CompositorPrivate;
class Region;
class Registry;
class Surface;

class LIRIWAYLANDCLIENT_EXPORT Compositor : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Compositor)
public:
    Surface *createSurface(QObject *parent = Q_NULLPTR);
    Region *createRegion(QObject *parent = Q_NULLPTR);
    Region *createRegion(const QRegion &region, QObject *parent = Q_NULLPTR);

    static QByteArray interfaceName();

private:
    Compositor(QObject *parent = Q_NULLPTR);

    friend class Registry;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_COMPOSITOR_H
