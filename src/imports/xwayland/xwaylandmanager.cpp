/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include <QtCore/QSocketNotifier>
#include <QtCore/QtMath>

#include <QtWaylandCompositor/QWaylandSurface>

#include "xcbcursors.h"
#include "xcbwindow.h"
#include "xcbwrapper.h"
#include "xcbproperties.h"
#include "xcbresources.h"
#include "xwayland.h"
#include "xwaylandmanager.h"
#include "xwaylandshellsurface.h"
#include "xwaylandserver.h"

#include <unistd.h>
#include <xcb/composite.h>
#include <wayland-server.h>

XWaylandManager::XWaylandManager(QObject *parent)
    : QObject(parent)
    , m_server(nullptr)
    , m_cursors(nullptr)
    , m_lastCursor(CursorUnset)
    , m_wmWindow(nullptr)
    , m_compositor(nullptr)
    , m_focusWindow(nullptr)
{
}

XWaylandManager::~XWaylandManager()
{
    Xcb::Cursors::destroyCursors(m_cursors);
    delete m_wmWindow;
    Xcb::closeConnection();
}

XWaylandServer *XWaylandManager::server() const
{
    return m_server;
}

void XWaylandManager::setServer(XWaylandServer *server)
{
    m_server = server;
}

QWaylandCompositor *XWaylandManager::compositor() const
{
    return m_compositor;
}

void XWaylandManager::setCompositor(QWaylandCompositor *compositor)
{
    m_compositor = compositor;
}

void XWaylandManager::start(int fd)
{
    // Connect to XCB (xcb_connect_to_fd() takes ownership of the fd)
    qCDebug(XWAYLAND) << "Connect to X11";
    Xcb::openConnection(fd);
    if (xcb_connection_has_error(Xcb::connection())) {
        qCWarning(XWAYLAND) << "Connection failed!";
        ::close(fd);
        return;
    }

    // Listen to WM events
    QSocketNotifier *notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(wmEvents()));

    // Resources and atoms
    Xcb::resources();

    // Visual and colormap
    setupVisualAndColormap();

    // Root window attributes
    quint32 values[1];
    values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
            XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
            XCB_EVENT_MASK_PROPERTY_CHANGE;
    xcb_change_window_attributes(Xcb::connection(), Xcb::rootWindow(),
                                 XCB_CW_EVENT_MASK, values);

    // Redirect subwindows
    xcb_composite_redirect_subwindows(Xcb::connection(), Xcb::rootWindow(),
                                      XCB_COMPOSITE_REDIRECT_MANUAL);

    // Claim supported atoms
    quint32 supported[5];
    supported[0] = Xcb::resources()->atoms->net_wm_moveresize;
    supported[1] = Xcb::resources()->atoms->net_wm_state;
    supported[2] = Xcb::resources()->atoms->net_wm_state_fullscreen;
    supported[3] = Xcb::resources()->atoms->net_wm_state_maximized_vert;
    supported[4] = Xcb::resources()->atoms->net_wm_state_maximized_horz;
    xcb_change_property(
                Xcb::connection(), XCB_PROP_MODE_REPLACE,
                Xcb::rootWindow(),
                Xcb::resources()->atoms->net_supported,
                XCB_ATOM_ATOM, 32,
                (sizeof supported / sizeof supported[0]), supported
            );

    // Unset active window
    setActiveWindow(XCB_WINDOW_NONE);

    // Clain WM selection
    wmSelection();

    // Initialize DND
    initializeDragAndDrop();

    // Flush connection
    xcb_flush(Xcb::connection());

    // Setup cursors
    m_cursors = Xcb::Cursors::createCursors();
    setCursor(Xcb::rootWindow(), CursorLeftPointer);

    // Create window and take WM_S0 selection, this will signal
    // Xwayland that the setup is done
    createWindowManager();

    qCDebug(XWAYLAND) << "X window manager created, root" << Xcb::rootWindow();

    Q_EMIT created();
}

