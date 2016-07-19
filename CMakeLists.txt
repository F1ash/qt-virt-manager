project(qt-virt-manager CXX)

# TODO: make a cross platform here
if (NOT CMAKE_INSTALL_PREFIX)
    set (CMAKE_INSTALL_PREFIX /usr)
endif ()
if (NOT SHARE_INSTALL_PREFIX)
    set (SHARE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    
list(APPEND CMAKE_PREFIX_PATH
    # added for brew build in Travis,
    # because don't have default path
    # for new installed bundles in OS_X
    ${QT5_LIB_PATH}
    ${VNC_LIB_PATH}
    ${SPICE_LIB_PATH})

cmake_minimum_required (VERSION 2.8.3)
include(CheckFunctionExists)
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

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
set (VERSION_MAJOR   0)
set (VERSION_MIDDLE  34)
set (VERSION_MINOR   50)
set (HEADERS
  src/mainwindow.h
  src/log_dock/log_dock.h
  src/log_dock/text_browser.h
  src/tray/traywidget.h
  src/settings/conn_settings.h
  src/wait_thread/wait_thread.h

  src/proxy_widget/proxy_widget.h
  src/soft_touched/st_stacked_widget.h

  src/menubar/menu_bar.h
  src/menubar/file_menu.h
  src/menubar/view_menu.h
  src/menubar/dock_menu.h
  src/menubar/help_menu.h
  src/menubar/donate_dialog.h

  src/dock_head_widgets/dock_head_widget.h
  src/dock_head_widgets/bar_name.h
  src/dock_head_widgets/dock_widget.h
  
  src/virt_objects/open_file_menu.h
  src/virt_objects/index.h
  src/virt_objects/model.h
  src/virt_objects/control_thread.h
  src/virt_objects/virt_entity_control.h
  src/virt_objects/virt_entity_config.h
  src/virt_objects/_virt_thread.h
  src/virt_objects/virt_event_loop.h
  src/virt_objects/tree_view.h
  src/virt_objects/h_navigation_label.h
  
  src/virt_objects/virt_connect/conn_element.h
  src/virt_objects/virt_connect/conn_alive_thread.h
  src/virt_objects/virt_connect/connection_list/connections.h
  src/virt_objects/virt_connect/connection_list/toolbar.h
  src/virt_objects/virt_connect/connection_list/conn_list_widget.h
  src/virt_objects/virt_connect/connection_list/conn_menu.h
  src/virt_objects/virt_connect/connection_list/conn_item_index.h
  src/virt_objects/virt_connect/connection_list/conn_item_model.h
  src/virt_objects/virt_connect/connection_list/conn_list_delegate.h
  src/virt_objects/virt_connect/connection_list/search_thread.h
  src/virt_objects/virt_connect/connection_list/wait_local_conn.h
  
  src/virt_objects/virt_domain/domain_control.h
  src/virt_objects/virt_domain/domain_model.h
  src/virt_objects/virt_domain/domain_toolbar.h
  src/virt_objects/virt_domain/domain_control_menu.h
  src/virt_objects/virt_domain/domain_control_thread.h
  src/virt_objects/virt_domain/migrate_dialog.h
  src/virt_objects/virt_domain/domain_list_delegate.h
  
  src/virt_objects/virt_network/virtnet_control.h
  src/virt_objects/virt_network/virtnet_model.h
  src/virt_objects/virt_network/virtnet_toolbar.h
  src/virt_objects/virt_network/virtnet_control_menu.h
  src/virt_objects/virt_network/net_control_thread.h
  src/virt_objects/virt_network/net_list_delegate.h
  
  src/virt_objects/virt_storage_pool/storage_pool_control.h
  src/virt_objects/virt_storage_pool/storage_pool_model.h
  src/virt_objects/virt_storage_pool/storage_pool_toolbar.h
  src/virt_objects/virt_storage_pool/storage_pool_control_thread.h
  src/virt_objects/virt_storage_pool/storage_pool_control_menu.h
  src/virt_objects/virt_storage_pool/delete_pool_menu.h
  src/virt_objects/virt_storage_pool/storage_pool_list_delegate.h
  
  src/virt_objects/virt_storage_vol/storage_vol_control.h
  src/virt_objects/virt_storage_vol/storage_vol_index.h
  src/virt_objects/virt_storage_vol/storage_vol_model.h
  src/virt_objects/virt_storage_vol/storage_vol_toolbar.h
  src/virt_objects/virt_storage_vol/storage_vol_control_thread.h
  src/virt_objects/virt_storage_vol/storage_vol_control_menu.h
  src/virt_objects/virt_storage_vol/wipe_menu.h
  src/virt_objects/virt_storage_vol/resize_dialog.h

  src/virt_objects/virt_secret/secret_control.h
  src/virt_objects/virt_secret/secret_control_thread.h
  src/virt_objects/virt_secret/secret_control_menu.h
  src/virt_objects/virt_secret/secret_toolbar.h
  src/virt_objects/virt_secret/secret_model.h
  src/virt_objects/virt_secret/secret_index.h
  src/virt_objects/virt_secret/secret_list_delegate.h

  src/virt_objects/virt_interface/virt_interface_control.h
  src/virt_objects/virt_interface/virt_interface_toolbar.h
  src/virt_objects/virt_interface/virt_interface_control_thread.h
  src/virt_objects/virt_interface/virt_interface_model.h
  src/virt_objects/virt_interface/virt_interface_index.h
  src/virt_objects/virt_interface/virt_interface_control_menu.h
  src/virt_objects/virt_interface/interface_list_delegate.h

  src/virt_objects/virt_nwfilter/nwfilter_control.h
  src/virt_objects/virt_nwfilter/nwfilter_control_thread.h
  src/virt_objects/virt_nwfilter/nwfilter_control_menu.h
  src/virt_objects/virt_nwfilter/nwfilter_toolbar.h
  src/virt_objects/virt_nwfilter/nwfilter_model.h
  src/virt_objects/virt_nwfilter/nwfilter_index.h
  src/virt_objects/virt_nwfilter/nwfilter_list_delegate.h

  src/create_widgets/domain/domain_widgets.h
  src/create_widgets/domain/_qwidget.h
  src/create_widgets/domain/_qwidget_threaded.h
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
  src/create_widgets/domain/common_widgets/disk_widgets/_use_encryption.h
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
  src/create_widgets/storage/_create_storage_widgets/find_secret_dialog.h
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
  src/create_widgets/domain/lxc_widgets/lxc_os_booting.h
  
  src/create_widgets/network/create_virt_network.h
  src/create_widgets/network/create_virt_network_ass.h
  src/create_widgets/network/network_widgets/assistant_widgets/introduce_page.h
  src/create_widgets/network/network_widgets/assistant_widgets/bridge_page.h
  src/create_widgets/network/network_widgets/assistant_widgets/forward_page.h
  src/create_widgets/network/network_widgets/assistant_widgets/ip_page.h
  src/create_widgets/network/network_widgets/assistant_widgets/no_gateway_page.h
  src/create_widgets/network/network_widgets/assistant_widgets/conclusion_page.h
  src/create_widgets/network/create_virt_network_adv.h
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
  src/create_widgets/network/network_widgets/qos_widget.h
  src/create_widgets/network/network_widgets/qos_widgets/bound_widget.h

  src/create_widgets/snapshot/create_snapshot_dialog.h
  src/create_widgets/snapshot/create_snapshot_flags.h
  src/create_widgets/snapshot/delete_snapshot_flags.h
  src/create_widgets/snapshot/revert_snapshot_flags.h
  src/create_widgets/snapshot/_snapshot_flags.h
  src/create_widgets/snapshot/_snapshot_stuff.h
  src/create_widgets/snapshot/_disks.h
  src/create_widgets/snapshot/_disk_item.h
  src/create_widgets/snapshot/set_disks_data_thread.h
  src/create_widgets/snapshot/types/mem_state_snapshot.h
  src/create_widgets/snapshot/types/disk_snapshot.h
  src/create_widgets/snapshot/types/system_checkpoint.h
  src/create_widgets/snapshot/snapshot_action_dialog.h
  src/create_widgets/snapshot/model/snapshot_tree_model.h
  src/create_widgets/snapshot/model/snapshot_tree_index.h

  src/create_widgets/secret/create_virt_secret.h
  src/create_widgets/secret/secret_widgets/volume_sec_type.h
  src/create_widgets/secret/secret_widgets/ceph_sec_type.h
  src/create_widgets/secret/secret_widgets/iscsi_sec_type.h
  src/create_widgets/secret/secret_widgets/_sec_type.h

  src/create_widgets/interface/create_interface.h

  src/create_widgets/nwfilter/create_virt_nwfilter.h

  src/vm_viewer/vm_viewer.h
  src/vm_viewer/viewer_toolbar.h
  src/vm_viewer/vm_state_widget.h
  src/vm_viewer/transformation_mode_menu.h

  src/vm_viewer/lxc/lxc_viewer.h
  src/vm_viewer/lxc/lxc_viewer_thread.h
  src/vm_viewer/spice/spice_viewer.h
  src/vm_viewer/vnc/vnc_viewer.h
  src/vm_viewer/click_label.h
  
  src/vm_viewer/qterminal/tabwidget.h
  src/vm_viewer/qterminal/termwidgetholder.h
  src/vm_viewer/qterminal/config.h
  src/vm_viewer/qterminal/fontdialog.h
  src/vm_viewer/qterminal/bookmarkswidget.h
  src/vm_viewer/qterminal/term_mainwindow.h
  src/vm_viewer/qterminal/properties.h
  src/vm_viewer/qterminal/termwidget.h
  src/vm_viewer/qterminal/propertiesdialog.h

  src/vm_viewer/qspice_widgets/qspice-channel.h
  src/vm_viewer/qspice_widgets/qspice-cursor-channel.h
  src/vm_viewer/qspice_widgets/qspice-display-channel.h
  src/vm_viewer/qspice_widgets/qspice-helper.h
  src/vm_viewer/qspice_widgets/qspice-inputs-channel.h
  src/vm_viewer/qspice_widgets/qspice-main-channel.h
  src/vm_viewer/qspice_widgets/qspice-object.h
  src/vm_viewer/qspice_widgets/qspice-session.h
  src/vm_viewer/qspice_widgets/qspice-widget.h
  src/vm_viewer/qspice_widgets/qspice-smartcard-channel.h
  src/vm_viewer/qspice_widgets/qspice-smartcard-manager.h
  src/vm_viewer/qspice_widgets/qspice-smartcard-widget.h
  src/vm_viewer/qspice_widgets/qspice-usbredir-channel.h
  src/vm_viewer/qspice_widgets/qspice-webdav-channel.h
  src/vm_viewer/qspice_widgets/qspice-port-channel.h
  src/vm_viewer/qspice_widgets/qspice-usbdevice-manager.h
  src/vm_viewer/qspice_widgets/qspice-usbdevice-widget.h
  src/vm_viewer/qspice_widgets/qspice-playback-channel.h
  src/vm_viewer/qspice_widgets/qspice-record-channel.h
  src/vm_viewer/qspice_widgets/qspice-audio.h

  src/vm_viewer/krdc_vnc_qtonly/Machine_View.h
  src/vm_viewer/krdc_vnc_qtonly/remoteview.h
  src/vm_viewer/krdc_vnc_qtonly/vncclientthread.h
  src/vm_viewer/krdc_vnc_qtonly/vncview.h

  src/state_monitor/domain_state_monitor.h
  src/state_monitor/domain_monitor_thread.h
  src/state_monitor/domain_state_viewer.h
  src/state_monitor/svg_templates.h

  src/task_warehouse/task_warehouse.h
)
set (SOURCES 
  src/main.cpp
  src/mainwindow.cpp
  src/log_dock/log_dock.cpp
  src/log_dock/text_browser.cpp
  src/tray/traywidget.cpp
  src/settings/conn_settings.cpp
  src/wait_thread/wait_thread.cpp

  src/proxy_widget/proxy_widget.cpp
  src/soft_touched/st_stacked_widget.cpp

  src/menubar/menu_bar.cpp
  src/menubar/file_menu.cpp
  src/menubar/view_menu.cpp
  src/menubar/dock_menu.cpp
  src/menubar/help_menu.cpp
  src/menubar/donate_dialog.cpp

  src/dock_head_widgets/dock_head_widget.cpp
  src/dock_head_widgets/bar_name.cpp
  src/dock_head_widgets/dock_widget.cpp
  
  src/virt_objects/open_file_menu.cpp
  src/virt_objects/index.cpp
  src/virt_objects/model.cpp
  src/virt_objects/control_thread.cpp
  src/virt_objects/virt_entity_control.cpp
  src/virt_objects/_virt_thread.cpp
  src/virt_objects/virt_event_loop.cpp
  src/virt_objects/tree_view.cpp
  src/virt_objects/h_navigation_label.cpp
  
  src/virt_objects/virt_connect/conn_element.cpp
  src/virt_objects/virt_connect/conn_alive_thread.cpp
  src/virt_objects/virt_connect/connection_list/connections.cpp
  src/virt_objects/virt_connect/connection_list/toolbar.cpp
  src/virt_objects/virt_connect/connection_list/conn_list_widget.cpp
  src/virt_objects/virt_connect/connection_list/conn_menu.cpp
  src/virt_objects/virt_connect/connection_list/conn_item_index.cpp
  src/virt_objects/virt_connect/connection_list/conn_item_model.cpp
  src/virt_objects/virt_connect/connection_list/conn_list_delegate.cpp
  src/virt_objects/virt_connect/connection_list/search_thread.cpp
  src/virt_objects/virt_connect/connection_list/wait_local_conn.cpp
  
  src/virt_objects/virt_domain/domain_control.cpp
  src/virt_objects/virt_domain/domain_model.cpp
  src/virt_objects/virt_domain/domain_toolbar.cpp
  src/virt_objects/virt_domain/domain_control_thread.cpp
  src/virt_objects/virt_domain/domain_control_menu.cpp
  src/virt_objects/virt_domain/migrate_dialog.cpp
  src/virt_objects/virt_domain/domain_list_delegate.cpp
  
  src/virt_objects/virt_network/virtnet_control.cpp
  src/virt_objects/virt_network/virtnet_model.cpp
  src/virt_objects/virt_network/virtnet_toolbar.cpp
  src/virt_objects/virt_network/net_control_thread.cpp
  src/virt_objects/virt_network/virtnet_control_menu.cpp
  src/virt_objects/virt_network/net_list_delegate.cpp
  
  src/virt_objects/virt_storage_pool/storage_pool_control.cpp
  src/virt_objects/virt_storage_pool/storage_pool_model.cpp
  src/virt_objects/virt_storage_pool/storage_pool_toolbar.cpp
  src/virt_objects/virt_storage_pool/storage_pool_control_thread.cpp
  src/virt_objects/virt_storage_pool/storage_pool_control_menu.cpp
  src/virt_objects/virt_storage_pool/delete_pool_menu.cpp
  src/virt_objects/virt_storage_pool/storage_pool_list_delegate.cpp
  
  src/virt_objects/virt_storage_vol/storage_vol_control.cpp
  src/virt_objects/virt_storage_vol/storage_vol_index.cpp
  src/virt_objects/virt_storage_vol/storage_vol_model.cpp
  src/virt_objects/virt_storage_vol/storage_vol_toolbar.cpp
  src/virt_objects/virt_storage_vol/storage_vol_control_thread.cpp
  src/virt_objects/virt_storage_vol/storage_vol_control_menu.cpp
  src/virt_objects/virt_storage_vol/wipe_menu.cpp
  src/virt_objects/virt_storage_vol/resize_dialog.cpp

  src/virt_objects/virt_secret/secret_control.cpp
  src/virt_objects/virt_secret/secret_control_thread.cpp
  src/virt_objects/virt_secret/secret_control_menu.cpp
  src/virt_objects/virt_secret/secret_toolbar.cpp
  src/virt_objects/virt_secret/secret_model.cpp
  src/virt_objects/virt_secret/secret_index.cpp
  src/virt_objects/virt_secret/secret_list_delegate.cpp

  src/virt_objects/virt_interface/virt_interface_control.cpp
  src/virt_objects/virt_interface/virt_interface_toolbar.cpp
  src/virt_objects/virt_interface/virt_interface_control_thread.cpp
  src/virt_objects/virt_interface/virt_interface_model.cpp
  src/virt_objects/virt_interface/virt_interface_index.cpp
  src/virt_objects/virt_interface/virt_interface_control_menu.cpp
  src/virt_objects/virt_interface/interface_list_delegate.cpp

  src/virt_objects/virt_nwfilter/nwfilter_control.cpp
  src/virt_objects/virt_nwfilter/nwfilter_control_thread.cpp
  src/virt_objects/virt_nwfilter/nwfilter_control_menu.cpp
  src/virt_objects/virt_nwfilter/nwfilter_toolbar.cpp
  src/virt_objects/virt_nwfilter/nwfilter_model.cpp
  src/virt_objects/virt_nwfilter/nwfilter_index.cpp
  src/virt_objects/virt_nwfilter/nwfilter_list_delegate.cpp
  
  src/create_widgets/domain/create_virt_domain.cpp
  src/create_widgets/domain/_qwidget.cpp
  src/create_widgets/domain/_qwidget_threaded.cpp
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
  src/create_widgets/domain/common_widgets/disk_widgets/_use_encryption.cpp
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
  src/create_widgets/storage/_create_storage_widgets/find_secret_dialog.cpp
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
  src/create_widgets/domain/lxc_widgets/lxc_os_booting.cpp
  
  src/create_widgets/network/create_virt_network.cpp
  src/create_widgets/network/create_virt_network_ass.cpp
  src/create_widgets/network/network_widgets/assistant_widgets/introduce_page.cpp
  src/create_widgets/network/network_widgets/assistant_widgets/bridge_page.cpp
  src/create_widgets/network/network_widgets/assistant_widgets/forward_page.cpp
  src/create_widgets/network/network_widgets/assistant_widgets/ip_page.cpp
  src/create_widgets/network/network_widgets/assistant_widgets/no_gateway_page.cpp
  src/create_widgets/network/network_widgets/assistant_widgets/conclusion_page.cpp
  src/create_widgets/network/create_virt_network_adv.cpp
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
  src/create_widgets/network/network_widgets/qos_widget.cpp
  src/create_widgets/network/network_widgets/qos_widgets/bound_widget.cpp

  src/create_widgets/snapshot/create_snapshot_dialog.cpp
  src/create_widgets/snapshot/create_snapshot_flags.cpp
  src/create_widgets/snapshot/delete_snapshot_flags.cpp
  src/create_widgets/snapshot/revert_snapshot_flags.cpp
  src/create_widgets/snapshot/_snapshot_flags.cpp
  src/create_widgets/snapshot/_snapshot_stuff.cpp
  src/create_widgets/snapshot/_disks.cpp
  src/create_widgets/snapshot/_disk_item.cpp
  src/create_widgets/snapshot/set_disks_data_thread.cpp
  src/create_widgets/snapshot/types/mem_state_snapshot.cpp
  src/create_widgets/snapshot/types/disk_snapshot.cpp
  src/create_widgets/snapshot/types/system_checkpoint.cpp
  src/create_widgets/snapshot/snapshot_action_dialog.cpp
  src/create_widgets/snapshot/model/snapshot_tree_model.cpp
  src/create_widgets/snapshot/model/snapshot_tree_index.cpp

  src/create_widgets/secret/create_virt_secret.cpp
  src/create_widgets/secret/secret_widgets/volume_sec_type.cpp
  src/create_widgets/secret/secret_widgets/ceph_sec_type.cpp
  src/create_widgets/secret/secret_widgets/iscsi_sec_type.cpp
  src/create_widgets/secret/secret_widgets/_sec_type.cpp

  src/create_widgets/interface/create_interface.cpp

  src/create_widgets/nwfilter/create_virt_nwfilter.cpp

  src/vm_viewer/vm_viewer.cpp
  src/vm_viewer/viewer_toolbar.cpp
  src/vm_viewer/vm_state_widget.cpp
  src/vm_viewer/transformation_mode_menu.cpp
  src/vm_viewer/click_label.cpp

  src/vm_viewer/lxc/lxc_viewer.cpp
  src/vm_viewer/lxc/lxc_viewer_thread.cpp
  src/vm_viewer/spice/spice_viewer.cpp
  src/vm_viewer/vnc/vnc_viewer.cpp

  src/vm_viewer/qterminal/properties.cpp
  src/vm_viewer/qterminal/bookmarkswidget.cpp
  src/vm_viewer/qterminal/tabwidget.cpp
  src/vm_viewer/qterminal/termwidgetholder.cpp
  src/vm_viewer/qterminal/propertiesdialog.cpp
  src/vm_viewer/qterminal/term_mainwindow.cpp
  src/vm_viewer/qterminal/termwidget.cpp
  src/vm_viewer/qterminal/fontdialog.cpp

  src/vm_viewer/qspice_widgets/qspice-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-cursor-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-display-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-helper.cpp
  src/vm_viewer/qspice_widgets/qspice-inputs-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-main-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-object.cpp
  src/vm_viewer/qspice_widgets/qspice-session.cpp
  src/vm_viewer/qspice_widgets/qspice-widget.cpp
  src/vm_viewer/qspice_widgets/qspice-smartcard-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-smartcard-manager.cpp
  src/vm_viewer/qspice_widgets/qspice-smartcard-widget.cpp
  src/vm_viewer/qspice_widgets/qspice-usbredir-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-port-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-webdav-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-usbdevice-manager.cpp
  src/vm_viewer/qspice_widgets/qspice-usbdevice-widget.cpp
  src/vm_viewer/qspice_widgets/qspice-playback-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-record-channel.cpp
  src/vm_viewer/qspice_widgets/qspice-audio.cpp

  src/vm_viewer/krdc_vnc_qtonly/Machine_View.cpp
  src/vm_viewer/krdc_vnc_qtonly/remoteview.cpp
  src/vm_viewer/krdc_vnc_qtonly/vncclientthread.cpp
  src/vm_viewer/krdc_vnc_qtonly/vncview.cpp

  src/state_monitor/domain_state_monitor.cpp
  src/state_monitor/domain_monitor_thread.cpp
  src/state_monitor/domain_state_viewer.cpp

  src/task_warehouse/task_warehouse.cpp
)
set (SOURCES_DIRS src)
set (DESKTOP_FILE qt-virt-manager.desktop)

project (${PROJECT_NAME})
include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SOURCES_DIRS})
configure_file (
    ${CMAKE_SOURCE_DIR}/src/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

set(QTERM_UI_SRC
    src/vm_viewer/qterminal/forms/qterminal.ui
    src/vm_viewer/qterminal/forms/propertiesdialog.ui
    src/vm_viewer/qterminal/forms/bookmarkswidget.ui
    src/vm_viewer/qterminal/forms/fontdialog.ui
)
set(QTERM_RCC_SRC
    #src/vm_viewer/qterminal/icons.qrc
    src/qt_virt_manager.qrc
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
    include("${CMAKE_MODULE_PATH}/CMakeQt4.txt")
    set(TERM_WIDGET_LIB qtermwidget4)
else ( QT4_BUILD )
    include("${CMAKE_MODULE_PATH}/CMakeQt5.txt")
    set(TERM_WIDGET_LIB qtermwidget5)
endif( QT4_BUILD )
##

if (NOT DEFINED WITH_LIBCACARD)
    set (WITH_LIBCACARD 0)
endif ()

find_package(GLib2                REQUIRED)
find_package(GObject              REQUIRED)
find_package(Gio                  REQUIRED)
find_package(Spice1               REQUIRED)
find_package(SpiceClientGLib2     REQUIRED)
if ( WITH_LIBCACARD )
    find_package(CACard           REQUIRED)
endif()
message(STATUS "Build with Libcacard: ${WITH_LIBCACARD}")
find_package(LibVirt REQUIRED)
find_package("${TERM_WIDGET_LIB}" REQUIRED)
find_package(LibVNCServer         REQUIRED)
include_directories(
    ${GLib2_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    ${CACard_INCLUDE_DIRS}
    ${Spice1_INCLUDE_DIRS}
    ${SPICE_CLIENT_GLIB2_INCLUDE_DIRS}
    ${LibVirt_INCLUDE_DIRS}
    ${QTERMWIDGET_INCLUDE_DIRS}
    ${LIBVNCCLIENT_INCLUDE_DIR}
)
list(APPEND BUILD_PROJECT_LIBRARIES
    ${LibVirt_LIBRARIES}
    ${QTERMWIDGET_LIBRARIES}
    ${GLib2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${GIO_LIBRARIES}
    ${CACard_LIBRARIES}
    ${Spice1_LIBRARIES}
    ${SPICE_CLIENT_GLIB2_LIBRARIES}
    ${LIBVNCCLIENT_LIBRARIES}
)
MESSAGE(STATUS "Found includes :
    ${GLib2_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
    ${CACard_INCLUDE_DIRS}
    ${Spice1_INCLUDE_DIRS}
    ${SPICE_CLIENT_GLIB2_INCLUDE_DIRS}
    ${LibVirt_INCLUDE_DIRS}
    ${QTERMWIDGET_INCLUDE_DIRS}
    ${LIBVNCCLIENT_INCLUDE_DIR}")
MESSAGE(STATUS "Found BUILD_PROJECT_LIBRARIES: ${BUILD_PROJECT_LIBRARIES}")

# * build & install application & files
if (NOT DEFINED USE_SPICE_AUDIO)
    set (USE_SPICE_AUDIO 1)
endif ()
message(STATUS "Build with SPICE_AUDIO: ${USE_SPICE_AUDIO}")
add_executable (${PROJECT_NAME}
    ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UI_SRCS} ${QTERM_RCC} ${QTERM_QM})
target_link_libraries (${PROJECT_NAME} ${BUILD_PROJECT_LIBRARIES})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS
    "-DWITH_LIBCACARD=${WITH_LIBCACARD} \
     -DUSE_SPICE_AUDIO=${USE_SPICE_AUDIO} \
     -DQSPICE_WIDGET_COMPILATION \
     -DQTONLY")

configure_file ( ${DESKTOP_FILE}.in ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE} )
install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}
    DESTINATION ${SHARE_INSTALL_PREFIX}/applications
    RENAME ${PROJECT_NAME}.desktop )
install ( TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
install ( DIRECTORY src/misc
    DESTINATION ${SHARE_INSTALL_PREFIX}/${PROJECT_NAME} )
install ( FILES src/icons/256x256/apps/virtual-engineering.png
    DESTINATION ${SHARE_INSTALL_PREFIX}/icons/hicolor/256x256/apps )
# * end
