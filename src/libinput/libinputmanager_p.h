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

#ifndef LIRI_LIBINPUTMANAGER_P_H
#define LIRI_LIBINPUTMANAGER_P_H

#include <QtCore/QObject>

#include <LiriLibInput/lirilibinputglobal.h>

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

class LibInputHandler;

class LIRILIBINPUT_EXPORT LibInputManager : public QObject
{
    Q_OBJECT
public:
    explicit LibInputManager(QObject *parent = 0);

    LibInputHandler *handler() const;

private:
    LibInputHandler *m_handler;
};

} // namespace Platform

} // namespace Liri

#endif // LIRI_LIBINPUTMANAGER_P_H