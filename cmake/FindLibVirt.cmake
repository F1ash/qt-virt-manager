# FindLibVirt.cmake

# LibVirt_FOUND - System has LibVirt
# LibVirt_INCLUDES - The LibVirt include directories
# LibVirt_LIBRARIES - The libraries needed to use LibVirt
# LibVirt_DEFINITIONS - Compiler switches required for using LibVirt

find_package(PkgConfig)
pkg_check_modules(PC_LibVirt QUIET libvirt)
set(LibVirt_DEFINITIONS ${PC_LibVirt_CFLAGS_OTHER})

find_path(LibVirt_INCLUDE_DIR
          NAMES libvirt/libvirt.h libvirt/virterror.h
          HINTS ${PC_LibVirt_INCLUDEDIR} ${PC_LibVirt_INCLUDE_DIRS}
          PATH_SUFFIXES libvirt)

find_library(LibVirt_LIBRARY
             NAMES virt libvirt
             HINTS ${PC_LibVirt_LIBDIR} ${PC_LibVirt_LIBRARY_DIRS})

set(LibVirt_LIBRARIES ${LibVirt_LIBRARY})
set(LibVirt_INCLUDE_DIRS ${LibVirt_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibVirt DEFAULT_MSG
                                  LibVirt_LIBRARY LibVirt_INCLUDE_DIR)

mark_as_advanced(LibVirt_INCLUDE_DIR LibVirt_LIBRARY)