void XWaylandManager::addWindow(xcb_window_t id, XWaylandShellSurface *shellSurface)
{
    if (id != XCB_WINDOW_NONE && shellSurface)
        m_windowsMap[id] = shellSurface;
}

void XWaylandManager::removeWindow(xcb_window_t id)
{
    m_windowsMap.remove(id);
}

void XWaylandManager::setActiveWindow(xcb_window_t window)
{
    xcb_change_property(Xcb::connection(), XCB_PROP_MODE_REPLACE,
                        Xcb::rootWindow(), Xcb::resources()->atoms->net_active_window,
                        Xcb::resources()->atoms->window, 32, 1, &window);
}

void XWaylandManager::setFocusWindow(xcb_window_t window)
{
    if (window == XCB_WINDOW_NONE) {
        xcb_set_input_focus(Xcb::connection(), XCB_INPUT_FOCUS_POINTER_ROOT,
                            XCB_NONE, XCB_TIME_CURRENT_TIME);
    } else {
        xcb_client_message_event_t msg;
        msg.response_type = XCB_CLIENT_MESSAGE;
        msg.format = 32;
        msg.window = window;
        msg.type = Xcb::resources()->atoms->wm_protocols;
        msg.data.data32[0] = Xcb::resources()->atoms->wm_take_focus;
        msg.data.data32[1] = XCB_TIME_CURRENT_TIME;

        xcb_send_event(Xcb::connection(), 0, window, XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
                       reinterpret_cast<char *>(&msg));
        xcb_set_input_focus(Xcb::connection(), XCB_INPUT_FOCUS_POINTER_ROOT,
                            window, XCB_TIME_CURRENT_TIME);

        quint32 values[1];
        values[0] = XCB_STACK_MODE_ABOVE;
        xcb_configure_window(Xcb::connection(), window,
                             XCB_CONFIG_WINDOW_STACK_MODE, values);

        m_focusWindow = m_windowsMap[window];
    }
}

XWaylandShellSurface *XWaylandManager::shellSurfaceFromId(xcb_window_t id)
{
    return m_windowsMap.value(id, nullptr);
}

XWaylandShellSurface *XWaylandManager::shellSurfaceFromSurface(QWaylandSurface *surface)
{
    for (auto item : m_windowsMap.keys()) {
        auto shellSurface = m_windowsMap.value(item);
        if (!shellSurface)
            continue;
        if (shellSurface->surface() == surface)
            return shellSurface;
    }

    return nullptr;
}

void XWaylandManager::setupVisualAndColormap()
{
    xcb_depth_iterator_t depthIterator =
            xcb_screen_allowed_depths_iterator(Xcb::screen());
    xcb_visualtype_t *visualType = nullptr;
    xcb_visualtype_iterator_t visualTypeIterator;
    while (depthIterator.rem > 0) {
        if (depthIterator.data->depth == 32) {
            visualTypeIterator = xcb_depth_visuals_iterator(depthIterator.data);
            visualType = visualTypeIterator.data;
            break;
        }

        xcb_depth_next(&depthIterator);
    }

    if (!visualType) {
        qCDebug(XWAYLAND) << "No 32-bit visualtype";
        return;
    }

    m_visualId = visualType->visual_id;
    m_colorMap = xcb_generate_id(Xcb::connection());
    xcb_create_colormap(Xcb::connection(), XCB_COLORMAP_ALLOC_NONE,
                        m_colorMap, Xcb::rootWindow(), m_visualId);
}

