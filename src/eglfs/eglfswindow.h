/****************************************************************************
 * This file is part of Liri.
 *
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

#ifndef LIRI_EGLFSWINDOW_H
#define LIRI_EGLFSWINDOW_H

#include <QtGui/qpa/qplatformwindow.h>
#include <QtPlatformCompositorSupport/private/qopenglcompositor_p.h>

#include <LiriEglFS/EglFSScreen>

class QPlatformTextureList;
class QOpenGLCompositorBackingStore;

namespace Liri {

namespace Platform {

class LIRIEGLFS_EXPORT EglFSWindow : public QPlatformWindow, public QOpenGLCompositorWindow
{
public:
    EglFSWindow(QWindow *w);
    virtual ~EglFSWindow();

    void create();
    void destroy();

    void setGeometry(const QRect &) override;
    QRect geometry() const override;
    void setVisible(bool visible) override;
    void requestActivateWindow() override;
    void raise() override;
    void lower() override;

    void propagateSizeHints() override { }
    void setMask(const QRegion &) override { }
    bool setKeyboardGrabEnabled(bool) override { return false; }
    bool setMouseGrabEnabled(bool) override { return false; }
    void setOpacity(qreal) override;
    WId winId() const override;

    QSurfaceFormat format() const override;

    EGLNativeWindowType eglWindow() const;
    EGLSurface surface() const;
    EglFSScreen *screen() const;

    bool hasNativeWindow() const { return m_flags.testFlag(HasNativeWindow); }

    virtual void invalidateSurface() override;
    virtual void resetSurface();

    QOpenGLCompositorBackingStore *backingStore() { return m_backingStore; }
    void setBackingStore(QOpenGLCompositorBackingStore *backingStore) { m_backingStore = backingStore; }
    bool isRaster() const;

    QWindow *sourceWindow() const override;
    const QPlatformTextureList *textures() const override;
    void endCompositing() override;

protected:
    QOpenGLCompositorBackingStore *m_backingStore;
    bool m_raster;
    WId m_winId;

    EGLSurface m_surface;
    EGLNativeWindowType m_window;

    EGLConfig m_config;
    QSurfaceFormat m_format;

    enum Flag {
        Created = 0x01,
        HasNativeWindow = 0x02
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    Flags m_flags;
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_EGLFSWINDOW_H
