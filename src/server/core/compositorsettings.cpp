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

#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandSeat>

#include "compositorsettings.h"
#include "compositorsettings_p.h"
#include "logging_p.h"

namespace Liri {

namespace WaylandServer {

/*
 * CompositorSettingsPrivate
 */

CompositorSettingsPrivate::CompositorSettingsPrivate()
    : initialized(false)
    , compositor(nullptr)
    , windowActionKey(Qt::MetaModifier)
    , repeatRate(40)
    , repeatDelay(400)
{
}

void CompositorSettingsPrivate::_q_setupKeyboard()
{
    if (!compositor || !initialized)
        return;

    QWaylandSeat *seat = compositor->defaultSeat();
    if (seat && seat->keyboard()) {
        seat->keyboard()->setRepeatRate(repeatRate);
        seat->keyboard()->setRepeatDelay(repeatDelay);
    }
}

/*
 * CompositorSettings
 */

CompositorSettings::CompositorSettings(QObject *parent)
    : QObject(parent)
    , d_ptr(new CompositorSettingsPrivate())
{
}

CompositorSettings::~CompositorSettings()
{
    delete d_ptr;
}

QWaylandCompositor *CompositorSettings::compositor() const
{
    Q_D(const CompositorSettings);
    return d->compositor;
}

void CompositorSettings::setCompositor(QWaylandCompositor *compositor)
{
    Q_D(CompositorSettings);

    if (d->compositor == compositor)
        return;

    if (d->compositor)
        disconnect(compositor, SIGNAL(defaultSeatChanged(QWaylandSeat*,QWaylandSeat*)),
                   this, SLOT(_q_setupKeyboard()));

    if (compositor) {
        connect(compositor, SIGNAL(defaultSeatChanged(QWaylandSeat*,QWaylandSeat*)),
                this, SLOT(_q_setupKeyboard()));
        d->_q_setupKeyboard();
    }

    d->compositor = compositor;
    Q_EMIT compositorChanged();
}

Qt::KeyboardModifier CompositorSettings::windowActionKey() const
{
    Q_D(const CompositorSettings);
    return d->windowActionKey;
}

void CompositorSettings::setWindowActionKey(Qt::KeyboardModifier mod)
{
    Q_D(CompositorSettings);

    if (d->windowActionKey == mod)
        return;

    d->windowActionKey = mod;
    Q_EMIT windowActionKeyChanged();
}

quint32 CompositorSettings::keyboardRepeatRate() const
{
    Q_D(const CompositorSettings);
    return d->repeatRate;
}

void CompositorSettings::setKeyboardRepeatRate(quint32 rate)
{
    Q_D(CompositorSettings);

    if (d->repeatRate == rate)
        return;

    d->repeatRate = rate;
    Q_EMIT keyboardRepeatRateChanged();

    if (d->compositor) {
        QWaylandSeat *seat = d->compositor->defaultSeat();
        if (seat && seat->keyboard())
            seat->keyboard()->setRepeatRate(d->repeatRate);
    }
}

quint32 CompositorSettings::keyboardRepeatDelay() const
{
    Q_D(const CompositorSettings);
    return d->repeatDelay;
}

void CompositorSettings::setKeyboardRepeatDelay(quint32 delay)
{
    Q_D(CompositorSettings);

    if (d->repeatDelay == delay)
        return;

    d->repeatDelay = delay;
    Q_EMIT keyboardRepeatDelayChanged();

    if (d->compositor) {
        QWaylandSeat *seat = d->compositor->defaultSeat();
        if (seat && seat->keyboard())
            seat->keyboard()->setRepeatDelay(d->repeatDelay);
    }
}

void CompositorSettings::classBegin()
{
}

void CompositorSettings::componentComplete()
{
    Q_D(CompositorSettings);
    d->initialized = true;
    d->_q_setupKeyboard();
}

} // namespace WaylandServer

} // namespace Liri

#include "moc_compositorsettings.cpp"
