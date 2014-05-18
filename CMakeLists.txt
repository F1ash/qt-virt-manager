# TODO: make a cross platform here
if (NOT CMAKE_INSTALL_PREFIX)
 set (CMAKE_INSTALL_PREFIX /usr)
endif ()

cmake_minimum_required (VERSION 2.8.3)

set (PROJECT_NAME qt-virt-manager)
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
  
  src/virt_objects/open_file_menu.h
  src/virt_objects/index.h
  src/virt_objects/model.h
  
  src/virt_objects/virt_connect/conn_element.h
  src/virt_objects/virt_connect/conn_callbacks.h
  src/virt_objects/virt_connect/conn_alive_thread.h
  
  src/virt_objects/virt_domain/domain_control.h
  src/virt_objects/virt_domain/domain_model.h
  src/virt_objects/virt_domain/domain_toolbar.h
  src/virt_objects/virt_domain/domain_control_menu.h
  src/virt_objects/virt_domain/domain_control_thread.h
  
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
  src/create_widgets/domain/create_virt_domain.h
  src/create_widgets/domain/common_widgets/general.h
  src/create_widgets/domain/common_widgets/memory.h
  src/create_widgets/domain/common_widgets/cpu.h
  src/create_widgets/domain/lxc_widgets/net_interface.h
  src/create_widgets/domain/lxc_widgets/os_booting.h
  
  src/create_widgets/network/create_virt_network.h

  src/vm_viewer/vm_viewer.h
  src/vm_viewer/viewer_toolbar.h

  src/vm_viewer/lxc/lxc_viewer.h

  # qtermwidget
  src/qtermwidget/lib/BlockArray.h
  src/qtermwidget/lib/History.h
  src/qtermwidget/lib/kpty_p.h
  src/qtermwidget/lib/kpty.h
  src/qtermwidget/lib/ScreenWindow.h
  src/qtermwidget/lib/CharacterColor.h

  src/qtermwidget/lib/Character.h
  src/qtermwidget/lib/k3process.h
  src/qtermwidget/lib/k3processcontroller.h
  src/qtermwidget/lib/LineFont.h
  src/qtermwidget/lib/Session.h
  src/qtermwidget/lib/ColorTables.h
  src/qtermwidget/lib/ShellCommand.h

  src/qtermwidget/lib/DefaultTranslatorText.h
  src/qtermwidget/lib/Pty.h
  src/qtermwidget/lib/TerminalCharacterDecoder.h

  src/qtermwidget/lib/Emulation.h
  src/qtermwidget/lib/KeyboardTranslator.h
  src/qtermwidget/lib/qtermwidget.h

  src/qtermwidget/lib/ExtendedDefaultTranslator.h
  src/qtermwidget/lib/TerminalDisplay.h
  src/qtermwidget/lib/konsole_wcwidth.h

  src/qtermwidget/lib/Filter.h
  src/qtermwidget/lib/Screen.h
  src/qtermwidget/lib/Vt102Emulation.h
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
  
  src/virt_objects/open_file_menu.cpp
  src/virt_objects/index.cpp
  src/virt_objects/model.cpp
  
  src/virt_objects/virt_connect/conn_element.cpp
  src/virt_objects/virt_connect/conn_callbacks.cpp
  src/virt_objects/virt_connect/conn_alive_thread.cpp
  
  src/virt_objects/virt_domain/domain_control.cpp
  src/virt_objects/virt_domain/domain_model.cpp
  src/virt_objects/virt_domain/domain_toolbar.cpp
  src/virt_objects/virt_domain/domain_control_thread.cpp
  src/virt_objects/virt_domain/domain_control_menu.cpp
  
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
  src/create_widgets/domain/common_widgets/general.cpp
  src/create_widgets/domain/common_widgets/memory.cpp
  src/create_widgets/domain/common_widgets/cpu.cpp
  src/create_widgets/domain/lxc_widgets/net_interface.cpp
  src/create_widgets/domain/lxc_widgets/os_booting.cpp
  
  src/create_widgets/network/create_virt_network.cpp

  src/vm_viewer/vm_viewer.cpp
  src/vm_viewer/viewer_toolbar.cpp

  src/vm_viewer/lxc/lxc_viewer.cpp

  # qtermwidget
  src/qtermwidget/lib/BlockArray.cpp
  src/qtermwidget/lib/History.cpp
  src/qtermwidget/lib/ScreenWindow.cpp
  src/qtermwidget/lib/k3processcontroller.cpp
  src/qtermwidget/lib/Session.cpp
  src/qtermwidget/lib/k3process.cpp
  src/qtermwidget/lib/LineFont.src
  src/qtermwidget/lib/ShellCommand.cpp
  src/qtermwidget/lib/Pty.cpp
  src/qtermwidget/lib/TerminalCharacterDecoder.cpp

  src/qtermwidget/lib/Emulation.cpp
  src/qtermwidget/lib/KeyboardTranslator.cpp
  src/qtermwidget/lib/qtermwidget.cpp
  src/qtermwidget/lib/TerminalDisplay.cpp
  src/qtermwidget/lib/konsole_wcwidth.cpp

  src/qtermwidget/lib/Filter.cpp
  src/qtermwidget/lib/Screen.cpp
  src/qtermwidget/lib/Vt102Emulation.cpp
  src/qtermwidget/lib/kpty.cpp

  src/qtermwidget/lib/default.keytab  
  src/qtermwidget/lib/kb-layouts/default.keytab
  src/qtermwidget/lib/kb-layouts/linux.keytab
  src/qtermwidget/lib/kb-layouts/vt420pc.keytab
)
set (SOURCES_DIRS src)
set (DESKTOP_FILE ${PROJECT_NAME}.desktop)

project (${PROJECT_NAME})

include_directories (${CMAKE_CURRENT_BINARY_DIR} ${SOURCES_DIRS})
find_package (Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
include (${QT_USE_FILE})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
set (BUILD_PROJECT_LIBRARIES ${QT_LIBRARIES})

# * build & install application & files
add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
target_link_libraries (${PROJECT_NAME}
    ${BUILD_PROJECT_LIBRARIES}
    virt
    util
)

install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install (FILES ${DESKTOP_FILE} DESTINATION ${SHARE_INSTALL_PREFIX}/applications)
install (DIRECTORY src/misc src/icons
  DESTINATION ${SHARE_INSTALL_PREFIX}/${PROJECT_NAME})
# * end
