/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 * Copyright (C) 2016 The Qt Company Ltd.
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:QTLGPL$
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General
 * Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPLv3 included in the
 * packaging of this file. Please review the following information to
 * ensure the GNU Lesser General Public License version 3 requirements
 * will be met: https://www.gnu.org/licenses/lgpl.html.
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 2.0 or (at your option) the GNU General
 * Public license version 3 or any later version approved by the KDE Free
 * Qt Foundation. The licenses are as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPLv2 and LICENSE.GPLv3
 * included in the packaging of this file. Please review the following
 * information to ensure the GNU General Public License requirements will
 * be met: https://www.gnu.org/licenses/gpl-2.0.html and
 * https://www.gnu.org/licenses/gpl-3.0.html.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRI_EGLFSKMSINTEGRATION_H
#define LIRI_EGLFSKMSINTEGRATION_H

#include <QtCore/QMap>
#include <QtCore/QVariant>

#include <LiriEglFS/EglDeviceIntegration>

namespace Liri {

namespace Platform {

class Udev;
class UdevMonitor;
class EglFSKmsDevice;

class EglFSKmsIntegration : public QObject, public EGLDeviceIntegration
{
    Q_OBJECT
public:
    EglFSKmsIntegration();

    void platformInit() override;
    void platformDestroy() override;
    void loadConfiguration(const QString &fileName) override;
    void saveConfiguration(const QString &fileName) override;
    bool needsLogind() override;
    EGLNativeDisplayType platformDisplay() const override;
    bool usesDefaultScreen() override;
    void screenInit() override;
    QSurfaceFormat surfaceFormatFor(const QSurfaceFormat &inputFormat) const override;
    QPlatformWindow *createPlatformWindow(QWindow *window) override;
    EGLNativeWindowType createNativeWindow(QPlatformWindow *platformWindow,
                                           const QSize &size,
                                           const QSurfaceFormat &format) override;
    EGLNativeWindowType createNativeOffscreenWindow(const QSurfaceFormat &format) override;
    void destroyNativeWindow(EGLNativeWindowType window) override;
    bool hasCapability(QPlatformIntegration::Capability cap) const override;
    QPlatformCursor *createCursor(QPlatformScreen *screen) const override;
    bool isResizingSurface(QPlatformSurface *surface) const override;
    void waitForVSync(QPlatformSurface *surface) const override;
    void resizeSurface(QPlatformSurface *surface) override;
    void presentBuffer(QPlatformSurface *surface) override;
    bool supportsPBuffers() const override;

    bool hwCursor() const;
    bool separateScreens() const;
    QMap<QString, QVariantMap> outputSettings() const;

private:
    EglFSKmsDevice *m_device;
    bool m_hwCursor;
    bool m_pbuffers;
    bool m_separateScreens;
    QString m_devicePath;
    QMap<QString, QVariantMap> m_outputSettings;
    Udev *m_udev;
    UdevMonitor *m_udevMonitor;
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_EGLFSKMSINTEGRATION_H
