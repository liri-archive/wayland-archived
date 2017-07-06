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

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>

#include "diagnostic_p.h"
#include "homeapplication.h"
#include "homeapplication_p.h"
#include "logging_p.h"

#include <unistd.h>
#include <sys/types.h>

namespace Liri {

namespace WaylandServer {

/*
 * HomeApplicationPrivate
 */

void HomeApplicationPrivate::verifyXdgRuntimeDir()
{
    QByteArray dirName = qgetenv("XDG_RUNTIME_DIR");

    if (qEnvironmentVariableIsEmpty("XDG_RUNTIME_DIR")) {
        QString msg = QObject::tr(
                    "The XDG_RUNTIME_DIR environment variable is not set.\n"
                    "Refer to your distribution on how to get it, or read\n"
                    "http://www.freedesktop.org/wiki/Specifications/basedir-spec\n"
                    "on how to implement it.\n");
        qFatal("%s", qPrintable(msg));
    }

    QFileInfo fileInfo(dirName);

    if (!fileInfo.exists()) {
        QString msg = QObject::tr(
                    "The XDG_RUNTIME_DIR environment variable is set to "
                    "\"%1\", which doesn't exist.\n").arg(dirName.constData());
        qFatal("%s", qPrintable(msg));
    }

    if (convertPermission(fileInfo) != 700 || fileInfo.ownerId() != getuid()) {
        QString msg = QObject::tr(
                    "XDG_RUNTIME_DIR is set to \"%1\" and is not configured correctly.\n"
                    "Unix access mode must be 0700, but is 0%2.\n"
                    "It must also be owned by the current user (UID %3), "
                    "but is owned by UID %4 (\"%5\").\n")
                .arg(dirName.constData())
                .arg(convertPermission(fileInfo))
                .arg(getuid())
                .arg(fileInfo.ownerId())
                .arg(fileInfo.owner());
        qFatal("%s\n", qPrintable(msg));
    }
}

/*
 * HomeApplication
 */

HomeApplication::HomeApplication(QObject *parent)
    : QObject(*new HomeApplicationPrivate(), parent)
{
    Q_D(HomeApplication);

    // Diagnostic output
    qInfo("%s", qPrintable(DiagnosticOutput::systemInformation()));
    qInfo("%s", qPrintable(DiagnosticOutput::openGlContext()));
    qInfo("%s", qPrintable(DiagnosticOutput::framework()));
    qInfo("%s", qPrintable(DiagnosticOutput::environment()));

    // Reroute the signal
    d->engine = new QQmlApplicationEngine(this);
    connect(d->engine, &QQmlApplicationEngine::objectCreated,
            this, &HomeApplication::objectCreated);
}

QString HomeApplication::screenConfiguration() const
{
    Q_D(const HomeApplication);
    return d->screenConfiguration;
}

void HomeApplication::setScreenConfiguration(const QString &fileName)
{
    Q_D(HomeApplication);

    if (d->screenConfiguration == fileName)
        return;

    if (d->initialized) {
        qCWarning(gLcCore) << "Setting HomeApplication::screenConfiguration after initialization has no effect";
        return;
    }

    d->screenConfiguration = fileName;
    Q_EMIT screenConfigurationChanged(fileName);

    QString backendName = d->screenConfiguration.isEmpty()
            ? QStringLiteral("native")
            : QStringLiteral("fake");
    QCoreApplication::instance()->setProperty("__liri_screen_backend", backendName);
    if (!d->screenConfiguration.isEmpty())
        QCoreApplication::instance()->setProperty("__liri_screen_configuration", d->screenConfiguration);
}

QList<QObject *> HomeApplication::rootObjects() const
{
    Q_D(const HomeApplication);
    return d->engine->rootObjects();
}

void HomeApplication::setContextProperty(const QString &name, const QVariant &value)
{
    Q_D(HomeApplication);

    d->engine->rootContext()->setContextProperty(name, value);
    Q_EMIT contextPropertyChanged(name, value);
}

void HomeApplication::setContextProperty(const QString &name, QObject *object)
{
    Q_D(HomeApplication);

    d->engine->rootContext()->setContextProperty(name, object);
    Q_EMIT contextPropertyChanged(name, object);
}

bool HomeApplication::loadUrl(const QUrl &url)
{
    Q_D(HomeApplication);

    // Do not run if already running
    if (d->running) {
        qCWarning(gLcCore) << "Compositor already running";
        return false;
    }

    // Check whether XDG_RUNTIME_DIR is ok or not
    d->verifyXdgRuntimeDir();

    qCDebug(gLcCore) << "Loading" << url.toString();

    d->engine->load(url);
    d->running = true;

    return true;
}

} // namespace WaylandServer

} // namespace Liri

#include "moc_homeapplication.cpp"
