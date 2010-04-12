###############################################################################
# Workspace module
# This module contains all classes that rather deal with the workspace   
# management.
############################################################################### 
WORKSPACE_SRC_DIR=./workspace
INCLUDEPATH+=./workspace

PUBLIC_HEADERS += $$WORKSPACE_SRC_DIR/mdeviceprofile.h

SOURCES += $$WORKSPACE_SRC_DIR/mdeviceprofile.cpp

devices.CONFIG += no_check_exist
devices.files = $$WORKSPACE_SRC_DIR/devices.conf
devices.path = $$M_INSTALL_SYSCONF/meegotouch

INSTALLS += devices
