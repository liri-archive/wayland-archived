#.rst:
# FindUDev
# -------
#
# Try to find udev on a Unix system.
#
# This will define the following variables:
#
# ``UDev_FOUND``
#     True if (the requested version of) UDev is available
# ``UDev_VERSION``
#     The version of UDev
# ``UDev_LIBRARIES``
#     This can be passed to target_link_libraries() instead of the ``UDev::UDev``
#     target
# ``UDev_INCLUDE_DIRS``
#     This should be passed to target_include_directories() if the target is not
#     used for linking
# ``UDev_DEFINITIONS``
#     This should be passed to target_compile_options() if the target is not
#     used for linking
#
# If ``UDev_FOUND`` is TRUE, it will also define the following imported target:
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
pkg_check_modules(PKG_UDev QUIET libudev)

set(UDev_DEFINITIONS ${PKG_UDev_CFLAGS_OTHER})
set(UDev_VERSION ${PKG_UDev_VERSION})

find_path(UDev_INCLUDE_DIR
    NAMES
        libudev.h
    HINTS
        ${PKG_UDev_INCLUDEDIR}
)
find_library(UDev_LIBRARY
    NAMES
        udev
    HINTS
        ${PKG_UDev_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UDev
    FOUND_VAR
        UDev_FOUND
    REQUIRED_VARS
        UDev_LIBRARY
        UDev_INCLUDE_DIR
    VERSION_VAR
        UDev_VERSION
)

if(UDev_FOUND AND NOT TARGET UDev::UDev)
    add_library(UDev::UDev UNKNOWN IMPORTED)
    set_target_properties(UDev::UDev PROPERTIES
        IMPORTED_LOCATION "${UDev_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${UDev_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${UDev_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(UDev_LIBRARY UDev_INCLUDE_DIR)

# compatibility variables
set(UDev_LIBRARIES ${UDev_LIBRARY})
set(UDev_INCLUDE_DIRS ${UDev_INCLUDE_DIR})
set(UDev_VERSION_STRING ${UDev_VERSION})

include(FeatureSummary)
set_package_properties(UDev PROPERTIES
    URL "http://www.freedesktop.org/software/systemd/libudev/"
    DESCRIPTION "Library to access udev device information."
)
