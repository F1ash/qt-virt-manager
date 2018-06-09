# FindLibUtf8Proc.cmake

# LIB_UTF8_PROC_FOUND - System has LIB_UTF8_PROC
# LIB_UTF8_PROC_INCLUDES - The LIB_UTF8_PROC include directories
# LIB_UTF8_PROC_LIBRARIES - The libraries needed to use LIB_UTF8_PROC
# LIB_UTF8_PROC_DEFINITIONS - Compiler switches required for using LIB_UTF8_PROC

find_package(PkgConfig)
pkg_check_modules(PC_LIB_UTF8_PROC QUIET libutf8proc)
set(LIB_UTF8_PROC_DEFINITIONS ${PC_LIB_UTF8_PROC_CFLAGS_OTHER})

find_path(LIB_UTF8_PROC_INCLUDE_DIR
          NAMES utf8proc.h
          HINTS ${PC_LIB_UTF8_PROC_INCLUDEDIR} ${PC_LIB_UTF8_PROC_INCLUDE_DIRS})

find_library(LIB_UTF8_PROC_LIBRARY
             NAMES utf8proc
             HINTS ${PC_LIB_UTF8_PROC_LIBDIR} ${PC_LIB_UTF8_PROC_LIBRARY_DIRS})

set(LIB_UTF8_PROC_LIBRARIES ${LIB_UTF8_PROC_LIBRARY})
set(LIB_UTF8_PROC_INCLUDE_DIRS ${LIB_UTF8_PROC_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIB_UTF8_PROC DEFAULT_MSG
                                  LIB_UTF8_PROC_LIBRARY
                                  LIB_UTF8_PROC_INCLUDE_DIR)

mark_as_advanced(LIB_UTF8_PROC_INCLUDE_DIR LIB_UTF8_PROC_LIBRARY)