void XWaylandManager::wmSelection()
{
    m_selRequest.requestor = XCB_NONE;

    quint32 values[1];
    values[0] = XCB_EVENT_MASK_PROPERTY_CHANGE;

    m_selWindow = xcb_generate_id(Xcb::connection());
    xcb_create_window(Xcb::connection(), XCB_COPY_FROM_PARENT,
                      m_selWindow, Xcb::rootWindow(),
                      0, 0, 10, 10, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      Xcb::rootVisual(),
                      XCB_CW_EVENT_MASK, values);
    xcb_set_selection_owner(Xcb::connection(), m_selWindow,
                            Xcb::resources()->atoms->clipboard_manager,
                            XCB_TIME_CURRENT_TIME);

    quint32 mask =
            XCB_XFIXES_SELECTION_EVENT_MASK_SET_SELECTION_OWNER |
            XCB_XFIXES_SELECTION_EVENT_MASK_SELECTION_WINDOW_DESTROY |
            XCB_XFIXES_SELECTION_EVENT_MASK_SELECTION_CLIENT_CLOSE;
    xcb_xfixes_select_selection_input(Xcb::connection(),
                                      m_selWindow,
                                      Xcb::resources()->atoms->clipboard,
                                      mask);

    //weston_wm_set_selection
}

void XWaylandManager::initializeDragAndDrop()
{

}

void XWaylandManager::setCursor(xcb_window_t window, const CursorType &cursor)
{
    if (m_lastCursor == cursor)
        return;

    m_lastCursor = cursor;
    quint32 cursorValueList = m_cursors[cursor];
    xcb_change_window_attributes(Xcb::connection(), window,
                                 XCB_CW_CURSOR, &cursorValueList);
    xcb_flush(Xcb::connection());
}

void XWaylandManager::createWindowManager()
{
    static const char name[] = "Liri";

    m_wmWindow = new Xcb::Window(QRect(0, 0, 10, 10),
                                 XCB_WINDOW_CLASS_INPUT_OUTPUT, 0, nullptr);
    xcb_window_t w = m_wmWindow->window();
    m_wmWindow->changeProperty(Xcb::resources()->atoms->net_supporting_wm_check,
                               XCB_ATOM_WINDOW, 32, 1, &w);
    m_wmWindow->changeProperty(Xcb::resources()->atoms->net_wm_name,
                               Xcb::resources()->atoms->utf8_string,
                               8, strlen(name), name);
    xcb_change_property(Xcb::connection(),
                        XCB_PROP_MODE_REPLACE,
                        Xcb::rootWindow(),
                        Xcb::resources()->atoms->net_supporting_wm_check,
                        XCB_ATOM_WINDOW, 32, 1, &w);

    // Claim WM_S0
    xcb_set_selection_owner(Xcb::connection(), w,
                            Xcb::resources()->atoms->wm_s0,
                            XCB_TIME_CURRENT_TIME);
    xcb_set_selection_owner(Xcb::connection(), w,
                            Xcb::resources()->atoms->net_wm_cm_s0,
                            XCB_TIME_CURRENT_TIME);
}

void XWaylandManager::handleButton(xcb_button_press_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_BUTTON_%s (detail %d)",
            event->response_type == XCB_BUTTON_PRESS ? "PRESS" : "RELEASE",
            event->detail);
}

void XWaylandManager::handleEnter(xcb_enter_notify_event_t *event)
{
    setCursor(event->event, CursorLeftPointer);
}

void XWaylandManager::handleLeave(xcb_leave_notify_event_t *event)
{
    setCursor(event->event, CursorLeftPointer);
}

void XWaylandManager::handleMotion(xcb_motion_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_MOTION_NOTIFY (window %d, x %d, y %d)",
            event->event, event->event_x, event->event_y);

    if (!m_windowsMap.contains(event->event))
        return;

    XWaylandShellSurface *shellSurface = m_windowsMap[event->event];
    shellSurface->moveTo(QPoint(event->event_x, event->event_y));
}

void XWaylandManager::handleCreateNotify(xcb_create_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_CREATE_NOTIFY (window %d, %d,%d @ %dx%d%s%s)",
            event->window, event->x, event->y, event->width, event->height,
            event->override_redirect ? ", override" : "",
            Xcb::isOurResource(event->window) ? ", ours" : "");

    if (Xcb::isOurResource(event->window))
        return;

    XWaylandShellSurface *parentShellSurface = nullptr;
    if (event->override_redirect != 0)
        parentShellSurface = m_windowsMap[event->parent];
    qCDebug(XWAYLAND_TRACE) << "Parent" << parentShellSurface;

    Q_EMIT shellSurfaceRequested(static_cast<quint32>(event->window),
                                 QRect(QPoint(event->x, event->y), QSize(event->width, event->height)),
                                 event->override_redirect != 0, parentShellSurface);
}

