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
    : QObject(*new CompositorSettingsPrivate(), parent)
{
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
