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
  
  src/create_widgets/domain/domain_widgets.h
  src/create_widgets/domain/_qwidget.h
  src/create_widgets/domain/create_virt_domain.h
  src/create_widgets/domain/common_widgets/general.h
  src/create_widgets/domain/common_widgets/memory.h
  src/create_widgets/domain/common_widgets/cpu.h
  src/create_widgets/domain/lxc_widgets/os_booting.h
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
  
  src/create_widgets/domain/create_virt_domain.cpp
  src/create_widgets/domain/_qwidget.cpp
  src/create_widgets/domain/common_widgets/general.cpp
  src/create_widgets/domain/common_widgets/memory.cpp
  src/create_widgets/domain/common_widgets/cpu.cpp
  src/create_widgets/domain/lxc_widgets/os_booting.cpp
)
set (SOURCES_DIRS src)
set (DESKTOP_FILE ${PROJECT_NAME}.desktop)

project (${PROJECT_NAME})

include_directories (${CMAKE_CURRENT_BINARY_DIR} ${SOURCES_DIRS})
find_package (Qt4 COMPONENTS QtCore QtGui QtXml)
include (${QT_USE_FILE})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
set (BUILD_PROJECT_LIBRARIES ${QT_LIBRARIES})

# * build & install application & files
add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
target_link_libraries (${PROJECT_NAME}
    ${BUILD_PROJECT_LIBRARIES}
    virt
)

install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install (FILES ${DESKTOP_FILE} DESTINATION ${SHARE_INSTALL_PREFIX}/applications)
install (DIRECTORY src/misc src/icons
  DESTINATION ${SHARE_INSTALL_PREFIX}/${PROJECT_NAME})
# * end