void XWaylandManager::handleMapRequest(xcb_map_request_event_t *event)
{
    if (Xcb::isOurResource(event->window)) {
        qCDebug(XWAYLAND_TRACE, "XCB_MAP_REQUEST (window %d, ours)",
                event->window);
        return;
    }

    if (!m_windowsMap.contains(event->window))
        return;

    XWaylandShellSurface *shellSurface = m_windowsMap[event->window];

    qCDebug(XWAYLAND_TRACE, "XCB_MAP_REQUEST (window %d, %p)",
            event->window, shellSurface);

    shellSurface->readProperties();
    shellSurface->setWmState(XWaylandShellSurface::NormalState);
    shellSurface->setNetWmState();
    shellSurface->setWorkspace(0);
    xcb_map_window(Xcb::connection(), event->window);
    xcb_flush(Xcb::connection());
}

void XWaylandManager::handleMapNotify(xcb_map_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_MAP_NOTIFY (window %d%s)",
            event->window,
            Xcb::isOurResource(event->window) ? ", ours" : "");
}

void XWaylandManager::handleUnmapNotify(xcb_unmap_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_UNMAP_NOTIFY (window %d, event %d%s)",
            event->window, event->event,
            Xcb::isOurResource(event->window) ? ", ours" : "");

    if (Xcb::isOurResource(event->window))
        return;

    // We just ignore the ICCCM 4.1.4 synthetic unmap notify
    // as it may come in after we've destroyed the window
    if (event->response_type & ~0x80)
        return;

    if (!m_windowsMap.contains(event->window))
        return;

    XWaylandShellSurface *shellSurface = m_windowsMap[event->window];

    if (m_focusWindow == shellSurface)
        m_focusWindow = nullptr;

    //shellSurface->setSurface(nullptr);
    shellSurface->setSurfaceId(0);
    shellSurface->setWmState(XWaylandShellSurface::WithdrawnState);
    shellSurface->setWorkspace(-1);
    xcb_unmap_window(Xcb::connection(), event->window);
    xcb_flush(Xcb::connection());
}

void XWaylandManager::handleReparentNotify(xcb_reparent_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_REPARENT_NOTIFY (window %d, parent %d, event %d)",
            event->window, event->parent, event->event);

    if (event->parent == Xcb::rootWindow()) {
        Q_EMIT shellSurfaceRequested(event->window,
                                     QRect(QPoint(event->x, event->y), QSize(10, 10)),
                                     event->override_redirect != 0, nullptr);
    } else if (!Xcb::isOurResource(event->parent)) {
        if (!m_windowsMap.contains(event->parent))
            return;
        m_windowsMap.take(event->parent)->deleteLater();
    }
}

void XWaylandManager::handleConfigureRequest(xcb_configure_request_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_CONFIGURE_REQUEST (window %d) %d,%d @ %dx%d",
            event->window, event->x, event->y, event->width, event->height);

    XWaylandShellSurface *shellSurface = m_windowsMap[event->window];

    quint32 values[6];
    int i = -1;

    if (event->value_mask & XCB_CONFIG_WINDOW_X) {
        values[++i] = event->x;
        if (shellSurface && shellSurface->surface()) {
            QPoint pos = shellSurface->geometry().topLeft();
            shellSurface->moveTo(QPoint(event->x, pos.y()));
        }
    }
    if (event->value_mask & XCB_CONFIG_WINDOW_Y) {
        values[++i] = event->y;
        if (shellSurface && shellSurface->surface()) {
            QPoint pos = shellSurface->geometry().topLeft();
            shellSurface->moveTo(QPoint(pos.x(), event->y));
        }
    }

    if (event->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
        values[++i] = event->width;
        if (shellSurface && shellSurface->surface()) {
            QSize size = shellSurface->geometry().size();
            shellSurface->resize(QSize(event->width, size.height()));
        }
    }
    if (event->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
        values[++i] = event->height;
        if (shellSurface && shellSurface->surface()) {
            QSize size = shellSurface->geometry().size();
            shellSurface->resize(QSize(size.width(), event->height));
        }
    }

    if (event->value_mask & XCB_CONFIG_WINDOW_SIBLING)
        values[++i] = event->sibling;

    if (event->value_mask & XCB_CONFIG_WINDOW_STACK_MODE)
        values[++i] = event->stack_mode;

    if (i >= 0) {
        xcb_configure_window(Xcb::connection(), event->window, event->value_mask, values);
        xcb_flush(Xcb::connection());
    }
}

