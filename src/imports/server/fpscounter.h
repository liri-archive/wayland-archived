/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <QtCore/QTimer>
#include <QtQuick/QQuickItem>

class FpsCounter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(unsigned int fps READ fps NOTIFY fpsChanged)
public:
    explicit FpsCounter(QQuickItem *parent = 0);

    unsigned int fps() const;

Q_SIGNALS:
    void fpsChanged();

private Q_SLOTS:
    void setup();
    void frameSwapped();
    void updateFps();

private:
    unsigned int m_counter;
    unsigned int m_fps;
    QTimer m_timer;
};

#endif // FPSCOUNTER_H
