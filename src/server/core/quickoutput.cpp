/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2014-2016 Pier Luigi Fiorini
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

#include <LiriEglFS/EglFSGlobal>

#include <QtCore/QElapsedTimer>
#include <QtCore/private/qobject_p.h>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/qpa/qplatformscreen.h>
#include <QtGui/qpa/qwindowsysteminterface.h>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandOutputMode>

#include "quickoutput.h"
#include "logging_p.h"
#include "extensions/screencaster.h"
#include "screen/screenbackend.h"

namespace Liri {

namespace WaylandServer {

/*
 * OutputPrivate
 */

class QuickOutputPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QuickOutput)
public:
    QuickOutputPrivate()
        : initialized(false)
        , nativeScreen(nullptr)
        , enabled(true)
    {
    }

    bool initialized;
    Screen *nativeScreen;
    bool enabled;
    QList<QObject *> objects;
};

/*
 * Output
 */

QuickOutput::QuickOutput()
    : QWaylandQuickOutput()
    , d_ptr(new QuickOutputPrivate())
{
}

QuickOutput::QuickOutput(QWaylandCompositor *compositor)
    : QWaylandQuickOutput(compositor, nullptr)
    , d_ptr(new QuickOutputPrivate())
{
    // We cannot have multiple top level windows on the same screen
    // with our QPA plugin, hence set the screen as soon as possible
    connect(this, &QuickOutput::windowChanged, this, [this] {
        QQuickWindow *quickWindow = qobject_cast<QQuickWindow *>(window());
        if (quickWindow && d_func()->nativeScreen)
            quickWindow->setScreen(d_func()->nativeScreen->screen());
    });
}

QQmlListProperty<QObject> QuickOutput::data()
{
    Q_D(QuickOutput);
    return QQmlListProperty<QObject>(this, d->objects);
}

Screen *QuickOutput::nativeScreen() const
{
    Q_D(const QuickOutput);
    return d->nativeScreen;
}

void QuickOutput::setNativeScreen(Screen *screen)
{
    Q_D(QuickOutput);

    if (d->nativeScreen == screen)
        return;

    if (d->initialized) {
        qCWarning(gLcCore)
                << "Setting Liri::WaylandServer::Output::nativeScreen "
                   "after initialization has no effect";
        return;
    }

    d->nativeScreen = screen;
    Q_EMIT nativeScreenChanged();

    // We cannot have multiple top level windows on the same screen
    // with our QPA plugin, hence set the screen as soon as possible
    QQuickWindow *quickWindow = qobject_cast<QQuickWindow *>(window());
    if (quickWindow)
        quickWindow->setScreen(d->nativeScreen->screen());
}

bool QuickOutput::isEnabled() const
{
    Q_D(const QuickOutput);
    return d->enabled;
}

void QuickOutput::setEnabled(bool value)
{
    Q_D(QuickOutput);

    if (d->enabled == value)
        return;

    d->enabled = value;
    Q_EMIT enabledChanged();
}

QuickOutput::PowerState QuickOutput::powerState() const
{
    Q_D(const QuickOutput);

    // Power state is supported only with native screens and our QPA
    QPlatformScreen *screen = nullptr;
    if (d->nativeScreen && d->nativeScreen->screen())
        screen = static_cast<QPlatformScreen *>(
                    d->nativeScreen->screen()->handle());
    if (!screen)
        return PowerStateOn;

    return static_cast<QuickOutput::PowerState>(screen->powerState());
}

void QuickOutput::setPowerState(PowerState state)
{
    Q_D(QuickOutput);

    // Power state is supported only with native screens and our QPA
    QPlatformScreen *screen = nullptr;
    if (d->nativeScreen && d->nativeScreen->screen())
        screen = static_cast<QPlatformScreen *>(
                    d->nativeScreen->screen()->handle());
    if (!screen)
        return;

    QPlatformScreen::PowerState pstate =
            static_cast<QPlatformScreen::PowerState>(state);
    if (screen->powerState() != pstate) {
        screen->setPowerState(pstate);
        Q_EMIT powerStateChanged();
    }
}

QuickOutput *QuickOutput::fromResource(wl_resource *resource)
{
    return qobject_cast<QuickOutput *>(QWaylandOutput::fromResource(resource));
}

void QuickOutput::initialize()
{
    Q_D(QuickOutput);

    QWaylandQuickOutput::initialize();

    QQuickWindow *quickWindow = qobject_cast<QQuickWindow *>(window());
    if (!quickWindow) {
        qCWarning(gLcCore,
                  "Could not locate QQuickWindow on initializing"
                  "Liri::WaylandServer::Output %p.\n", this);
        return;
    }

    // We want to read contents for the screencaster
    connect(quickWindow, &QQuickWindow::afterRendering,
            this, &QuickOutput::readContent);

    // Add modes
    QList<Screen::Mode> modes = d->nativeScreen->modes();
    if (d->nativeScreen && modes.size() > 0 && !sizeFollowsWindow()) {
        int modeId = 0;
        Q_FOREACH (const Screen::Mode &mode, modes) {
            QWaylandOutputMode wlMode(mode.size, qRound(mode.refreshRate * 1000));
            addMode(wlMode, modeId == d->nativeScreen->preferredMode());
            modeId++;
        }

        // Set current mode
        int currentModeId = d->nativeScreen->currentMode();
        if (currentModeId < 0)
            currentModeId = 0;
        Screen::Mode currentMode = modes.at(currentModeId);
        setCurrentMode(QWaylandOutputMode(currentMode.size, qRound(currentMode.refreshRate * 1000)));
    }

    // Set the window visible now
    quickWindow->setVisible(true);

    // Center mouse cursor
    if (quickWindow->screen() == QGuiApplication::primaryScreen())
        QWindowSystemInterface::handleMouseEvent(quickWindow, geometry().center(),
                                                 geometry().center(), Qt::NoButton);

    d->initialized = true;
}

void QuickOutput::readContent()
{
    QQuickWindow *quickWindow = qobject_cast<QQuickWindow *>(window());
    if (!quickWindow)
        return;

    QWaylandCompositorExtension *e = compositor()->extension(Screencaster::interfaceName());
    Screencaster *screencaster = qobject_cast<Screencaster *>(e);
    if (screencaster && screencaster->isInitialized())
        screencaster->recordFrame(quickWindow);
}

} // namespace WaylandServer

} // namespace Liri

#include "moc_quickoutput.cpp"
