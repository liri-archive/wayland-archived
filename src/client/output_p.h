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

#ifndef LIRIWAYLANDCLIENT_OUTPUT_P_H
#define LIRIWAYLANDCLIENT_OUTPUT_P_H

#include <QtCore/private/qobject_p.h>

#include <LiriWaylandClient/Output>
#include <LiriWaylandClient/private/qwayland-wayland.h>

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

namespace WaylandClient {

class LIRIWAYLANDCLIENT_EXPORT OutputPrivate
        : public QObjectPrivate
        , public QtWayland::wl_output
{
    Q_DECLARE_PUBLIC(Output)
public:
    OutputPrivate();

    QString manufacturer;
    QString model;

    QPoint position;
    QSize physicalSize;
    qint32 scale;

    QVector<Output::Mode> modes;
    QVector<Output::Mode>::iterator currentMode;

    Output::Subpixel subpixelValue;
    Output::Transform transformValue;

    static Output *fromWlOutput(struct ::wl_output *output);
    static OutputPrivate *get(Output *output) { return output->d_func(); }

protected:
    void output_geometry(int32_t x, int32_t y,
                         int32_t physical_width, int32_t physical_height,
                         int32_t subpixel, const QString &make,
                         const QString &model, int32_t transform) override;
    void output_mode(uint32_t flags, int32_t width, int32_t height, int32_t refresh) override;
    void output_done() override;
    void output_scale(int32_t factor) override;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRIWAYLANDCLIENT_OUTPUT_P_H
