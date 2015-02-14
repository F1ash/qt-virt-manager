# TODO: make a cross platform here
if (NOT CMAKE_INSTALL_PREFIX)
    set (CMAKE_INSTALL_PREFIX /usr)
endif ()
list(APPEND CMAKE_PREFIX_PATH . ./cmake)

cmake_minimum_required (VERSION 2.8.3)
include(CheckFunctionExists)

##
if ( NOT DEFAULT_ENCODING )
    set ( DEFAULT_ENCODING "UTF-8" )
endif ( NOT DEFAULT_ENCODING )

if ( NOT BUILD_QT_VERSION )
    set (BUILD_QT_VERSION 4)
endif ( NOT BUILD_QT_VERSION )

if ( BUILD_QT_VERSION STREQUAL 4 )
    set (QT4_BUILD 1)
    set (QT5_BUILD 0)
    set (PROJ_PREFIX qt4)
else ( BUILD_QT_VERSION STREQUAL 4 )
    set (QT4_BUILD 0)
    set (QT5_BUILD 1)
    set (PROJ_PREFIX qt5)
endif ( BUILD_QT_VERSION STREQUAL 4 )

message (STATUS "Build with QT version: ${BUILD_QT_VERSION}")
message (STATUS "QT4 build: ${QT4_BUILD}")
message (STATUS "QT5 build: ${QT5_BUILD}")
##

IF (NOT LIB_SUFFIX)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "LIB_SUFFIX variable is not defined. It will be autodetected now.")
    MESSAGE(STATUS "You can set it manually with -DLIB_SUFFIX=<value> (64 for example)")
    IF (NOT APPLE)
        # check 64 bit
        if (CMAKE_SIZEOF_VOID_P EQUAL 4)
            SET (LIB_SUFFIX "")
        else (CMAKE_SIZEOF_VOID_P EQUAL 4)
            set (LIB_SUFFIX "64")
            MESSAGE(STATUS "   LIB_SUFFIX is set to '${LIB_SUFFIX}'")
        endif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    ELSE (NOT APPLE)
        message(STATUS "LIB_SUFFIX is disabled for APPLE: ${CMAKE_SYSTEM_NAME}")
        SET (LIB_SUFFIX "")
    ENDIF (NOT APPLE)
    # BSD does not use lib64 for 64bit libs
    IF (${CMAKE_SYSTEM_NAME} MATCHES "(FreeBSD|OpenBSD)")
        message(STATUS "LIB_SUFFIX is disabled for *BSD: ${CMAKE_SYSTEM_NAME}")
        SET (LIB_SUFFIX "")
    ENDIF()
    #
    MESSAGE(STATUS "")
ENDIF (NOT LIB_SUFFIX)