void XWaylandManager::handleConfigureNotify(xcb_configure_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_CONFIGURE_NOTIFY (window %d) %d,%d @ %dx%d",
            event->window, event->x, event->y, event->width, event->height);

    if (!m_windowsMap.contains(event->window))
        return;

    XWaylandShellSurface *shellSurface = m_windowsMap[event->window];
    shellSurface->moveTo(QPoint(event->x, event->y));
    if (shellSurface->overrideRedirect())
        shellSurface->resize(QSize(event->width, event->y));
}

void XWaylandManager::handleDestroyNotify(xcb_destroy_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_DESTROY_NOTIFY (window %d, event %d%s)",
            event->window, event->event,
            Xcb::isOurResource(event->window) ? ", ours" : "");

    if (Xcb::isOurResource(event->window))
        return;

    if (!m_windowsMap.contains(event->window))
        return;

    XWaylandShellSurface *shellSurface = m_windowsMap.take(event->window);
    connect(shellSurface, &XWaylandShellSurface::unmapped,
            shellSurface, &XWaylandShellSurface::deleteLater);
    shellSurface->setSurface(nullptr);
}

void XWaylandManager::handlePropertyNotify(xcb_property_notify_event_t *event)
{
    qCDebug(XWAYLAND_TRACE, "XCB_PROPERTY_NOTIFY (window %d)", event->window);

    XWaylandShellSurface *shellSurface = m_windowsMap[event->window];
    if (!shellSurface)
        return;

    shellSurface->dirtyProperties();

    if (event->state == XCB_PROPERTY_DELETE)
        qCDebug(XWAYLAND_TRACE, "\tdeleted");
    else
        Xcb::Properties::readAndDumpProperty(event->atom, event->window);
}

void XWaylandManager::handleClientMessage(xcb_client_message_event_t *event)
{
    QString name = Xcb::Atom::nameFromAtom(event->type);
    qCDebug(XWAYLAND_TRACE, "XCB_CLIENT_MESSAGE (%s %d %d %d %d %d win %d)",
            qPrintable(name),
            event->data.data32[0],
            event->data.data32[1],
            event->data.data32[2],
            event->data.data32[3],
            event->data.data32[4],
            event->window);

    // Check whether we have the surface because the window may get
    // created and destroyed before we actually handle this message
    if (!m_windowsMap.contains(event->window))
        return;

    // Get the surface for this shell surface
    XWaylandShellSurface *shellSurface = m_windowsMap[event->window];

    // Handle messages
    if (event->type == Xcb::resources()->atoms->net_wm_moveresize)
        handleMoveResize(shellSurface, event);
    else if (event->type == Xcb::resources()->atoms->wm_change_state)
        handleChangeState(shellSurface, event);
    else if (event->type == Xcb::resources()->atoms->net_wm_state)
        handleState(shellSurface, event);
    else if (event->type == Xcb::resources()->atoms->wl_surface_id)
        handleSurfaceId(shellSurface, event);
}

