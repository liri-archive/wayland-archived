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

#ifndef LIRI_LIBINPUTKEYBOARD_P_H
#define LIRI_LIBINPUTKEYBOARD_P_H

#include <QtCore/private/qobject_p.h>

#include "eglfsxkb.h"
#include "libinputhandler.h"
#include "libinputkeyboard.h"

#include <libinput.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri LibInput API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Liri {

namespace Platform {

class LIRILIBINPUT_EXPORT LibInputKeyboardPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(LibInputKeyboard)
public:
    LibInputKeyboardPrivate(LibInputHandler *h);
    ~LibInputKeyboardPrivate();

    LibInputHandler *handler;

    xkb_context *context;
    xkb_keymap *keymap;
    xkb_state *state;
    xkb_mod_index_t modifiers[4];

    QTimer repeatTimer;
    quint32 repeatRate;
    quint32 repeatDelay;
    struct {
        int key;
        Qt::KeyboardModifiers modifiers;
        quint32 nativeScanCode;
        quint32 nativeVirtualKey;
        quint32 nativeModifiers;
        QString text;
        ushort repeatCount;
    } repeatData;
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_LIBINPUTKEYBOARD_P_H
