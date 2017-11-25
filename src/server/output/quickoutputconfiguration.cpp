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

#include "outputconfiguration_p.h"
#include "quickoutputconfiguration.h"
#include "quickoutputconfiguration_p.h"

namespace Liri {

namespace WaylandServer {

/*
 * QuickOutputConfigurationPrivate
 */

QuickOutputConfigurationPrivate::QuickOutputConfigurationPrivate(QuickOutputConfiguration *self)
    : q_ptr(self)
{
}

QQmlListProperty<OutputChangeset> QuickOutputConfigurationPrivate::changes()
{
    Q_Q(QuickOutputConfiguration);
    return QQmlListProperty<OutputChangeset>(q, nullptr, changesCount, changesAt);
}

int QuickOutputConfigurationPrivate::changesCount(QQmlListProperty<OutputChangeset> *prop)
{
    OutputConfiguration *that = static_cast<OutputConfiguration *>(prop->object);
    return OutputConfigurationPrivate::get(that)->changes.values().count();
}

OutputChangeset *QuickOutputConfigurationPrivate::changesAt(QQmlListProperty<OutputChangeset> *prop, int index)
{
    OutputConfiguration *that = static_cast<OutputConfiguration *>(prop->object);
    return OutputConfigurationPrivate::get(that)->changes.values().at(index);
}

/*
 * QuickOutputConfiguration
 */

QuickOutputConfiguration::QuickOutputConfiguration()
    : OutputConfiguration()
    , d_ptr(new QuickOutputConfigurationPrivate(this))
{
}

QuickOutputConfiguration::~QuickOutputConfiguration()
{
    delete d_ptr;
}

QQmlListProperty<QObject> QuickOutputConfiguration::data()
{
    Q_D(QuickOutputConfiguration);
    return QQmlListProperty<QObject>(this, d->objects);
}

} // namespace WaylandServer

} // namespace Liri

#include "moc_quickoutputconfiguration.cpp"
