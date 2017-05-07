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

    void setGeometry(const QRect &) Q_DECL_OVERRIDE;
    QRect geometry() const Q_DECL_OVERRIDE;
    void setVisible(bool visible) Q_DECL_OVERRIDE;
    void requestActivateWindow() Q_DECL_OVERRIDE;
    void raise() Q_DECL_OVERRIDE;
    void lower() Q_DECL_OVERRIDE;

    void propagateSizeHints() Q_DECL_OVERRIDE { }
    void setMask(const QRegion &) Q_DECL_OVERRIDE { }
    bool setKeyboardGrabEnabled(bool) Q_DECL_OVERRIDE { return false; }
    bool setMouseGrabEnabled(bool) Q_DECL_OVERRIDE { return false; }
    void setOpacity(qreal) Q_DECL_OVERRIDE;
    WId winId() const Q_DECL_OVERRIDE;

    QSurfaceFormat format() const Q_DECL_OVERRIDE;

    EGLNativeWindowType eglWindow() const;
    EGLSurface surface() const;
    EglFSScreen *screen() const;

    bool hasNativeWindow() const { return m_flags.testFlag(HasNativeWindow); }

    virtual void invalidateSurface() Q_DECL_OVERRIDE;
    virtual void resetSurface();

    QOpenGLCompositorBackingStore *backingStore() { return m_backingStore; }
    void setBackingStore(QOpenGLCompositorBackingStore *backingStore) { m_backingStore = backingStore; }
    bool isRaster() const;

    QWindow *sourceWindow() const Q_DECL_OVERRIDE;
    const QPlatformTextureList *textures() const Q_DECL_OVERRIDE;
    void endCompositing() Q_DECL_OVERRIDE;

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