void XWaylandManager::handleFocusIn(xcb_focus_in_event_t *event)
{
    // Do not interfere with grabs
    if (event->mode == XCB_NOTIFY_MODE_GRAB ||
            event->mode == XCB_NOTIFY_MODE_UNGRAB)
        return;

    // Do not let X clients change focus behind our back, reset
    // focus to the old one if it has changed
    if (!m_focusWindow || event->event != m_focusWindow->window())
        setFocusWindow(event->event);
}

bool XWaylandManager::handleSelection(xcb_generic_event_t *event)
{
    switch (event->response_type & ~0x80) {
    case XCB_SELECTION_NOTIFY:
        handleSelectionNotify(reinterpret_cast<xcb_selection_notify_event_t *>(event));
        return true;
        /*
    case XCB_PROPERTY_NOTIFY:
        return handleSelectionPropertyNotify(event);
    case XCB_SELECTION_REQUEST:
        handleSelectionRequest(event);
        return true;
        */
    default:
        break;
    }

    /*
    switch (event->response_type - xfixes->first_event) {
    case XCB_XFIXES_SELECTION_NOTIFY:
        return handleXFixesSelectionNotify(event);
    }
    */

    return false;
}

void XWaylandManager::handleSelectionNotify(xcb_selection_notify_event_t *event)
{
    if (event->property == XCB_ATOM_NONE) {
        /* convert selection failed */
    } else if (event->target == Xcb::resources()->atoms->targets) {
        /*
              weston_wm_get_selection_targets(wm);
      } else {
              weston_wm_get_selection_data(wm);
              */
    }
}

void XWaylandManager::handleMoveResize(XWaylandShellSurface *shellSurface, xcb_client_message_event_t *event)
{
    if (!shellSurface || !event)
        return;

    int detail = event->data.data32[2];
    switch (detail) {
    case _NET_WM_MOVERESIZE_MOVE:
        Q_EMIT shellSurface->startMove();
        break;
    case _NET_WM_MOVERESIZE_SIZE_TOPLEFT:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::TopLeftEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_TOP:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::TopEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_TOPRIGHT:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::TopRightEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_RIGHT:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::RightEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::BottomRightEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_BOTTOM:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::BottomEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::BottomLeftEdge);
        break;
    case _NET_WM_MOVERESIZE_SIZE_LEFT:
        Q_EMIT shellSurface->startResize(XWaylandShellSurface::LeftEdge);
        break;
    default:
        break;
    }
}

void XWaylandManager::handleChangeState(XWaylandShellSurface *shellSurface, xcb_client_message_event_t *event)
{
    if (!shellSurface || !event)
        return;

    if (event->data.data32[0] == 3)
        Q_EMIT shellSurface->setMinimized();
}

void XWaylandManager::handleState(XWaylandShellSurface *shellSurface, xcb_client_message_event_t *event)
{
    if (!shellSurface || !event)
        return;

    auto updateState = [](int action, bool *state) {
#define _NET_WM_STATE_REMOVE    0
#define _NET_WM_STATE_ADD       1
#define _NET_WM_STATE_TOGGLE    2

        bool newState;
        bool changed;

        switch (action) {
        case _NET_WM_STATE_REMOVE:
            newState = false;
            break;
        case _NET_WM_STATE_ADD:
            newState = true;
            break;
        case _NET_WM_STATE_TOGGLE:
            newState = !*state;
            break;
        default:
            return false;
        }

        changed = (*state != newState);
        *state = newState;

        return changed;
    };

    quint32 action = event->data.data32[0];
    quint32 property = event->data.data32[1];

    bool maximized = shellSurface->isMaximized();

    if (property == Xcb::resources()->atoms->net_wm_state_fullscreen &&
            updateState(action, &shellSurface->m_fullscreen)) {
        shellSurface->setNetWmState();

        if (shellSurface->m_fullscreen) {
            //shellSurface->m_properties.savedSize = shellSurface->m_properties.size;
            shellSurface->m_fullscreen = false;
            Q_EMIT shellSurface->fullscreenChanged();
        }
    } else {
        if ((property == Xcb::resources()->atoms->net_wm_state_maximized_horz ||
             property == Xcb::resources()->atoms->net_wm_state_maximized_vert) &&
                updateState(action, &shellSurface->m_maximized))
            shellSurface->setNetWmState();

        if (maximized != shellSurface->isMaximized()) {
            if (shellSurface->isMaximized()) {
                //shellSurface->m_properties.savedSize = shellSurface->m_properties.size;
                shellSurface->unmaximize();
            }
        }
    }
}

