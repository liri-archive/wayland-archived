/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (C) 2015 The Qt Company Ltd.
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

#include <QtCore/QSet>
#include <QtCore/QSysInfo>
#include <QtCore/QTextStream>
#include <QtCore/private/qsimd_p.h>
#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QWindow>
#include <QtGui/qpa/qplatformnativeinterface.h>

#include "diagnostic_p.h"

#include <wayland-version.h>

#define DUMP_CPU_FEATURE(feature, name)  \
    if (qCpuHasFeature(feature)) str << " " name;

extern char **environ;

namespace Liri {

namespace DiagnosticOutput {

static QByteArray wordWrap(QByteArray string, int length)
{
    QByteArray::iterator it = string.begin();
    QByteArray::iterator lastSpace = string.begin();
    int distance = 0;

    while (it != string.end()) {
        while (it != string.end() && distance <= length) {
            distance++;

            if (*it == ' ') {
                lastSpace = it;
                if (length == distance)
                    *lastSpace = '\n';
            }

            ++it;
        }

        if (lastSpace != string.begin())
            *lastSpace = '\n';

        lastSpace = string.begin();
        distance = 0;
    }

    return string;
}

QString systemInformation()
{
    QString result;
    QTextStream str(&result);

    str << "OS: " << QSysInfo::prettyProductName();
    str << " ["
        << QSysInfo::kernelType()
        << " version " << QSysInfo::kernelVersion()
        << "]\n";
    str << "Architecture: " << QSysInfo::currentCpuArchitecture() << "; ";
    str << "features:";
#if defined(Q_PROCESSOR_X86)
    DUMP_CPU_FEATURE(SSE2, "SSE2");
    DUMP_CPU_FEATURE(SSE3, "SSE3");
    DUMP_CPU_FEATURE(SSSE3, "SSSE3");
    DUMP_CPU_FEATURE(SSE4_1, "SSE4.1");
    DUMP_CPU_FEATURE(SSE4_2, "SSE4.2");
    DUMP_CPU_FEATURE(AVX, "AVX");
    DUMP_CPU_FEATURE(AVX2, "AVX2");
    DUMP_CPU_FEATURE(RTM, "RTM");
    DUMP_CPU_FEATURE(HLE, "HLE");
#elif defined(Q_PROCESSOR_ARM)
    DUMP_CPU_FEATURE(ARM_NEON, "Neon");
#elif defined(Q_PROCESSOR_MIPS)
    DUMP_CPU_FEATURE(DSP, "DSP");
    DUMP_CPU_FEATURE(DSPR2, "DSPR2");
#endif
    str << '\n';
    return result;
}

#ifndef QT_NO_OPENGL
QTextStream &operator<<(QTextStream &str, const QSurfaceFormat &format)
{
    str << "Version: " << format.majorVersion() << '.'
        << format.minorVersion() << " Profile: " << format.profile()
        << " Swap behavior: " << format.swapBehavior()
        << " Buffer size (RGB";
    if (format.hasAlpha())
        str << 'A';
    str << "): " << format.redBufferSize() << ',' << format.greenBufferSize()
        << ',' << format.blueBufferSize();
    if (format.hasAlpha())
        str << ',' << format.alphaBufferSize();
    if (const int dbs = format.depthBufferSize())
        str << " Depth buffer: " << dbs;
    if (const int sbs = format.stencilBufferSize())
        str << " Stencil buffer: " << sbs;
    const int samples = format.samples();
    if (samples > 0)
        str << " Samples: " << samples;
    return str;
}
#endif

QString openGlContext()
{
    QString result;
    QTextStream str(&result);

    QOpenGLContext context;
    if (context.create()) {
#  ifdef QT_OPENGL_DYNAMIC
        str << "Dynamic GL ";
#  endif
        switch (context.openGLModuleType()) {
        case QOpenGLContext::LibGL:
            str << "LibGL";
            break;
        case QOpenGLContext::LibGLES:
            str << "LibGLES";
            break;
        }

        QWindow window;
        if (QGuiApplication::platformName() == QLatin1String("liri"))
            window.setFlags(Qt::Desktop);
        window.setSurfaceType(QSurface::OpenGLSurface);
        //window.setScreen(QGuiApplication::primaryScreen());
        window.create();
        if (context.makeCurrent(&window)) {
            QOpenGLFunctions functions(&context);

            str << " Vendor: " << reinterpret_cast<const char *>(functions.glGetString(GL_VENDOR))
                << "\nRenderer: " << reinterpret_cast<const char *>(functions.glGetString(GL_RENDERER))
                << "\nVersion: " << reinterpret_cast<const char *>(functions.glGetString(GL_VERSION))
                << "\nGLSL version: " << reinterpret_cast<const char *>(functions.glGetString(GL_SHADING_LANGUAGE_VERSION))
                << "\nFormat: " << context.format();

            QList<QByteArray> extensionList = context.extensions().toList();
            std::sort(extensionList.begin(), extensionList.end());
            QByteArray extensions = extensionList.join(' ');
            str << " \nFound " << extensionList.size() << " extensions:\n";
            str << wordWrap(extensions, 78);

            context.doneCurrent();
        }
        window.destroy();
    } else {
        str << "Unable to create an Open GL context.\n";
    }

    return result;
}

QString framework()
{
    QString result;
    QTextStream str(&result);

    str << "Wayland version: " << WAYLAND_VERSION
        << "\nQt version: " << QT_VERSION_STR
        << "\nLiri Wayland version: " << LIRIWAYLAND_VERSION
        << '\n';

    return result;
}

QString environment()
{
    QString result;
    QTextStream str(&result);

    // Environment variables
    str << "Environment variables:\n";
    for (char **current = environ; *current; current++) {
        if (::strncmp(*current, "QT", 2) == 0 ||
                ::strncmp(*current, "QML", 3) == 0 ||
                ::strncmp(*current, "QSG", 3) == 0 ||
                ::strncmp(*current, "XDG", 3) == 0)
            str << "  " << *current << '\n';
    }

    return result;
}

} // namespace DiagnosticOutput

} // namespace Liri
