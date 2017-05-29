/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#ifndef XCB_PROPERTIES_H
#define XCB_PROPERTIES_H

#include <xcb/xcb.h>

namespace Xcb {

namespace Properties {

#if 0
void dump(xcb_atom_t property, xcb_get_property_reply_t *reply);
#endif
void dumpProperty(xcb_atom_t property, xcb_get_property_reply_t *reply);
void readAndDumpProperty(xcb_atom_t atom, xcb_window_t window);

} // namespace Cursors

} // namespace Xcb

#endif // XCB_PROPERTIES_H
