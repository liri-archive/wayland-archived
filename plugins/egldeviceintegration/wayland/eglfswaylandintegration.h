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

#ifndef LIRI_EGLFSWAYLANDINTEGRATION_H
#define LIRI_EGLFSWAYLANDINTEGRATION_H

#include <QtGui/qpa/qplatformwindow.h>

#include <Liri/WaylandClient/ClientConnection>
#include <Liri/WaylandClient/Compositor>
#include <Liri/WaylandClient/FullScreenShell>
#include <Liri/WaylandClient/Keyboard>
#include <Liri/WaylandClient/Pointer>
#include <Liri/WaylandClient/Registry>
#include <Liri/WaylandClient/Seat>
#include <Liri/WaylandClient/Touch>

#include <Liri/Platform/EGLDeviceIntegration>

#include "eglfswaylandinput.h"

namespace Liri {

namespace Platform {

class EglFSWaylandScreen;

class EglFSWaylandIntegration : public QObject, public EGLDeviceIntegration
{
    Q_OBJECT
public:
    EglFSWaylandIntegration();

    void platformInit() Q_DECL_OVERRIDE;
    void platformDestroy() Q_DECL_OVERRIDE;

    EGLNativeDisplayType platformDisplay() const Q_DECL_OVERRIDE;

    bool handlesInput() Q_DECL_OVERRIDE;
    bool usesVtHandler() Q_DECL_OVERRIDE;
    bool usesDefaultScreen() Q_DECL_OVERRIDE;

    void screenInit() Q_DECL_OVERRIDE;

    QPlatformWindow *createPlatformWindow(QWindow *window) Q_DECL_OVERRIDE;
    QPlatformOpenGLContext *createPlatformOpenGLContext(QOpenGLContext *context) Q_DECL_OVERRIDE;

    bool hasCapability(QPlatformIntegration::Capability cap) const Q_DECL_OVERRIDE;

    WaylandClient::Compositor *compositor() const { return m_compositor; }
    WaylandClient::FullScreenShell *fullScreenShell() const { return m_fullScreenShell; }

private:
    QThread *m_thread;
    WaylandClient::ClientConnection *m_connection;
    WaylandClient::Registry *m_registry;
    WaylandClient::Compositor *m_compositor;
    WaylandClient::FullScreenShell *m_fullScreenShell;
    WaylandClient::Seat *m_seat;
    QVector<WaylandClient::Output *> m_outputs;
    QVector<EglFSWaylandScreen *> m_screens;
    EglFSWaylandInput *m_input;
    QTouchDevice *m_touchDevice;

private Q_SLOTS:
    void keyboardAdded();
    void pointerAdded();
    void touchAdded();
    void touchRemoved();
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_EGLFSWAYLANDINTEGRATION_H
