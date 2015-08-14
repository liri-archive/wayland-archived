/****************************************************************************
**
** Copyright (C) 2013 Klarälvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qwaylandinputpanel.h"

#include <private/qobject_p.h>

#include "qwlinputpanel_p.h"
#include "qwlsurface_p.h"

QT_BEGIN_NAMESPACE

class QWaylandInputPanelPrivate : public QObjectPrivate
{
public:
    QWaylandInputPanelPrivate(QtWayland::InputPanel *panel)
        : inputPanel(panel)
    {
    }

    QtWayland::InputPanel *inputPanel;
};


QWaylandInputPanel::QWaylandInputPanel(QtWayland::InputPanel *inputPanel)
    : QObject(*new QWaylandInputPanelPrivate(inputPanel))
{
}

QtWayland::InputPanel *QWaylandInputPanel::handle() const
{
    Q_D(const QWaylandInputPanel);

    return d->inputPanel;
}

QWaylandSurface *QWaylandInputPanel::focus() const
{
    Q_D(const QWaylandInputPanel);

    QtWayland::Surface *surface = d->inputPanel->focus();
    if (surface)
        return surface->waylandSurface();
    return 0;
}

bool QWaylandInputPanel::visible() const
{
    Q_D(const QWaylandInputPanel);

    return d->inputPanel->inputPanelVisible();
}

QRect QWaylandInputPanel::cursorRectangle() const
{
    Q_D(const QWaylandInputPanel);

    return d->inputPanel->cursorRectangle();
}

QT_END_NAMESPACE
