# TODO: make a cross platform here
if (NOT CMAKE_INSTALL_PREFIX)
 set (CMAKE_INSTALL_PREFIX /usr)
endif ()

cmake_minimum_required (VERSION 2.8.3)

set (PROJECT_NAME qt-virt-manager)
set (HELPER_NAME qt_virt_manager_helper)
set (PROJECT_VERSION 1.0.0)
set (HEADERS 
  src/mainwindow.h
  src/log_doc/log_doc.h
  src/tray/traywidget.h
  src/toolbar/toolbar.h
  src/settings/conn_settings.h
  src/wait_thread/wait_thread.h
  src/layout/conn_list_widget.h
  src/layout/conn_menu.h
  src/layout/conn_item_index.h
  src/layout/conn_item_model.h
  src/layout/progressbar_delegate.h
  src/virt_objects/virt_domain/domain_doc.h
  src/virt_objects/virt_connect/conn_element.h
  src/virt_objects/virt_connect/conn_alive_thread.h
  src/virt_objects/virt_network/virtnet_control.h
  src/virt_objects/virt_network/virtnet_model.h
  src/virt_objects/virt_network/virtnet_index.h
  src/virt_objects/virt_network/virtnet_toolbar.h
  src/virt_objects/virt_network/open_file_menu.h
  src/virt_objects/virt_network/virtnet_control_menu.h
  src/virt_objects/virt_network/net_control_thread.h
  src/virt_objects/virt_storage_vol/storage_vol_doc.h
  src/virt_objects/virt_storage_pool/storage_pool_doc.h
)
set (SOURCES 
  src/main.cpp
  src/mainwindow.cpp
  src/log_doc/log_doc.cpp
  src/tray/traywidget.cpp
  src/toolbar/toolbar.cpp
  src/settings/conn_settings.cpp
  src/wait_thread/wait_thread.cpp
  src/layout/conn_list_widget.cpp
  src/layout/conn_menu.cpp
  src/layout/conn_item_index.cpp
  src/layout/conn_item_model.cpp
  src/layout/progressbar_delegate.cpp
  src/virt_objects/virt_domain/domain_doc.cpp
  src/virt_objects/virt_connect/conn_element.cpp
  src/virt_objects/virt_connect/conn_alive_thread.cpp
  src/virt_objects/virt_network/virtnet_control.cpp
  src/virt_objects/virt_network/virtnet_model.cpp
  src/virt_objects/virt_network/virtnet_index.cpp
  src/virt_objects/virt_network/virtnet_toolbar.cpp
  src/virt_objects/virt_network/open_file_menu.cpp
  src/virt_objects/virt_network/net_control_thread.cpp
  src/virt_objects/virt_network/virtnet_control_menu.cpp
  src/virt_objects/virt_storage_vol/storage_vol_doc.cpp
  src/virt_objects/virt_storage_pool/storage_pool_doc.cpp
)
set (SOURCES_DIRS src)
set (DESKTOP_FILE ${PROJECT_NAME}.desktop)

project (${PROJECT_NAME})

include_directories (${CMAKE_CURRENT_BINARY_DIR} ${SOURCES_DIRS})
find_package (Qt4 COMPONENTS QtCore QtGui)
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
# * end
