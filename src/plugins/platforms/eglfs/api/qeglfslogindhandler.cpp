/****************************************************************************
**
** Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qeglfshooks_p.h"
#include "qeglfslogindhandler_p.h"

#include <LiriLogind/Logind>

using namespace Liri;

QEglFSLogindHandler::QEglFSLogindHandler()
    : QObject()
    , m_loop(new QEventLoop(this))
{
}

void QEglFSLogindHandler::initialize()
{
    // Connect to logind and take control
    Logind *logind = Logind::instance();
    if (logind->isConnected()) {
        qCDebug(qLcEglDevDebug) << "logind connection already established";
        takeControl(true);
    } else {
        qCDebug(qLcEglDevDebug) << "logind connection not yet established";
        connect(logind, &Logind::connectedChanged,
                this, &QEglFSLogindHandler::takeControl,
                Qt::QueuedConnection);
    }

    // Wait for logind setup
    m_loop->exec();
}

void QEglFSLogindHandler::stop()
{
    m_loop->quit();
}

void QEglFSLogindHandler::takeControl(bool connected)
{
    if (!connected)
        return;

    Logind *logind = Logind::instance();

    disconnect(logind, &Logind::connectedChanged,
            this, &QEglFSLogindHandler::takeControl);

    if (logind->hasSessionControl()) {
        qCDebug(qLcEglDevDebug) << "Session control already acquired via logind";
        Q_EMIT initializationRequested();
    } else {
        qCDebug(qLcEglDevDebug) << "Take control of session via logind";
        logind->takeControl();
        connect(logind, &Logind::hasSessionControlChanged,
                this, &QEglFSLogindHandler::initializationRequested,
                Qt::QueuedConnection);
    }
}
