/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2015-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

/*
 * This code takes inspiration from the xwayland module from Weston,
 * which is:
 *
 * Copyright (C) 2011 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <QtWaylandCompositor/QWaylandCompositor>

#include "xcbwrapper.h"
#include "xcbproperties.h"
#include "xcbresources.h"
#include "xwayland.h"
#include "xwaylandmanager.h"
#include "xwaylandshellsurface.h"
#include "xwaylandquickshellintegration.h"

#define TYPE_WM_PROTOCOLS       XCB_ATOM_CUT_BUFFER0
#define TYPE_MOTIF_WM_HINTS     XCB_ATOM_CUT_BUFFER1
#define TYPE_NET_WM_STATE       XCB_ATOM_CUT_BUFFER2
#define TYPE_WM_NORMAL_HINTS    XCB_ATOM_CUT_BUFFER3

#define MWM_DECOR_EVERYTHING \
        (MWM_DECOR_BORDER | MWM_DECOR_RESIZEH | MWM_DECOR_TITLE | \
         MWM_DECOR_MENU | MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE)

XWaylandShellSurface::XWaylandShellSurface(QObject *parent)
    : QObject(parent)
    , m_wm(nullptr)
    , m_window(XCB_WINDOW_NONE)
    , m_propsDirty(true)
    , m_overrideRedirect(false)
    , m_transientFor(nullptr)
    , m_windowType(Qt::WindowType::Window)
    , m_wmWindowType(ToplevelWindow)
    , m_surfaceId(0)
    , m_surface(nullptr)
    , m_wmState(WithdrawnState)
    , m_workspace(0)
    , m_activated(false)
    , m_decorate(true)
    , m_maximized(false)
    , m_fullscreen(false)
    , m_moving(false)
    , m_resizing(false)
{
}

XWaylandShellSurface::~XWaylandShellSurface()
{
    if (m_wm)
        m_wm->removeWindow(m_window);
}

void XWaylandShellSurface::initialize(XWaylandManager *wm, quint32 window,
                                      const QRect &geometry, bool overrideRedirect,
                                      XWaylandShellSurface *parentShellSurface)
{
    m_wm = wm;
    m_window = static_cast<xcb_window_t>(window);
    m_geometry = geometry;
    m_overrideRedirect = overrideRedirect;

    m_properties.deleteWindow = 0;

    xcb_get_geometry_cookie_t cookie =
            xcb_get_geometry(Xcb::connection(), m_window);

    quint32 values[1];
    values[0] = XCB_EVENT_MASK_PROPERTY_CHANGE;
    xcb_change_window_attributes(Xcb::connection(), m_window,
                                 XCB_CW_EVENT_MASK, values);

    xcb_get_geometry_reply_t *reply =
            xcb_get_geometry_reply(Xcb::connection(), cookie, nullptr);
    if (reply)
        m_hasAlpha = reply->depth == 32;
    else
        m_hasAlpha = false;
    free(reply);

    Q_EMIT xChanged();
    Q_EMIT yChanged();

    m_transientFor = parentShellSurface;
    Q_EMIT parentSurfaceChanged();
    if (m_transientFor) {
        m_windowType = Qt::SubWindow;
        Q_EMIT windowTypeChanged();
    }

    m_wm->addWindow(m_window, this);
}

Qt::WindowType XWaylandShellSurface::windowType() const
{
    return m_windowType;
}

XWaylandShellSurface::WmWindowType XWaylandShellSurface::wmWindowType() const
{
    return m_wmWindowType;
}

quint32 XWaylandShellSurface::surfaceId() const
{
    return m_surfaceId;
}

void XWaylandShellSurface::setSurfaceId(quint32 id)
{
    m_surfaceId = id;
}

QWaylandSurface *XWaylandShellSurface::surface() const
{
    return m_surface;
}

void XWaylandShellSurface::setSurface(QWaylandSurface *surface)
{
    if (m_surface) {
        disconnect(m_wm->compositor(), &QWaylandCompositor::defaultSeatChanged,
                this, &XWaylandShellSurface::handleSeatChanged);
        disconnect(m_surface, &QWaylandSurface::surfaceDestroyed,
                   this, &XWaylandShellSurface::handleSurfaceDestroyed);
    }

    m_surface = surface;
    Q_EMIT surfaceChanged();

    if (m_surface) {
        connect(m_surface, &QWaylandSurface::surfaceDestroyed,
                this, &XWaylandShellSurface::handleSurfaceDestroyed);

        qCDebug(XWAYLAND) << "Assign surface" << surface << "to shell surface for" << m_window;

        readProperties();
        Q_EMIT m_wm->shellSurfaceCreated(this);

        handleSeatChanged(m_wm->compositor()->defaultSeat(), nullptr);
        connect(m_wm->compositor(), &QWaylandCompositor::defaultSeatChanged,
                this, &XWaylandShellSurface::handleSeatChanged);

        Q_EMIT mapped();
    } else {
        qCDebug(XWAYLAND) << "Unassign surface to shell surface for" << m_window;
        Q_EMIT unmapped();
        m_wm->removeWindow(m_window);
    }
}

XWaylandShellSurface *XWaylandShellSurface::parentSurface() const
{
    return m_transientFor;
}

bool XWaylandShellSurface::isActivated() const
{
    return m_activated;
}

QString XWaylandShellSurface::appId() const
{
    return m_properties.appId;
}

QString XWaylandShellSurface::title() const
{
    return m_properties.title;
}

bool XWaylandShellSurface::decorate() const
{
    return m_decorate;
}

bool XWaylandShellSurface::overrideRedirect() const
{
    return m_overrideRedirect;
}

QPoint XWaylandShellSurface::position() const
{
    return m_geometry.topLeft();
}

QRect XWaylandShellSurface::geometry() const
{
    return m_geometry;
}

void XWaylandShellSurface::setGeometry(const QRect &geometry)
{
    if (m_geometry == geometry)
        return;

    m_geometry = geometry;
    Q_EMIT xChanged();
    Q_EMIT yChanged();
}

int XWaylandShellSurface::x() const
{
    return m_geometry.topLeft().x();
}

int XWaylandShellSurface::y() const
{
    return m_geometry.topLeft().y();
}

bool XWaylandShellSurface::isMaximized() const
{
    return m_maximized;
}

bool XWaylandShellSurface::isFullscreen() const
{
    return m_fullscreen;
}

void XWaylandShellSurface::setWmState(WmState state)
{
    quint32 property[2];
    property[0] = state;
    property[1] = XCB_WINDOW_NONE;

    xcb_change_property(Xcb::connection(),
                        XCB_PROP_MODE_REPLACE,
                        m_window,
                        Xcb::resources()->atoms->wm_state,
                        Xcb::resources()->atoms->wm_state,
                        32, 2, property);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::setNetWmState()
{
    quint32 property[3];
    quint32 i = 0;

    if (m_fullscreen)
        property[i++] = Xcb::resources()->atoms->net_wm_state_fullscreen;
    if (m_maximized) {
        property[i++] = Xcb::resources()->atoms->net_wm_state_maximized_horz;
        property[i++] = Xcb::resources()->atoms->net_wm_state_maximized_vert;
    }

    xcb_change_property(Xcb::connection(),
                        XCB_PROP_MODE_REPLACE,
                        m_window,
                        Xcb::resources()->atoms->net_wm_state,
                        XCB_ATOM_ATOM,
                        32, i, property);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::setWorkspace(int workspace)
{
    // Passing a workspace < 0 deletes the property
    if (workspace >= 0) {
        xcb_change_property(Xcb::connection(),
                            XCB_PROP_MODE_REPLACE,
                            m_window,
                            Xcb::resources()->atoms->net_wm_desktop,
                            XCB_ATOM_CARDINAL,
                            31, 1, &workspace);
    } else {
        xcb_delete_property(Xcb::connection(),
                            m_window, Xcb::resources()->atoms->net_wm_desktop);
    }
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::dirtyProperties()
{
    m_propsDirty = true;
}

void XWaylandShellSurface::readProperties()
{
    if (!m_propsDirty)
        return;
    m_propsDirty = false;

    QMap<xcb_atom_t, xcb_atom_t> props;
    props[XCB_ATOM_WM_CLASS] = XCB_ATOM_STRING;
    props[XCB_ATOM_WM_NAME] = XCB_ATOM_STRING;
    props[XCB_ATOM_WM_TRANSIENT_FOR] = XCB_ATOM_WINDOW;
    props[Xcb::resources()->atoms->wm_protocols] = TYPE_WM_PROTOCOLS;
    props[Xcb::resources()->atoms->wm_normal_hints] = TYPE_WM_NORMAL_HINTS;
    props[Xcb::resources()->atoms->net_wm_state] = TYPE_NET_WM_STATE;
    props[Xcb::resources()->atoms->net_wm_window_type] = XCB_ATOM_ATOM;
    props[Xcb::resources()->atoms->net_wm_name] = XCB_ATOM_STRING;
    props[Xcb::resources()->atoms->motif_wm_hints] = TYPE_MOTIF_WM_HINTS;

    QMap<xcb_atom_t, xcb_get_property_cookie_t> cookies;
    Q_FOREACH (xcb_atom_t atom, props.keys()) {
        xcb_get_property_cookie_t cookie = xcb_get_property(
                    Xcb::connection(), 0, m_window, atom, XCB_ATOM_ANY, 0, 2048);
        cookies[atom] = cookie;
    }

    m_sizeHints.flags = 0;
    m_motifHints.flags = 0;
    m_properties.deleteWindow = 0;

    if (m_overrideRedirect) {
        m_decorate = false;
        Q_EMIT decorateChanged();
    }

    qCDebug(XWAYLAND_TRACE) << "Properties:";

    Q_FOREACH (xcb_atom_t atom, props.keys()) {
        xcb_get_property_reply_t *reply =
                xcb_get_property_reply(Xcb::connection(), cookies[atom], nullptr);
        if (!reply)
            // Bad window, usually
            continue;
        if (reply->type == XCB_ATOM_NONE) {
            // No such property
            free(reply);
            continue;
        }

        Xcb::Properties::dumpProperty(atom, reply);

        switch (props[atom]) {
        case XCB_ATOM_STRING: {
            char *p = strndup(reinterpret_cast<char *>(xcb_get_property_value(reply)),
                              xcb_get_property_value_length(reply));
            if (atom == XCB_ATOM_WM_CLASS) {
                m_properties.appId = QString::fromUtf8(p);
                Q_EMIT appIdChanged();
            } else if (atom == XCB_ATOM_WM_NAME || Xcb::resources()->atoms->net_wm_name) {
                m_properties.title = QString::fromUtf8(p);
                Q_EMIT titleChanged();
            } else {
                free(p);
            }
            break;
        }
        case XCB_ATOM_WINDOW: {
            xcb_window_t *xid = reinterpret_cast<xcb_window_t *>(xcb_get_property_value(reply));
            XWaylandShellSurface *shellSurface = m_wm->shellSurfaceFromId(*xid);
            if (shellSurface) {
                m_transientFor = shellSurface;
                m_windowType = Qt::SubWindow;
                Q_EMIT parentSurfaceChanged();
                Q_EMIT windowTypeChanged();
            }
            break;
        }
        case XCB_ATOM_ATOM: {
            if (atom == Xcb::resources()->atoms->net_wm_window_type) {
                xcb_atom_t *atoms = static_cast<xcb_atom_t *>(xcb_get_property_value(reply));
                for (quint32 i = 0; i < reply->value_len; ++i) {
                    // Set Popup window type unless we already know this is a SubWindow
                    if (!m_transientFor) {
                        if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_tooltip ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_utility ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_dnd ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_dropdown ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_menu ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_notification ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_popup ||
                                atoms[i] == Xcb::resources()->atoms->net_wm_window_type_combo) {
                            m_windowType = Qt::Popup;
                            Q_EMIT windowTypeChanged();
                        }
                    }

                    // Save XWayland window type
                    WmWindowType wmWindowType;
                    if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_tooltip)
                        wmWindowType = TooltipWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_utility)
                        wmWindowType = UtilityWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_dnd)
                        wmWindowType = DndWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_dropdown)
                        wmWindowType = DropdownWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_menu)
                        wmWindowType = MenuWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_notification)
                        wmWindowType = NotificationWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_popup)
                        wmWindowType = PopupWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_combo)
                        wmWindowType = ComboWindow;
                    else if (atoms[i] == Xcb::resources()->atoms->net_wm_window_type_splash)
                        wmWindowType = SplashWindow;
                    else
                        wmWindowType = ToplevelWindow;
                    if (wmWindowType != m_wmWindowType) {
                        m_wmWindowType = wmWindowType;
                        Q_EMIT wmWindowTypeChanged();
                    }

                    // Make sure only toplevel windows are decorated
                    if (m_decorate && m_wmWindowType != ToplevelWindow) {
                        m_decorate = false;
                        Q_EMIT decorateChanged();
                    }
                }
            }
            break;
        }
        case TYPE_WM_PROTOCOLS: {
            xcb_atom_t *atoms = reinterpret_cast<xcb_atom_t *>(xcb_get_property_value(reply));
            for (quint32 i = 0; i < reply->value_len; ++i)
                if (atoms[i] == Xcb::resources()->atoms->wm_delete_window)
                    m_properties.deleteWindow = 1;
            break;
        }
        case TYPE_WM_NORMAL_HINTS:
            memcpy(&m_sizeHints, xcb_get_property_value(reply), sizeof m_sizeHints);
            break;
        case TYPE_NET_WM_STATE: {
            xcb_atom_t *value = reinterpret_cast<xcb_atom_t *>(xcb_get_property_value(reply));
            uint32_t i;
            for (i = 0; i < reply->value_len; i++) {
                if (value[i] == Xcb::resources()->atoms->net_wm_state_fullscreen && !m_fullscreen) {
                    m_fullscreen = true;
                    Q_EMIT fullscreenChanged();
                }
            }
            if (value[i] == Xcb::resources()->atoms->net_wm_state_maximized_horz && !m_maximized) {
                m_maximized = true;
                Q_EMIT maximizedChanged();
            }
            if (value[i] == Xcb::resources()->atoms->net_wm_state_maximized_vert && !m_maximized) {
                m_maximized = true;
                Q_EMIT maximizedChanged();
            }
            break;
        }
        case TYPE_MOTIF_WM_HINTS:
            memcpy(&m_motifHints, xcb_get_property_value(reply), sizeof m_motifHints);
            if (m_motifHints.flags & MWM_HINTS_DECORATIONS) {
                if (m_motifHints.decorations & MWM_DECOR_ALL)
                    // MWM_DECOR_ALL means all except the other values listed
                    m_decorate = MWM_DECOR_EVERYTHING & (~m_motifHints.decorations);
                else
                    m_decorate = m_motifHints.decorations > 0;
                Q_EMIT decorateChanged();
            }
            break;
        default:
            break;
        }

        free(reply);
    }
}

QSize XWaylandShellSurface::sizeForResize(const QSizeF &size, const QPointF &delta, ResizeEdge edge)
{
    qreal width = size.width();
    qreal height = size.height();

    if (edge & LeftEdge)
        width -= delta.x();
    else if (edge & RightEdge)
        width += delta.x();

    if (edge & TopEdge)
        height -= delta.y();
    else if (edge & BottomEdge)
        height += delta.y();

    return QSizeF(width, height).toSize();
}

void XWaylandShellSurface::sendConfigure(const QRect &geometry)
{
    m_geometry = geometry;

    quint32 mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
    quint32 values[4];

    values[0] = m_geometry.topLeft().x();
    values[1] = m_geometry.topLeft().y();
    values[2] = m_geometry.size().width();
    values[3] = m_geometry.size().height();

    xcb_configure_window(Xcb::connection(), m_window, mask, values);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::sendResize(const QSize &size)
{
    m_geometry.setSize(size);

    quint32 mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
    quint32 values[2];

    values[0] = m_geometry.size().width();
    values[1] = m_geometry.size().height();

    xcb_configure_window(Xcb::connection(), m_window, mask, values);
    xcb_flush(Xcb::connection());
}

bool XWaylandShellSurface::isMoving() const
{
    return m_moving;
}

void XWaylandShellSurface::setMoving(bool moving)
{
    m_moving = moving;
}

bool XWaylandShellSurface::isResizing() const
{
    return m_resizing;
}

void XWaylandShellSurface::setResizing(bool resizing)
{
    m_resizing = resizing;
}

void XWaylandShellSurface::moveTo(const QPoint &pos)
{
    // Don't move the window when resizing
    if (m_resizing)
        return;

    m_geometry.setTopLeft(pos);
    Q_EMIT setPosition(pos.x(), pos.y());
}

void XWaylandShellSurface::resize(const QSize &size)
{
    m_geometry.setSize(size);
    Q_EMIT setSize(size);
}

void XWaylandShellSurface::sendPosition(const QPointF &pos)
{
    if (pos.toPoint() == m_geometry.topLeft())
        return;

    quint32 mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
    quint32 values[2];
    values[0] = pos.toPoint().x();
    values[1] = pos.toPoint().y();
    m_geometry.setTopLeft(pos.toPoint());

    xcb_configure_window(Xcb::connection(), m_window, mask, values);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::sendX(qreal x)
{
    if (qRound(x) == m_geometry.topLeft().x())
        return;

    quint32 mask = XCB_CONFIG_WINDOW_X;
    quint32 values[1];
    values[0] = qRound(x);
    m_geometry.setLeft(qRound(x));

    xcb_configure_window(Xcb::connection(), m_window, mask, values);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::sendY(qreal y)
{
    if (qRound(y) == m_geometry.topLeft().y())
        return;

    quint32 mask = XCB_CONFIG_WINDOW_Y;
    quint32 values[1];
    values[0] = qRound(y);
    m_geometry.setTop(qRound(y));

    xcb_configure_window(Xcb::connection(), m_window, mask, values);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::sendResize(const QSizeF &size)
{
    if (size.toSize() == m_geometry.size())
        return;

    quint32 mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
    quint32 values[2];

    values[0] = size.toSize().width();
    values[1] = size.toSize().height();
    m_geometry.setSize(size.toSize());

    xcb_configure_window(Xcb::connection(), m_window, mask, values);
    xcb_flush(Xcb::connection());
}

void XWaylandShellSurface::maximize(QWaylandOutput *output)
{
    Q_UNUSED(output);

    if (!m_maximized) {
        m_maximized = true;
        m_savedGeometry = m_geometry;
        sendConfigure(output->availableGeometry());
        moveTo(position());
        setNetWmState();
        Q_EMIT maximizedChanged();
    }
}

void XWaylandShellSurface::unmaximize()
{
    if (m_maximized) {
        m_maximized = false;
        sendConfigure(m_savedGeometry);
        moveTo(position());
        setNetWmState();
        Q_EMIT maximizedChanged();
    }
}

void XWaylandShellSurface::close()
{
    xcb_client_message_event_t msg;

    if (m_properties.deleteWindow) {
        msg.response_type = XCB_CLIENT_MESSAGE;
        msg.format = 32;
        msg.window = m_window;
        msg.type = Xcb::resources()->atoms->wm_protocols;
        msg.data.data32[0] = Xcb::resources()->atoms->wm_delete_window;
        msg.data.data32[1] = XCB_CURRENT_TIME;

        xcb_send_event(Xcb::connection(), 0, m_window, XCB_EVENT_MASK_NO_EVENT,
                       reinterpret_cast<char *>(&msg));
    } else {
        xcb_kill_client(Xcb::connection(), m_window);
    }

    xcb_flush(Xcb::connection());
}

XWaylandQuickShellIntegration *XWaylandShellSurface::createIntegration(XWaylandQuickShellSurfaceItem *item)
{
    return new XWaylandQuickShellIntegration(item);
}

XWaylandShellSurface::operator xcb_window_t() const
{
    return m_window;
}

xcb_window_t XWaylandShellSurface::window() const
{
    return m_window;
}

void XWaylandShellSurface::handleSeatChanged(QWaylandSeat *newSeat, QWaylandSeat *oldSeat)
{
    if (oldSeat)
        disconnect(oldSeat, &QWaylandSeat::keyboardFocusChanged,
                   this, &XWaylandShellSurface::handleFocusChanged);

    if (newSeat)
        connect(newSeat, &QWaylandSeat::keyboardFocusChanged,
                this, &XWaylandShellSurface::handleFocusChanged);
}

void XWaylandShellSurface::handleFocusChanged(QWaylandSurface *newSurface, QWaylandSurface *oldSurface)
{
    XWaylandShellSurface *newShellSurface = m_wm->shellSurfaceFromSurface(newSurface);
    XWaylandShellSurface *oldShellSurface = m_wm->shellSurfaceFromSurface(oldSurface);

    if (newShellSurface)
        newShellSurface->handleFocusReceived();

    if (oldShellSurface)
        oldShellSurface->handleFocusLost();
}

void XWaylandShellSurface::handleFocusReceived()
{
    if (!surface())
        return;

    m_wm->setActiveWindow(m_window);
    if (!m_overrideRedirect)
        m_wm->setFocusWindow(m_window);
    xcb_flush(Xcb::connection());

    m_activated = true;
    Q_EMIT activatedChanged();
}

void XWaylandShellSurface::handleFocusLost()
{
    if (!surface())
        return;

    //m_wm->setActiveWindow(XCB_WINDOW_NONE);
    //m_wm->setFocusWindow(XCB_WINDOW_NONE);
    //xcb_flush(Xcb::connection());

    m_activated = false;
    Q_EMIT activatedChanged();
}

void XWaylandShellSurface::handleSurfaceDestroyed()
{
    qCWarning(XWAYLAND) << "Surface paired with window" << m_window << "destroyed";

    Q_EMIT unmapped();
    m_surface = nullptr;
    Q_EMIT surfaceChanged();
    Q_EMIT surfaceDestroyed();
}

#include "moc_xwaylandshellsurface.cpp"
