/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef LIRI_OUTPUTCONFIGURATION_H
#define LIRI_OUTPUTCONFIGURATION_H

#include <QtCore/QObject>

#include <QtWaylandCompositor/QWaylandCompositorExtension>
#include <QtWaylandCompositor/QWaylandResource>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_BEGIN_NAMESPACE

class QWaylandOutput;

QT_END_NAMESPACE

namespace Liri {

namespace WaylandServer {

class OutputChangeset;
class OutputConfigurationPrivate;
class OutputManagement;
class OutputManagementPrivate;

class LIRIWAYLANDSERVER_EXPORT OutputConfiguration
        : public QWaylandCompositorExtensionTemplate<OutputConfiguration>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OutputConfiguration)
public:
    OutputConfiguration();
    explicit OutputConfiguration(OutputManagement *parent, const QWaylandResource &resource);
    virtual ~OutputConfiguration();

    Q_INVOKABLE void initialize(Liri::WaylandServer::OutputManagement *parent, const QWaylandResource &resource);
    
    QHash<QWaylandOutput *, OutputChangeset *> changes() const;

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

    static OutputConfiguration *fromResource(wl_resource *resource);

Q_SIGNALS:
    void changeRequested();

public Q_SLOTS:
    void setApplied();
    void setFailed();

private:
    OutputConfigurationPrivate *const d_ptr;

    void initialize() override;
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_OUTPUTCONFIGURATION_H
