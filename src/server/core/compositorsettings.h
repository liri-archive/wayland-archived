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

#ifndef LIRI_COMPOSITORSETTINGS_H
#define LIRI_COMPOSITORSETTINGS_H

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class QWaylandCompositor;

namespace Liri {

namespace WaylandServer {

class CompositorSettingsPrivate;

class LIRIWAYLANDSERVER_EXPORT CompositorSettings : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_DECLARE_PRIVATE(CompositorSettings)
    Q_PROPERTY(QWaylandCompositor *compositor READ compositor WRITE setCompositor NOTIFY compositorChanged)
    Q_PROPERTY(Qt::KeyboardModifier windowActionKey READ windowActionKey WRITE setWindowActionKey NOTIFY windowActionKeyChanged)
    Q_PROPERTY(quint32 keyboardRepeatRate READ keyboardRepeatRate WRITE setKeyboardRepeatRate NOTIFY keyboardRepeatRateChanged)
    Q_PROPERTY(quint32 keyboardRepeatDelay READ keyboardRepeatDelay WRITE setKeyboardRepeatDelay NOTIFY keyboardRepeatDelayChanged)
public:
    CompositorSettings(QObject *parent = nullptr);

    QWaylandCompositor *compositor() const;
    void setCompositor(QWaylandCompositor *compositor);

    Qt::KeyboardModifier windowActionKey() const;
    void setWindowActionKey(Qt::KeyboardModifier mod);

    quint32 keyboardRepeatRate() const;
    void setKeyboardRepeatRate(quint32 rate);

    quint32 keyboardRepeatDelay() const;
    void setKeyboardRepeatDelay(quint32 delay);

Q_SIGNALS:
    void compositorChanged();
    void windowActionKeyChanged();
    void keyboardRepeatRateChanged();
    void keyboardRepeatDelayChanged();

protected:
    void classBegin() override;
    void componentComplete() override;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_setupKeyboard())
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_COMPOSITORSETTINGS_H
