###############################################################################
# DuiWorkspace module
# This module contains all classes that rather deal with the workspace   
# management.
############################################################################### 
WORKSPACE_SRC_DIR=./workspace
INCLUDEPATH+=./workspace
HEADERS += $$WORKSPACE_SRC_DIR/duideviceprofile.h
SOURCES += $$WORKSPACE_SRC_DIR/duideviceprofile.cpp

devices.CONFIG += no_check_exist
devices.files = $$WORKSPACE_SRC_DIR/devices.conf
devices.path = $$DUI_INSTALL_SYSCONF/dui

INSTALLS += devices
