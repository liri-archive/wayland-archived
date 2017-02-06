#.rst:
# FindGLib
# -------
#
# Try to find glib-2.0 on a Unix system.
#
# This will define the following variables:
#
# ``GLib_FOUND``
#     True if (the requested version of) glib is available
# ``GLib_VERSION``
#     The version of glib
# ``GLib_LIBRARIES``
#     This can be passed to target_link_libraries() instead of the ``GLib::GLib``
#     target
# ``GLib_INCLUDE_DIRS``
#     This should be passed to target_include_directories() if the target is not
#     used for linking
# ``GLib_DEFINITIONS``
#     This should be passed to target_compile_options() if the target is not
#     used for linking
#
# If ``GLib_FOUND`` is TRUE, it will also define the following imported target:
#
# ``UDev::UDev``
#     The UDev library
#
# In general we recommend using the imported target, as it is easier to use.
# Bear in mind, however, that if the target is in the link interface of an
# exported library, it must be made available by the package config file.

#=============================================================================
# Copyright 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

if(CMAKE_VERSION VERSION_LESS 2.8.12)
    message(FATAL_ERROR "CMake 2.8.12 is required by FindUDev.cmake")
endif()
if(CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS 2.8.12)
    message(AUTHOR_WARNING "Your project should require at least CMake 2.8.12 to use FindUDev.cmake")
endif()

# Use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig)
pkg_check_modules(PKG_GLib QUIET glib-2.0)

set(GLib_DEFINITIONS ${PKG_GLib_CFLAGS_OTHER})
set(GLib_VERSION ${PKG_GLib_VERSION})

find_path(GLib_INCLUDE_DIR
    NAMES
        glib-2.0/glib.h
    HINTS
        ${PKG_GLib_INCLUDEDIR}
)
find_library(GLib_LIBRARY
    NAMES
        glib-2.0
    HINTS
        ${PKG_GLib_LIBRARY_DIRS}
)
find_library(GObject_LIBRARY
    NAMES
        gobject-2.0
    HINTS
        ${PKG_GLib_LIBRARY_DIRS}
)
find_library(GThread_LIBRARY
    NAMES
        gthread-2.0
    HINTS
        ${PKG_GLib_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLib
    FOUND_VAR
        GLib_FOUND
    REQUIRED_VARS
        GLib_LIBRARY
        GLib_INCLUDE_DIR
    VERSION_VAR
        GLib_VERSION
)

if(GLib_FOUND AND NOT TARGET GLib::GLib)
    add_library(GLib::GLib UNKNOWN IMPORTED)
    set_target_properties(GLib::GLib PROPERTIES
        IMPORTED_LOCATION "${GLib_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${GLib_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLib_INCLUDE_DIR}"
    )
endif()

if(GLib_FOUND AND NOT TARGET GLib::GObject)
    add_library(GLib::GObject UNKNOWN IMPORTED)
    set_target_properties(GLib::GObject PROPERTIES
        IMPORTED_LOCATION "${GObject_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${GLib_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLib_INCLUDE_DIR}"
    )
endif()

if(GLib_FOUND AND NOT TARGET GLib::GThread)
    add_library(GLib::GThread UNKNOWN IMPORTED)
    set_target_properties(GLib::GThread PROPERTIES
        IMPORTED_LOCATION "${GThread_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${GLib_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLib_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(GLib_LIBRARY GLib_INCLUDE_DIR)

# Compatibility variables
set(GLib_LIBRARIES ${GLib_LIBRARY} ${GObject_LIBRARY} ${GThread_LIBRARY})
set(GLib_INCLUDE_DIRS ${GLib_INCLUDE_DIR})
set(GLib_VERSION_STRING ${GLib_VERSION})

include(FeatureSummary)
set_package_properties(GLib PROPERTIES
    URL "https://developer.gnome.org/glib/"
    DESCRIPTION "GLib provides the core application building blocks for libraries and applications written in C."
)
