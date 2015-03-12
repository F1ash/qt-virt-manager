# FindSpice1.cmake

# Spice1_FOUND - System has Spice1
# Spice1_INCLUDES - The Spice1 include directories
# Spice1_LIBRARIES - The libraries needed to use Spice1
# Spice1_DEFINITIONS - Compiler switches required for using Spice1

find_package(PkgConfig)
pkg_check_modules(PC_Spice1 QUIET spice-protocol)
set(Spice1_DEFINITIONS ${PC_Spice1_CFLAGS_OTHER})

find_path(Spice1_INCLUDE_DIR
          NAMES spice/enums.h
          HINTS ${PC_Spice1_INCLUDEDIR} ${PC_Spice1_INCLUDE_DIRS}
          PATH_SUFFIXES spice-1)

# don't used ${Spice1_LIBRARY}, because library don't exist as one,
# it part of spice-client-glib-2.0 library.
#find_library(Spice1_LIBRARY NAMES spice-1 libspice-protocol libspice-1
#             HINTS ${PC_Spice1_LIBDIR} ${PC_Spice1_LIBRARY_DIRS})

#set(Spice1_LIBRARIES ${Spice1_LIBRARY})
set(Spice1_INCLUDE_DIRS ${Spice1_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Spice1 DEFAULT_MSG
                                  Spice1_INCLUDE_DIR
                                 #Spice1_LIBRARY
                                  )

mark_as_advanced(
    Spice1_INCLUDE_DIR
   #Spice1_LIBRARY
)
