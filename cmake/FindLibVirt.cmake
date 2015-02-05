# FindLibVirt.cmake

# LibVirt_FOUND - System has LibVirt
# LibVirt_INCLUDES - The LibVirt include directories
# LibVirt_LIBRARIES - The libraries needed to use LibVirt
# LibVirt_DEFINITIONS - Compiler switches required for using LibVirt

# LibFindMacros.cmake file must be placed in the module path
include(LibFindMacros)

# Dependencies
#libfind_package(LibVirt <dependencies don't found>)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(LibVirt_PKGCONF LibVirt)

# Include dir
find_path(LibVirt_INCLUDE_DIR
  NAMES libvirt.h libvirt/libvirt.h
  PATHS ${LibVirt_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(LibVirt_LIBRARY
  NAMES virt
  PATHS ${LibVirt_PKGCONF_LIBRARY_DIRS}
)

set(LibVirt_PROCESS_INCLUDES ${LibVirt_INCLUDE_DIR}) # <dependencies don't found>_INCLUDE_DIRS)
set(LibVirt_PROCESS_LIBS ${LibVirt_LIBRARY}) # <dependencies don't found>_LIBRARIES)
libfind_process(LibVirt)
