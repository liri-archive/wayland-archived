/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini
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

#ifndef LIRI_OUTPUTCONFIGURATION_P_H
#define LIRI_OUTPUTCONFIGURATION_P_H

#include <QtWaylandCompositor/QWaylandOutput>
#include <QtWaylandCompositor/private/qwaylandcompositorextension_p.h>

#include <LiriWaylandServer/OutputConfiguration>
#include <LiriWaylandServer/private/qwayland-server-liri-outputmanagement.h>

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

class OutputChangeset;

class LIRIWAYLANDSERVER_EXPORT OutputConfigurationPrivate
        : public QWaylandCompositorExtensionPrivate
        , public QtWaylandServer::liri_outputconfiguration
{
    Q_DECLARE_PUBLIC(OutputConfiguration)
public:
    OutputConfigurationPrivate();

    OutputManagement *management;
    QHash<QWaylandOutput *, OutputChangeset *> changes;

    OutputChangeset *pendingChanges(QWaylandOutput *output);
    bool hasPendingChanges(QWaylandOutput *output) const;
    void clearPendingChanges();

    static OutputConfigurationPrivate *get(OutputConfiguration *configuration) { return configuration->d_func(); }

protected:
    virtual void liri_outputconfiguration_enable(Resource *resource,
                                                 struct ::wl_resource *outputResource,
                                                 int32_t enable) Q_DECL_OVERRIDE;
    virtual void liri_outputconfiguration_primary(Resource *resource,
                                                  struct ::wl_resource *outputResource,
                                                  int32_t primary) Q_DECL_OVERRIDE;
    virtual void liri_outputconfiguration_mode(Resource *resource,
                                               struct ::wl_resource *outputResource,
                                               int32_t mode_id) Q_DECL_OVERRIDE;
    virtual void liri_outputconfiguration_transform(Resource *resource,
                                                    struct ::wl_resource *outputResource,
                                                    int32_t wlTransform) Q_DECL_OVERRIDE;
    virtual void liri_outputconfiguration_position(Resource *resource,
                                                   struct ::wl_resource *outputResource,
                                                   int32_t x, int32_t y) Q_DECL_OVERRIDE;
    virtual void liri_outputconfiguration_scale(Resource *resource,
                                                struct ::wl_resource *outputResource,
                                                int32_t scale) Q_DECL_OVERRIDE;
    virtual void liri_outputconfiguration_apply(Resource *resource) Q_DECL_OVERRIDE;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_OUTPUTCONFIGURATION_P_H
