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

#ifndef LIRI_SCREENCASTER_P_H
#define LIRI_SCREENCASTER_P_H

#include <QtQuick/QQuickWindow>

#include <QtWaylandCompositor/QWaylandOutput>
#include <QtWaylandCompositor/private/qwaylandcompositorextension_p.h>

#include <LiriWaylandServer/Screencaster>
#include <LiriWaylandServer/private/qwayland-server-liri-screencaster.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Green Island API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Liri {

namespace WaylandServer {

class LIRIWAYLANDSERVER_EXPORT ScreencasterPrivate
        : public QWaylandCompositorExtensionPrivate
        , public QtWaylandServer::liri_screencaster
{
    Q_DECLARE_PUBLIC(Screencaster)
public:
    ScreencasterPrivate();

    QMutex requestsMutex;
    QMultiHash<QQuickWindow *, Screencast *> requests;

    void addRequest(QQuickWindow *window, Screencast *screencast);
    void removeRequest(QQuickWindow *window, Screencast *screencast);

    static ScreencasterPrivate *get(Screencaster *screencaster) { return screencaster->d_func(); }

protected:
    void liri_screencaster_bind_resource(Resource *resource) Q_DECL_OVERRIDE;
    void liri_screencaster_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void liri_screencaster_capture(Resource *resource,
                                   uint32_t id,
                                   struct ::wl_resource *outputResource) Q_DECL_OVERRIDE;
};

class LIRIWAYLANDSERVER_EXPORT ScreencastPrivate
        : public QWaylandCompositorExtensionPrivate
        , public QtWaylandServer::liri_screencast
{
    Q_DECLARE_PUBLIC(Screencast)
public:
    ScreencastPrivate();

    bool valid;
    Screencaster *screencaster;
    QWaylandOutput *output;
    QQuickWindow *window;
    wl_resource *bufferResource;
    wl_shm_buffer *buffer;

    void frameRecording(Screencast *screencast, uint time,
                        const QImage &image);

    static ScreencastPrivate *get(Screencast *screencast) { return screencast->d_func(); }

protected:
    void liri_screencast_destroy(Resource *resource) Q_DECL_OVERRIDE;
    void liri_screencast_record(Resource *resource,
                                struct ::wl_resource *bufferResource) Q_DECL_OVERRIDE;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENCASTER_P_H