set (PROJECT_NAME ${PROJ_PREFIX}-virt-manager)
set (PROJECT_VERSION 1.0.0)
set (HEADERS 
  src/mainwindow.h
  src/log_dock/log_dock.h
  src/tray/traywidget.h
  src/toolbar/toolbar.h
  src/settings/conn_settings.h
  src/wait_thread/wait_thread.h
  
  src/layout/conn_list_widget.h
  src/layout/conn_menu.h
  src/layout/conn_item_index.h
  src/layout/conn_item_model.h
  src/layout/progressbar_delegate.h

  src/dock_head_widgets/dock_head_widget.h
  src/dock_head_widgets/bar_name.h
  src/dock_head_widgets/dock_widget.h
  
  src/virt_objects/open_file_menu.h
  src/virt_objects/index.h
  src/virt_objects/model.h
  src/virt_objects/control_thread.h
  src/virt_objects/virt_entity_control.h
  
  src/virt_objects/virt_connect/conn_element.h
  src/virt_objects/virt_connect/conn_alive_thread.h
  
  src/virt_objects/virt_domain/domain_control.h
  src/virt_objects/virt_domain/domain_model.h
  src/virt_objects/virt_domain/domain_toolbar.h
  src/virt_objects/virt_domain/domain_control_menu.h
  src/virt_objects/virt_domain/domain_control_thread.h
  src/virt_objects/virt_domain/migrate_dialog.h
  
  src/virt_objects/virt_network/virtnet_control.h
  src/virt_objects/virt_network/virtnet_model.h
  src/virt_objects/virt_network/virtnet_toolbar.h
  src/virt_objects/virt_network/virtnet_control_menu.h
  src/virt_objects/virt_network/net_control_thread.h
  
  src/virt_objects/virt_storage_pool/storage_pool_control.h
  src/virt_objects/virt_storage_pool/storage_pool_model.h
  src/virt_objects/virt_storage_pool/storage_pool_toolbar.h
  src/virt_objects/virt_storage_pool/storage_pool_control_thread.h
  src/virt_objects/virt_storage_pool/storage_pool_control_menu.h
  src/virt_objects/virt_storage_pool/delete_pool_menu.h
  
  src/virt_objects/virt_storage_vol/storage_vol_control.h
  src/virt_objects/virt_storage_vol/storage_vol_index.h
  src/virt_objects/virt_storage_vol/storage_vol_model.h
  src/virt_objects/virt_storage_vol/storage_vol_toolbar.h
  src/virt_objects/virt_storage_vol/storage_vol_control_thread.h
  src/virt_objects/virt_storage_vol/storage_vol_control_menu.h
  src/virt_objects/virt_storage_vol/wipe_menu.h
  src/virt_objects/virt_storage_vol/resize_dialog.h
  
  src/create_widgets/domain/domain_widgets.h
  src/create_widgets/domain/_qwidget.h
  src/create_widgets/domain/_changed.h
  src/create_widgets/domain/restore_panel.h
  src/create_widgets/domain/create_virt_domain.h
  src/create_widgets/domain/_tab.h
  src/create_widgets/domain/common_widgets/os_booting.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/_arch.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/boot_menu.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/boot_devices.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/path_to_file.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/boot_type.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/bios_boot.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/host_boot.h
  src/create_widgets/domain/common_widgets/os_boot_widgets/direct_kernel_boot.h
  src/create_widgets/domain/common_widgets/general.h
  src/create_widgets/domain/common_widgets/memory.h
  src/create_widgets/domain/common_widgets/memory_widgets/hugepages.h
  src/create_widgets/domain/common_widgets/cpu.h
  src/create_widgets/domain/common_widgets/cpu_widgets/cpu_allocation.h
  src/create_widgets/domain/common_widgets/cpu_widgets/logical_host_cpu.h
  src/create_widgets/domain/common_widgets/cpu_widgets/cpu_model.h
  src/create_widgets/domain/common_widgets/cpu_widgets/cpu_topology.h
  src/create_widgets/domain/common_widgets/devices.h
  src/create_widgets/domain/common_widgets/devices_widgets/device_data.h
  src/create_widgets/domain/common_widgets/devices_widgets/device_existance_menu.h
  src/create_widgets/domain/common_widgets/devices_widgets/device_stack.h
  src/create_widgets/domain/common_widgets/devices_widgets/device_restore_menu.h
  src/create_widgets/domain/common_widgets/char_device.h
  src/create_widgets/domain/common_widgets/char_dev_widgets/pty_widget.h
  src/create_widgets/domain/common_widgets/char_dev_widgets/dev_widget.h
  src/create_widgets/domain/common_widgets/char_dev_widgets/file_widget.h
  src/create_widgets/domain/common_widgets/char_dev_widgets/tcp_widget.h
  src/create_widgets/domain/common_widgets/char_dev_widgets/udp_widget.h
  src/create_widgets/domain/common_widgets/char_dev_widgets/unix_widget.h
  src/create_widgets/domain/common_widgets/channel_device.h
  src/create_widgets/domain/common_widgets/console_device.h
  src/create_widgets/domain/common_widgets/smartcard_device.h
  src/create_widgets/domain/common_widgets/input_device.h
  src/create_widgets/domain/common_widgets/device_address.h
  src/create_widgets/domain/common_widgets/address_widgets/_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/ccid_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/ccw_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/drive_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/isa_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/pci_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/spaprvio_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/usb_addr.h
  src/create_widgets/domain/common_widgets/address_widgets/virtioserial_addr.h
  src/create_widgets/domain/common_widgets/hub_device.h
  src/create_widgets/domain/common_widgets/video_device.h
  src/create_widgets/domain/common_widgets/sound_device.h
  src/create_widgets/domain/common_widgets/host_device.h
  src/create_widgets/domain/common_widgets/host_dev_widgets/usb_host_device.h
  src/create_widgets/domain/common_widgets/host_dev_widgets/pci_host_device.h
  src/create_widgets/domain/common_widgets/host_dev_widgets/scsi_host_device.h
  src/create_widgets/domain/common_widgets/host_dev_widgets/bch_host_device.h
  src/create_widgets/domain/common_widgets/graphics_device.h
  src/create_widgets/domain/common_widgets/graph_dev_widgets/sdl_graphics.h
  src/create_widgets/domain/common_widgets/graph_dev_widgets/vnc_graphics.h
  src/create_widgets/domain/common_widgets/graph_dev_widgets/rdp_graphics.h
  src/create_widgets/domain/common_widgets/graph_dev_widgets/desktop_graphics.h
  src/create_widgets/domain/common_widgets/graph_dev_widgets/spice_graphics.h
  src/create_widgets/domain/common_widgets/redirdev_device.h
  src/create_widgets/domain/common_widgets/redir_filter.h
  src/create_widgets/domain/common_widgets/filesystems.h
  src/create_widgets/domain/common_widgets/fs_type_widgets/_fstype.h
  src/create_widgets/domain/common_widgets/fs_type_widgets/mount_fstype.h
  src/create_widgets/domain/common_widgets/fs_type_widgets/file_fstype.h
  src/create_widgets/domain/common_widgets/fs_type_widgets/block_fstype.h
  src/create_widgets/domain/common_widgets/fs_type_widgets/ram_fstype.h
  src/create_widgets/domain/common_widgets/fs_type_widgets/bind_fstype.h
  src/create_widgets/domain/common_widgets/net_interfaces.h
  src/create_widgets/domain/common_widgets/net_if_widgets/mac_address.h
  src/create_widgets/domain/common_widgets/net_if_widgets/nic_model.h
  src/create_widgets/domain/common_widgets/net_if_widgets/virtual_port.h
  src/create_widgets/domain/common_widgets/net_if_widgets/multicast_tunnel.h
  src/create_widgets/domain/common_widgets/net_if_widgets/tcp_tunnel.h
  src/create_widgets/domain/common_widgets/net_if_widgets/pci_passthrough.h
  src/create_widgets/domain/common_widgets/net_if_widgets/direct_attachment.h
  src/create_widgets/domain/common_widgets/net_if_widgets/generic_ethernet.h
  src/create_widgets/domain/common_widgets/net_if_widgets/userspace_slirp.h
  src/create_widgets/domain/common_widgets/net_if_widgets/bridge_to_lan.h
  src/create_widgets/domain/common_widgets/net_if_widgets/virtual_network.h
  src/create_widgets/domain/common_widgets/disk.h
  src/create_widgets/domain/common_widgets/disk_widgets/_disk.h
  src/create_widgets/domain/common_widgets/disk_widgets/_target.h
  src/create_widgets/domain/common_widgets/disk_widgets/_device.h
  src/create_widgets/domain/common_widgets/disk_widgets/_read_only.h
  src/create_widgets/domain/common_widgets/disk_widgets/_startup_policy.h
  src/create_widgets/domain/common_widgets/disk_widgets/_hosts.h
  src/create_widgets/domain/common_widgets/disk_widgets/virtvolume_dialog.h
  src/create_widgets/domain/common_widgets/disk_widgets/block_disk.h
  src/create_widgets/domain/common_widgets/disk_widgets/dir_disk.h
  src/create_widgets/domain/common_widgets/disk_widgets/file_disk.h
  src/create_widgets/domain/common_widgets/disk_widgets/network_disk.h
  src/create_widgets/domain/common_widgets/disk_widgets/volume_disk.h
  src/create_widgets/domain/common_widgets/security_label.h
  src/create_widgets/domain/common_widgets/sec_label_widgets/sec_labels.h
  src/create_widgets/domain/common_widgets/encryption.h
  src/create_widgets/domain/common_widgets/emulator.h
  src/create_widgets/domain/common_widgets/random.h
  src/create_widgets/domain/common_widgets/memballoon.h
  src/create_widgets/domain/common_widgets/misc_settings.h
  src/create_widgets/domain/common_widgets/misc_settings_widgets/events.h
  src/create_widgets/domain/common_widgets/misc_settings_widgets/power.h
  src/create_widgets/domain/common_widgets/misc_settings_widgets/hv_features.h
  src/create_widgets/storage/_create_storage_widgets/_create_storage.h
  src/create_widgets/storage/_create_storage_widgets/_storage_source.h
  src/create_widgets/storage/_create_storage_widgets/_adapter.h
  src/create_widgets/storage/_create_storage_widgets/_auth.h
  src/create_widgets/storage/_create_storage_widgets/_block_devices.h
  src/create_widgets/storage/_create_storage_widgets/_storage_target.h
  src/create_widgets/storage/_create_storage_widgets/_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/dir_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/fs_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/logical_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/netfs_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/disk_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/iscsi_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/gluster_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/mpath_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/rbd_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/scsi_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/sheepdog_pool_stuff.h
  src/create_widgets/storage/_create_storage_widgets/zfs_pool_stuff.h
  src/create_widgets/storage/create_pool.h
  src/create_widgets/storage/create_volume.h
  src/create_widgets/domain/edit_widgets/emulator_edit.h
  src/create_widgets/domain/edit_widgets/hub_device_edit.h
  src/create_widgets/domain/edit_widgets/smartcard_device_edit.h
  src/create_widgets/domain/edit_widgets/char_device_edit.h
  src/create_widgets/domain/edit_widgets/sound_device_edit.h
  src/create_widgets/domain/edit_widgets/console_device_edit.h
  src/create_widgets/domain/edit_widgets/input_device_edit.h
  src/create_widgets/domain/edit_widgets/memballoon_edit.h
  src/create_widgets/domain/edit_widgets/redirdev_device_edit.h
  src/create_widgets/domain/edit_widgets/video_device_edit.h
  src/create_widgets/domain/edit_widgets/random_edit.h
  src/create_widgets/domain/edit_widgets/host_device_edit.h
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/usb_host_device_edit.h
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/bch_host_device_edit.h
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/pci_host_device_edit.h
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/scsi_host_device_edit.h
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/adapter_address.h
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/iscsi_device.h
  src/create_widgets/domain/edit_widgets/graphics_device_edit.h
  src/create_widgets/domain/edit_widgets/net_interfaces_edit.h
  src/create_widgets/domain/edit_widgets/filesystems_edit.h
  src/create_widgets/domain/edit_widgets/disk_edit.h
  src/create_widgets/domain/lxc_widgets/os_booting.h
  
  src/create_widgets/network/create_virt_network.h
  src/create_widgets/network/network_widgets/_checked_widget.h
  src/create_widgets/network/network_widgets/_list_widget.h
  src/create_widgets/network/network_widgets/bridge_widget.h
  src/create_widgets/network/network_widgets/domain_widget.h
  src/create_widgets/network/network_widgets/addressing_widget.h
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widget.h
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/forwarder.h
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/host_dns.h
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/txt_dns.h
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/srv_dns.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widget.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/addtab.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipvx.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv4.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv6.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ip_widget.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/dhcp_widget.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/dhcp_widgets/dhcp_host.h
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/dhcp_widgets/dhcp_range.h
  src/create_widgets/network/network_widgets/addressing_widgets/mac_widget.h
  src/create_widgets/network/network_widgets/forward_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/nat_mode_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/route_mode_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/bridge_mode_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/private_mode_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/vepa_mode_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/passthrough_mode_widget.h
  src/create_widgets/network/network_widgets/forward_mode_widgets/hostdev_mode_widget.h

  src/vm_viewer/vm_viewer.h
  src/vm_viewer/viewer_toolbar.h

  src/vm_viewer/lxc/lxc_viewer.h

  src/state_monitor/domain_state_monitor.h
  src/state_monitor/domain_monitor_thread.h
  src/state_monitor/domain_state_viewer.h
  src/state_monitor/svg_tamplates.h

  src/task_warehouse/task_warehouse.h
  
  src/vm_viewer/qterminal/tabwidget.h
  src/vm_viewer/qterminal/termwidgetholder.h
  src/vm_viewer/qterminal/config.h
  src/vm_viewer/qterminal/fontdialog.h
  src/vm_viewer/qterminal/bookmarkswidget.h
  src/vm_viewer/qterminal/mainwindow.h
  src/vm_viewer/qterminal/properties.h
  src/vm_viewer/qterminal/termwidget.h
  src/vm_viewer/qterminal/propertiesdialog.h
)
set (SOURCES 
  src/main.cpp
  src/mainwindow.cpp
  src/log_dock/log_dock.cpp
  src/tray/traywidget.cpp
  src/toolbar/toolbar.cpp
  src/settings/conn_settings.cpp
  src/wait_thread/wait_thread.cpp
  
  src/layout/conn_list_widget.cpp
  src/layout/conn_menu.cpp
  src/layout/conn_item_index.cpp
  src/layout/conn_item_model.cpp
  src/layout/progressbar_delegate.cpp

  src/dock_head_widgets/dock_head_widget.cpp
  src/dock_head_widgets/bar_name.cpp
  src/dock_head_widgets/dock_widget.cpp
  
  src/virt_objects/open_file_menu.cpp
  src/virt_objects/index.cpp
  src/virt_objects/model.cpp
  src/virt_objects/control_thread.cpp
  src/virt_objects/virt_entity_control.cpp
  
  src/virt_objects/virt_connect/conn_element.cpp
  src/virt_objects/virt_connect/conn_alive_thread.cpp
  
  src/virt_objects/virt_domain/domain_control.cpp
  src/virt_objects/virt_domain/domain_model.cpp
  src/virt_objects/virt_domain/domain_toolbar.cpp
  src/virt_objects/virt_domain/domain_control_thread.cpp
  src/virt_objects/virt_domain/domain_control_menu.cpp
  src/virt_objects/virt_domain/migrate_dialog.cpp
  
  src/virt_objects/virt_network/virtnet_control.cpp
  src/virt_objects/virt_network/virtnet_model.cpp
  src/virt_objects/virt_network/virtnet_toolbar.cpp
  src/virt_objects/virt_network/net_control_thread.cpp
  src/virt_objects/virt_network/virtnet_control_menu.cpp
  
  src/virt_objects/virt_storage_pool/storage_pool_control.cpp
  src/virt_objects/virt_storage_pool/storage_pool_model.cpp
  src/virt_objects/virt_storage_pool/storage_pool_toolbar.cpp
  src/virt_objects/virt_storage_pool/storage_pool_control_thread.cpp
  src/virt_objects/virt_storage_pool/storage_pool_control_menu.cpp
  src/virt_objects/virt_storage_pool/delete_pool_menu.cpp
  
  src/virt_objects/virt_storage_vol/storage_vol_control.cpp
  src/virt_objects/virt_storage_vol/storage_vol_index.cpp
  src/virt_objects/virt_storage_vol/storage_vol_model.cpp
  src/virt_objects/virt_storage_vol/storage_vol_toolbar.cpp
  src/virt_objects/virt_storage_vol/storage_vol_control_thread.cpp
  src/virt_objects/virt_storage_vol/storage_vol_control_menu.cpp
  src/virt_objects/virt_storage_vol/wipe_menu.cpp
  src/virt_objects/virt_storage_vol/resize_dialog.cpp
  
  src/create_widgets/domain/create_virt_domain.cpp
  src/create_widgets/domain/_qwidget.cpp
  src/create_widgets/domain/_changed.cpp
  src/create_widgets/domain/restore_panel.cpp
  src/create_widgets/domain/_tab.cpp
  src/create_widgets/domain/common_widgets/os_booting.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/_arch.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/boot_menu.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/boot_devices.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/path_to_file.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/boot_type.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/bios_boot.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/host_boot.cpp
  src/create_widgets/domain/common_widgets/os_boot_widgets/direct_kernel_boot.cpp
  src/create_widgets/domain/common_widgets/general.cpp
  src/create_widgets/domain/common_widgets/memory.cpp
  src/create_widgets/domain/common_widgets/memory_widgets/hugepages.cpp
  src/create_widgets/domain/common_widgets/cpu.cpp
  src/create_widgets/domain/common_widgets/cpu_widgets/cpu_allocation.cpp
  src/create_widgets/domain/common_widgets/cpu_widgets/logical_host_cpu.cpp
  src/create_widgets/domain/common_widgets/cpu_widgets/cpu_model.cpp
  src/create_widgets/domain/common_widgets/cpu_widgets/cpu_topology.cpp
  src/create_widgets/domain/common_widgets/devices.cpp
  src/create_widgets/domain/common_widgets/devices_widgets/device_data.cpp
  src/create_widgets/domain/common_widgets/devices_widgets/device_existance_menu.cpp
  src/create_widgets/domain/common_widgets/devices_widgets/device_stack.cpp
  src/create_widgets/domain/common_widgets/devices_widgets/device_restore_menu.cpp
  src/create_widgets/domain/common_widgets/char_device.cpp
  src/create_widgets/domain/common_widgets/char_dev_widgets/pty_widget.cpp
  src/create_widgets/domain/common_widgets/char_dev_widgets/dev_widget.cpp
  src/create_widgets/domain/common_widgets/char_dev_widgets/file_widget.cpp
  src/create_widgets/domain/common_widgets/char_dev_widgets/tcp_widget.cpp
  src/create_widgets/domain/common_widgets/char_dev_widgets/udp_widget.cpp
  src/create_widgets/domain/common_widgets/char_dev_widgets/unix_widget.cpp
  src/create_widgets/domain/common_widgets/channel_device.cpp
  src/create_widgets/domain/common_widgets/console_device.cpp
  src/create_widgets/domain/common_widgets/smartcard_device.cpp
  src/create_widgets/domain/common_widgets/input_device.cpp
  src/create_widgets/domain/common_widgets/device_address.cpp
  src/create_widgets/domain/common_widgets/address_widgets/_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/ccid_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/ccw_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/drive_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/isa_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/pci_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/spaprvio_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/usb_addr.cpp
  src/create_widgets/domain/common_widgets/address_widgets/virtioserial_addr.cpp
  src/create_widgets/domain/common_widgets/hub_device.cpp
  src/create_widgets/domain/common_widgets/video_device.cpp
  src/create_widgets/domain/common_widgets/sound_device.cpp
  src/create_widgets/domain/common_widgets/host_device.cpp
  src/create_widgets/domain/common_widgets/host_dev_widgets/usb_host_device.cpp
  src/create_widgets/domain/common_widgets/host_dev_widgets/pci_host_device.cpp
  src/create_widgets/domain/common_widgets/host_dev_widgets/scsi_host_device.cpp
  src/create_widgets/domain/common_widgets/host_dev_widgets/bch_host_device.cpp
  src/create_widgets/domain/common_widgets/graphics_device.cpp
  src/create_widgets/domain/common_widgets/graph_dev_widgets/sdl_graphics.cpp
  src/create_widgets/domain/common_widgets/graph_dev_widgets/vnc_graphics.cpp
  src/create_widgets/domain/common_widgets/graph_dev_widgets/rdp_graphics.cpp
  src/create_widgets/domain/common_widgets/graph_dev_widgets/desktop_graphics.cpp
  src/create_widgets/domain/common_widgets/graph_dev_widgets/spice_graphics.cpp
  src/create_widgets/domain/common_widgets/redirdev_device.cpp
  src/create_widgets/domain/common_widgets/redir_filter.cpp
  src/create_widgets/domain/common_widgets/filesystems.cpp
  src/create_widgets/domain/common_widgets/fs_type_widgets/_fstype.cpp
  src/create_widgets/domain/common_widgets/fs_type_widgets/mount_fstype.cpp
  src/create_widgets/domain/common_widgets/fs_type_widgets/file_fstype.cpp
  src/create_widgets/domain/common_widgets/fs_type_widgets/block_fstype.cpp
  src/create_widgets/domain/common_widgets/fs_type_widgets/ram_fstype.cpp
  src/create_widgets/domain/common_widgets/fs_type_widgets/bind_fstype.cpp
  src/create_widgets/domain/common_widgets/net_interfaces.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/mac_address.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/nic_model.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/virtual_port.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/multicast_tunnel.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/tcp_tunnel.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/pci_passthrough.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/direct_attachment.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/generic_ethernet.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/userspace_slirp.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/bridge_to_lan.cpp
  src/create_widgets/domain/common_widgets/net_if_widgets/virtual_network.cpp
  src/create_widgets/domain/common_widgets/disk.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/_disk.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/_target.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/_device.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/_read_only.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/_startup_policy.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/_hosts.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/virtvolume_dialog.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/block_disk.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/dir_disk.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/file_disk.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/network_disk.cpp
  src/create_widgets/domain/common_widgets/disk_widgets/volume_disk.cpp
  src/create_widgets/domain/common_widgets/security_label.cpp
  src/create_widgets/domain/common_widgets/sec_label_widgets/sec_labels.cpp
  src/create_widgets/domain/common_widgets/encryption.cpp
  src/create_widgets/domain/common_widgets/emulator.cpp
  src/create_widgets/domain/common_widgets/random.cpp
  src/create_widgets/domain/common_widgets/memballoon.cpp
  src/create_widgets/domain/common_widgets/misc_settings.cpp
  src/create_widgets/domain/common_widgets/misc_settings_widgets/events.cpp
  src/create_widgets/domain/common_widgets/misc_settings_widgets/power.cpp
  src/create_widgets/domain/common_widgets/misc_settings_widgets/hv_features.cpp
  src/create_widgets/storage/_create_storage_widgets/_create_storage.cpp
  src/create_widgets/storage/_create_storage_widgets/_storage_source.cpp
  src/create_widgets/storage/_create_storage_widgets/_adapter.cpp
  src/create_widgets/storage/_create_storage_widgets/_auth.cpp
  src/create_widgets/storage/_create_storage_widgets/_block_devices.cpp
  src/create_widgets/storage/_create_storage_widgets/_storage_target.cpp
  src/create_widgets/storage/_create_storage_widgets/_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/dir_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/fs_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/logical_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/netfs_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/disk_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/iscsi_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/gluster_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/mpath_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/rbd_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/scsi_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/sheepdog_pool_stuff.cpp
  src/create_widgets/storage/_create_storage_widgets/zfs_pool_stuff.cpp
  src/create_widgets/storage/create_pool.cpp
  src/create_widgets/storage/create_volume.cpp
  src/create_widgets/domain/edit_widgets/emulator_edit.cpp
  src/create_widgets/domain/edit_widgets/hub_device_edit.cpp
  src/create_widgets/domain/edit_widgets/smartcard_device_edit.cpp
  src/create_widgets/domain/edit_widgets/char_device_edit.cpp
  src/create_widgets/domain/edit_widgets/sound_device_edit.cpp
  src/create_widgets/domain/edit_widgets/console_device_edit.cpp
  src/create_widgets/domain/edit_widgets/input_device_edit.cpp
  src/create_widgets/domain/edit_widgets/memballoon_edit.cpp
  src/create_widgets/domain/edit_widgets/redirdev_device_edit.cpp
  src/create_widgets/domain/edit_widgets/video_device_edit.cpp
  src/create_widgets/domain/edit_widgets/random_edit.cpp
  src/create_widgets/domain/edit_widgets/host_device_edit.cpp
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/usb_host_device_edit.cpp
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/bch_host_device_edit.cpp
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/pci_host_device_edit.cpp
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/scsi_host_device_edit.cpp
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/adapter_address.cpp
  src/create_widgets/domain/edit_widgets/host_dev_edit_widgets/iscsi_device.cpp
  src/create_widgets/domain/edit_widgets/graphics_device_edit.cpp
  src/create_widgets/domain/edit_widgets/net_interfaces_edit.cpp
  src/create_widgets/domain/edit_widgets/filesystems_edit.cpp
  src/create_widgets/domain/edit_widgets/disk_edit.cpp
  src/create_widgets/domain/lxc_widgets/os_booting.cpp
  
  src/create_widgets/network/create_virt_network.cpp
  src/create_widgets/network/network_widgets/_checked_widget.cpp
  src/create_widgets/network/network_widgets/_list_widget.cpp
  src/create_widgets/network/network_widgets/bridge_widget.cpp
  src/create_widgets/network/network_widgets/domain_widget.cpp
  src/create_widgets/network/network_widgets/addressing_widget.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widget.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/forwarder.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/host_dns.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/txt_dns.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/dns_widgets/srv_dns.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widget.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/addtab.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipvx.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv4.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv6.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ip_widget.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/dhcp_widget.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/dhcp_widgets/dhcp_host.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/ip_widgets/dhcp_widgets/dhcp_range.cpp
  src/create_widgets/network/network_widgets/addressing_widgets/mac_widget.cpp
  src/create_widgets/network/network_widgets/forward_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/nat_mode_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/route_mode_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/bridge_mode_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/private_mode_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/vepa_mode_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/passthrough_mode_widget.cpp
  src/create_widgets/network/network_widgets/forward_mode_widgets/hostdev_mode_widget.cpp

  src/vm_viewer/vm_viewer.cpp
  src/vm_viewer/viewer_toolbar.cpp

  src/vm_viewer/lxc/lxc_viewer.cpp

  src/state_monitor/domain_state_monitor.cpp
  src/state_monitor/domain_monitor_thread.cpp
  src/state_monitor/domain_state_viewer.cpp

  src/task_warehouse/task_warehouse.cpp
  
  src/vm_viewer/qterminal/properties.cpp
  src/vm_viewer/qterminal/bookmarkswidget.cpp
  src/vm_viewer/qterminal/tabwidget.cpp
  src/vm_viewer/qterminal/termwidgetholder.cpp
  #src/vm_viewer/qterminal/main.cpp
  src/vm_viewer/qterminal/propertiesdialog.cpp
  src/vm_viewer/qterminal/mainwindow.cpp
  src/vm_viewer/qterminal/termwidget.cpp
  src/vm_viewer/qterminal/fontdialog.cpp
)
set (SOURCES_DIRS src)
set (DESKTOP_FILE qt-virt-manager.desktop)

