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

#ifndef LIRI_SCREENBACKEND_H
#define LIRI_SCREENBACKEND_H

#include <QtCore/QObject>
#include <QtCore/QSize>

#include <QtWaylandCompositor/QWaylandOutput>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class QScreen;

namespace Liri {

namespace WaylandServer {

class OutputChangeset;
class ScreenPrivate;
class ScreenBackendPrivate;

class LIRIWAYLANDSERVER_EXPORT Screen : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Screen)
    Q_PROPERTY(QScreen *screen READ screen CONSTANT)
    Q_PROPERTY(QString manufacturer READ manufacturer CONSTANT)
    Q_PROPERTY(QString model READ model CONSTANT)
    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(int width READ width NOTIFY sizeChanged)
    Q_PROPERTY(int height READ height NOTIFY sizeChanged)
    Q_PROPERTY(int refreshRate READ refreshRate NOTIFY refreshRateChanged)
    Q_PROPERTY(QSizeF physicalSize READ physicalSize NOTIFY physicalSizeChanged)
    Q_PROPERTY(QWaylandOutput::Subpixel subpixel READ subpixel NOTIFY subpixelChanged)
    Q_PROPERTY(QWaylandOutput::Transform transform READ transform NOTIFY transformChanged)
    Q_PROPERTY(int scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(int currentMode READ currentMode NOTIFY currentModeChanged)
    Q_PROPERTY(int preferredMode READ preferredMode NOTIFY preferredModeChanged)
public:
    struct Mode {
        QSize size;
        qreal refreshRate;
    };

    Screen(QObject *parent = nullptr);

    QScreen *screen() const;

    QString manufacturer() const;
    QString model() const;

    QPoint position() const;
    QSize size() const;
    int width() const;
    int height() const;
    int refreshRate() const;
    QSizeF physicalSize() const;
    QWaylandOutput::Subpixel subpixel() const;
    QWaylandOutput::Transform transform() const;
    int scaleFactor() const;

    int currentMode() const;
    int preferredMode() const;
    QList<Mode> modes() const;

    Q_INVOKABLE bool applyChangeset(Liri::WaylandServer::OutputChangeset *changeset);
    Q_INVOKABLE void discardChangeset(Liri::WaylandServer::OutputChangeset *changeset);

    static ScreenPrivate *get(Screen *screen) { return screen->d_func(); }

Q_SIGNALS:
    void positionChanged();
    void sizeChanged();
    void refreshRateChanged();
    void physicalSizeChanged();
    void subpixelChanged();
    void transformChanged();
    void scaleFactorChanged();
    void currentModeChanged();
    void preferredModeChanged();
    void modesChanged();
};

class LIRIWAYLANDSERVER_EXPORT ScreenBackend : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ScreenBackend)
public:
    ScreenBackend(QObject *parent = nullptr);
    virtual ~ScreenBackend();

    QList<Screen *> screens() const;

    virtual void acquireConfiguration() = 0;

    static ScreenBackendPrivate *get(ScreenBackend *backend) { return backend->d_func(); }

Q_SIGNALS:
    void screenAdded(Screen *screen);
    void screenRemoved(Screen *screen);
    void primaryScreenChanged(Screen *screen);
};

} // namespace WaylandServer

} // namespace Liri

#endif // LIRI_SCREENBACKEND_H
