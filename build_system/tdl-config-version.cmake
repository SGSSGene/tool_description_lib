# This file adds version support for `find_package(tdl 3.1)`.
# See https://cmake.org/cmake/help/v3.16/manual/cmake-packages.7.html#package-version-file for more information.
#
# This file was partially generated by
# https://cmake.org/cmake/help/v3.16/module/CMakePackageConfigHelpers.html#command:write_basic_package_version_file

# Note that tdl-config.cmake can be standalone and thus TDL_CLONE_DIR might be empty.
find_path (TDL_CLONE_DIR NAMES build_system/tdl-config.cmake HINTS "${CMAKE_CURRENT_LIST_DIR}/..")
find_path (TDL_INCLUDE_DIR NAMES tdl/version.h HINTS "${TDL_CLONE_DIR}/include")

# extract version from tdl/version.h header
file(STRINGS "${TDL_INCLUDE_DIR}/tdl/version.h" TDL_version.h REGEX "#define TDL_VERSION_(MAJOR|MINOR|PATCH)")
string(REGEX REPLACE "#define TDL_VERSION_(MAJOR|MINOR|PATCH) " "" PACKAGE_VERSION "${TDL_VERION_h}")
string(REGEX REPLACE ";" "." PACKAGE_VERSION "${PACKAGE_VERSION}")

if(PACKAGE_VERSION VERSION_LESS PACKAGE_FIND_VERSION)
    set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()

    if(PACKAGE_VERSION MATCHES "^([0-9]+)\\.")
        set(_PACKAGE_VERSION_MAJOR "${CMAKE_MATCH_1}")
    endif()

    if(PACKAGE_FIND_VERSION_MAJOR STREQUAL _PACKAGE_VERSION_MAJOR)
        set(PACKAGE_VERSION_COMPATIBLE TRUE)
    else()
        set(PACKAGE_VERSION_COMPATIBLE FALSE)
    endif()

    if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
        set(PACKAGE_VERSION_EXACT TRUE)
    endif()
endif()

# extract release candidate
file(STRINGS "${TDL_INCLUDE_DIR}/tdl/version.h" TDL_RELEASE_CANDIDATE_H REGEX "#define TDL_RELEASE_CANDIDATE ")
string(REGEX REPLACE "#define TDL_RELEASE_CANDIDATE " "" TDL_RELEASE_CANDIDATE_VERSION "${TDL_RELEASE_CANDIDATE_H}")

# As of writing this (cmake 3.20):
# cmake does not allow to set a version containing a suffix via `project(... VERSION 3.0.3-rc.1)`.
# Version comparisons like VERSION_LESS, VERSION_GREATER do support comparing versions with a suffix (they just drop
# it), see https://cmake.org/cmake/help/latest/command/if.html#version-comparisons.
#
# If https://gitlab.kitware.com/cmake/cmake/-/issues/16716 is ever resolved, we can use TDL_VERSION instead of
# TDL_PROJECT_VERSION.
#
# TDL_PROJECT_VERSION is intended to be used within `project (... VERSION "${TDL_PROJECT_VERSION}")`.
set (TDL_PROJECT_VERSION "${PACKAGE_VERSION}")
if (TDL_RELEASE_CANDIDATE_VERSION VERSION_GREATER "0")
    set (PACKAGE_VERSION "${PACKAGE_VERSION}-rc.${TDL_RELEASE_CANDIDATE_VERSION}")
endif ()

if (NOT TDL_PROJECT_VERSION VERSION_EQUAL PACKAGE_VERSION)
    # Note: depending on how https://gitlab.kitware.com/cmake/cmake/-/issues/16716 is resolved (whether they use semver
    # comparison semantics), (NOT "3.0.3" VERSION_GREATER_EQUAL "3.0.3-rc.1") might be the correct expression.
    message (AUTHOR_WARNING "TDL_PROJECT_VERSION and TDL_VERSION mismatch, please report this issue and mention your cmake version.")
endif ()

# if the installed or the using project don't have CMAKE_SIZEOF_VOID_P set, ignore it:
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "")
    return()
endif()

# check that the installed version has the same 32/64bit-ness as the one which is currently searching:
if(NOT "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    math(EXPR installedBits "8 * 8")
    # For some time we set these variables to show that we do not support 32 bit architectures,
    # but it seems to hard to actively forbid them. Most of the library does build, but some parts
    # might not.
    # set(PACKAGE_VERSION "${PACKAGE_VERSION} (${installedBits}bit)")
    # set(PACKAGE_VERSION_UNSUITABLE TRUE)
    message (AUTHOR_WARNING "tdl does not support 32bit architectures; No guarantees; Patches are welcome.")
endif()
