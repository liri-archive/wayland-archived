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

#ifndef LIRI_OUTPUTCONFIGURATION_P_H
#define LIRI_OUTPUTCONFIGURATION_P_H

#include <QtWaylandCompositor/QWaylandOutput>
#include <QtWaylandCompositor/private/qwaylandcompositorextension_p.h>

#include <LiriWaylandServer/OutputConfiguration>
#include "qwayland-server-liri-outputmanagement.h"

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
                                                 int32_t enable) override;
    virtual void liri_outputconfiguration_primary(Resource *resource,
                                                  struct ::wl_resource *outputResource,
                                                  int32_t primary) override;
    virtual void liri_outputconfiguration_mode(Resource *resource,
                                               struct ::wl_resource *outputResource,
                                               int32_t mode_id) override;
    virtual void liri_outputconfiguration_transform(Resource *resource,
                                                    struct ::wl_resource *outputResource,
                                                    int32_t wlTransform) override;
    virtual void liri_outputconfiguration_position(Resource *resource,
                                                   struct ::wl_resource *outputResource,
                                                   int32_t x, int32_t y) override;
    virtual void liri_outputconfiguration_scale(Resource *resource,
                                                struct ::wl_resource *outputResource,
                                                int32_t scale) override;
    virtual void liri_outputconfiguration_apply(Resource *resource) override;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_OUTPUTCONFIGURATION_P_H