void XWaylandManager::handleSurfaceId(XWaylandShellSurface *shellSurface, xcb_client_message_event_t *event)
{
    if (!shellSurface || !event)
        return;

    if (shellSurface->surface()) {
        qCWarning(XWAYLAND_TRACE) << "Window" << shellSurface->window() << "already has a surface id";
        return;
    }

    quint32 id = event->data.data32[0];
    shellSurface->setSurfaceId(id);

    wl_resource *resource = wl_client_get_object(m_server->client(), id);
    QWaylandSurface *surface = resource ? QWaylandSurface::fromResource(resource) : nullptr;
    if (surface)
        shellSurface->setSurface(surface);
    else
        m_unpairedWindows.append(shellSurface);
}

void XWaylandManager::wmEvents()
{
    // Uncomment if you want to be flooded by messages
    //qCDebug(XWAYLAND_TRACE) << "WM event";

    xcb_generic_event_t *event;
    int count = 0;

    while ((event = xcb_poll_for_event(Xcb::connection()))) {
        //handle selection event
        //handle dnd event

        int type = event->response_type & ~0x80;
        switch (type) {
        case XCB_BUTTON_PRESS:
        case XCB_BUTTON_RELEASE:
            handleButton(reinterpret_cast<xcb_button_press_event_t *>(event));
            break;
        case XCB_ENTER_NOTIFY:
            handleEnter(reinterpret_cast<xcb_enter_notify_event_t *>(event));
            break;
        case XCB_LEAVE_NOTIFY:
            handleLeave(reinterpret_cast<xcb_leave_notify_event_t *>(event));
            break;
        case XCB_MOTION_NOTIFY:
            handleMotion(reinterpret_cast<xcb_motion_notify_event_t *>(event));
            break;
        case XCB_CREATE_NOTIFY:
            handleCreateNotify(reinterpret_cast<xcb_create_notify_event_t *>(event));
            break;
        case XCB_MAP_REQUEST:
            handleMapRequest(reinterpret_cast<xcb_map_request_event_t *>(event));
            break;
        case XCB_MAP_NOTIFY:
            handleMapNotify(reinterpret_cast<xcb_map_notify_event_t *>(event));
            break;
        case XCB_UNMAP_NOTIFY:
            handleUnmapNotify(reinterpret_cast<xcb_unmap_notify_event_t *>(event));
            break;
        case XCB_REPARENT_NOTIFY:
            handleReparentNotify(reinterpret_cast<xcb_reparent_notify_event_t *>(event));
            break;
        case XCB_CONFIGURE_REQUEST:
            handleConfigureRequest(reinterpret_cast<xcb_configure_request_event_t *>(event));
            break;
        case XCB_CONFIGURE_NOTIFY:
            handleConfigureNotify(reinterpret_cast<xcb_configure_notify_event_t *>(event));
            break;
        case XCB_DESTROY_NOTIFY:
            handleDestroyNotify(reinterpret_cast<xcb_destroy_notify_event_t *>(event));
            break;
        case XCB_MAPPING_NOTIFY:
            break;
        case XCB_PROPERTY_NOTIFY:
            handlePropertyNotify(reinterpret_cast<xcb_property_notify_event_t *>(event));
            break;
        case XCB_CLIENT_MESSAGE:
            handleClientMessage(reinterpret_cast<xcb_client_message_event_t *>(event));
            break;
        case XCB_FOCUS_IN:
            handleFocusIn(reinterpret_cast<xcb_focus_in_event_t *>(event));
        default:
            break;
        }

        free(event);
        count++;
    }

    if (count > 0)
        xcb_flush(Xcb::connection());
}
