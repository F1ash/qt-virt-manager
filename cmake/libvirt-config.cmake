
# libvirt-config.cmake
# --------------------
#
#   LibVirt_INCLUDE_DIRS  - Directory where libvirt headers are located.
#   LibVirt_LIBRARIES    - libvirt libraries to link against

get_filename_component(
  _libvirt_rootdir
  ${CMAKE_CURRENT_LIST_DIR}/../../../../../../ ABSOLUTE
)

set(LibVirt_INSTALL_PREFIX ${_libvirt_rootdir}usr)
set(LibVirt_INCLUDE_DIRS   ${LibVirt_INSTALL_PREFIX}/include)
set(LibVirt_LIBRARY_DIR    ${LibVirt_INSTALL_PREFIX}/lib${LIB_SUFFIX})
set(LibVirt_LIBRARIES      -L${LibVirt_LIBRARY_DIR} -lvirt)

mark_as_advanced( LibVirt_INCLUDE_DIRS LibVirt_LIBRARIES )