project (${PROJECT_NAME})
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${SOURCES_DIRS})

set(QTERM_UI_SRC
    src/vm_viewer/qterminal/forms/qterminal.ui
    src/vm_viewer/qterminal/forms/propertiesdialog.ui
    src/vm_viewer/qterminal/forms/bookmarkswidget.ui
    src/vm_viewer/qterminal/forms/fontdialog.ui
)
set(QTERM_RCC_SRC
    src/vm_viewer/qterminal/icons.qrc
)
set(QTERM_TS
    src/vm_viewer/qterminal/translations/qterminal_cs.ts
    src/vm_viewer/qterminal/translations/qterminal_de.ts
    src/vm_viewer/qterminal/translations/qterminal_es.ts
    src/vm_viewer/qterminal/translations/qterminal_et.ts
    src/vm_viewer/qterminal/translations/qterminal_it.ts
    src/vm_viewer/qterminal/translations/qterminal_pt.ts
    src/vm_viewer/qterminal/translations/qterminal_ru.ts
)

##
if ( QT4_BUILD )
    include("CMakeQt4.txt")
else ( QT4_BUILD )
    include("CMakeQt5.txt")
endif( QT4_BUILD )
##

find_package(LibVirt REQUIRED)
include_directories(${LibVirt_INCLUDE_DIRS})
list(APPEND BUILD_PROJECT_LIBRARIES ${LibVirt_LIBRARIES})

# * build & install application & files
add_executable (${PROJECT_NAME}
    ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UI_SRCS} ${QTERM_RCC} ${QTERM_QM})
if ( QT4_BUILD )
    target_link_libraries (${PROJECT_NAME}
        ${BUILD_PROJECT_LIBRARIES}
        util
        qtermwidget4
    )
else ( QT4_BUILD )
    target_link_libraries (${PROJECT_NAME}
        ${BUILD_PROJECT_LIBRARIES}
        util
        qtermwidget5
    )
endif( QT4_BUILD )

configure_file ( ${DESKTOP_FILE}.in ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE} )
install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}
    DESTINATION ${SHARE_INSTALL_PREFIX}/applications
    RENAME ${PROJECT_NAME}.desktop )
install ( TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
install ( DIRECTORY src/misc src/icons
    DESTINATION ${SHARE_INSTALL_PREFIX}/${PROJECT_NAME} )
# * end
