/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini
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

#include "output_p.h"
#include "outputconfiguration.h"
#include "outputconfiguration_p.h"

namespace Liri {

namespace WaylandClient {

/*
 * OutputConfigurationPrivate
 */

OutputConfigurationPrivate::OutputConfigurationPrivate()
    : QtWayland::liri_outputconfiguration()
{
}

/*
 * OutputConfiguration
 */

OutputConfiguration::OutputConfiguration(QObject *parent)
    : QObject(*new OutputConfigurationPrivate(), parent)
{
}

void OutputConfiguration::setEnabled(Output *output, bool value)
{
    Q_D(OutputConfiguration);

    auto wlOutput = OutputPrivate::get(output)->object();
    d->enable(wlOutput, value ? 1 : 0);
}

void OutputConfiguration::setPrimary(Output *output, bool value)
{
    Q_D(OutputConfiguration);

    auto wlOutput = OutputPrivate::get(output)->object();
    d->primary(wlOutput, value ? 1 : 0);
}

void OutputConfiguration::setModeId(Output *output, int modeId)
{
    Q_D(OutputConfiguration);

    auto wlOutput = OutputPrivate::get(output)->object();
    d->mode(wlOutput, modeId);
}

void OutputConfiguration::setTransform(Output *output, Output::Transform transform)
{
    Q_D(OutputConfiguration);

    // TODO: Convert transform to wltransform
    auto wlOutput = OutputPrivate::get(output)->object();
    d->transform(wlOutput, int32_t(transform));
}

void OutputConfiguration::setPosition(Output *output, const QPoint &position)
{
    Q_D(OutputConfiguration);

    auto wlOutput = OutputPrivate::get(output)->object();
    d->position(wlOutput, position.x(), position.y());
}

void OutputConfiguration::setScaleFactor(Output *output, qint32 scaleFactor)
{
    Q_D(OutputConfiguration);

    auto wlOutput = OutputPrivate::get(output)->object();
    d->scale(wlOutput, scaleFactor);
}

void OutputConfiguration::apply()
{
    Q_D(OutputConfiguration);
    d->apply();
}

} // namespace WaylandClient

} // namespace Liri

#include "moc_outputconfiguration.cpp"
