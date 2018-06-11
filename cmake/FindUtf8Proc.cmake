# FindUtf8Proc.cmake

# UTF8PROC_FOUND - System has UTF8PROC
# UTF8PROC_INCLUDES - The UTF8PROC include directories
# UTF8PROC_LIBRARIES - The libraries needed to use UTF8PROC
# UTF8PROC_DEFINITIONS - Compiler switches required for using UTF8PROC

find_package(PkgConfig)
pkg_check_modules(PC_UTF8PROC QUIET utf8proc)
set(UTF8PROC_DEFINITIONS ${PC_UTF8PROC_CFLAGS_OTHER})

find_path(UTF8PROC_INCLUDE_DIR
          NAMES utf8proc.h
          HINTS ${PC_UTF8PROC_INCLUDEDIR} ${PC_UTF8PROC_INCLUDE_DIRS})

find_library(UTF8PROC_LIBRARY
             NAMES utf8proc
             HINTS ${PC_UTF8PROC_LIBDIR} ${PC_UTF8PROC_LIBRARY_DIRS})

set(UTF8PROC_LIBRARIES ${UTF8PROC_LIBRARY})
set(UTF8PROC_INCLUDE_DIRS ${UTF8PROC_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UTF8PROC DEFAULT_MSG
                                  UTF8PROC_LIBRARY
                                  UTF8PROC_INCLUDE_DIR)

mark_as_advanced(UTF8PROC_INCLUDE_DIR UTF8PROC_LIBRARY)
