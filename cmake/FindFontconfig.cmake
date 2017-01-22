#.rst:
# FindFontconfig
# -------
#
# Try to find fontconfig.
#
# This will define the following variables:
#
# ``Fontconfig_FOUND``
#     True if (the requested version of) Fontconfig is available
# ``Fontconfig_VERSION``
#     The version of Fontconfig
# ``Fontconfig_LIBRARIES``
#     This can be passed to target_link_libraries() instead of the ``Fontconfig::Fontconfig``
#     target
# ``Fontconfig_INCLUDE_DIRS``
#     This should be passed to target_include_directories() if the target is not
#     used for linking
# ``Fontconfig_DEFINITIONS``
#     This should be passed to target_compile_options() if the target is not
#     used for linking
#
# If ``Fontconfig_FOUND`` is TRUE, it will also define the following imported target:
#
# ``Fontconfig::Fontconfig``
#     The Fontconfig library
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
    message(FATAL_ERROR "CMake 2.8.12 is required by FindFontconfig.cmake")
endif()
if(CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS 2.8.12)
    message(AUTHOR_WARNING "Your project should require at least CMake 2.8.12 to use FindFontconfig.cmake")
endif()

# Use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig)
pkg_check_modules(PKG_Fontconfig QUIET fontconfig)

set(Fontconfig_DEFINITIONS ${PKG_Fontconfig_CFLAGS_OTHER})
set(Fontconfig_VERSION ${PKG_Fontconfig_VERSION})

find_path(Fontconfig_INCLUDE_DIR
    NAMES
        fontconfig/fontconfig.h
    HINTS
        ${PKG_Fontconfig_INCLUDEDIR}
)
find_library(Fontconfig_LIBRARY
    NAMES
        fontconfig
    HINTS
        ${PKG_Fontconfig_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fontconfig
    FOUND_VAR
        Fontconfig_FOUND
    REQUIRED_VARS
        Fontconfig_LIBRARY
        Fontconfig_INCLUDE_DIR
    VERSION_VAR
        Fontconfig_VERSION
)

if(Fontconfig_FOUND AND NOT TARGET Fontconfig::Fontconfig)
    add_library(Fontconfig::Fontconfig UNKNOWN IMPORTED)
    set_target_properties(Fontconfig::Fontconfig PROPERTIES
        IMPORTED_LOCATION "${Fontconfig_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${Fontconfig_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${Fontconfig_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(Fontconfig_LIBRARY Fontconfig_INCLUDE_DIR)

# compatibility variables
set(Fontconfig_LIBRARIES ${Fontconfig_LIBRARY})
set(Fontconfig_INCLUDE_DIRS ${Fontconfig_INCLUDE_DIR})
set(Fontconfig_VERSION_STRING ${Fontconfig_VERSION})

include(FeatureSummary)
set_package_properties(Fontconfig PROPERTIES
    URL "http://www.freedesktop.org/wiki/Software/fontconfig"
    DESCRIPTION "Library for configuring and customizing font access."
)
