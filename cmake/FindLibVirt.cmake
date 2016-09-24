# FindLibVirt.cmake

# LibVirt_FOUND - System has LibVirt
# LibVirt_INCLUDES - The LibVirt include directories
# LibVirt_LIBRARIES - The libraries needed to use LibVirt
# LibVirt_DEFINITIONS - Compiler switches required for using LibVirt

find_package(PkgConfig)
pkg_check_modules(PC_LibVirt QUIET libvirt)
set(LibVirt_DEFINITIONS ${PC_LibVirt_CFLAGS_OTHER})

find_path(
    LibVirt_INCLUDE_DIRS
    NAMES libvirt.h virterror.h
    HINTS ${PC_LibVirt_INCLUDEDIR} ${PC_LibVirt_INCLUDE_DIRS}
    PATH_SUFFIXES libvirt)

find_library(
    LibVirt_LIBRARIES
    NAMES virt libvirt
    HINTS ${PC_LibVirt_LIBDIR} ${PC_LibVirt_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    LibVirt DEFAULT_MSG LibVirt_LIBRARIES LibVirt_INCLUDE_DIRS)

mark_as_advanced(LibVirt_INCLUDE_DIRS LibVirt_LIBRARIES)
