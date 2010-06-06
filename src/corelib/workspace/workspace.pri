###############################################################################
# Workspace module
# This module contains all classes that rather deal with the workspace   
# management.
############################################################################### 
WORKSPACE_SRC_DIR=./workspace
INCLUDEPATH+=./workspace

PUBLIC_HEADERS += $$WORKSPACE_SRC_DIR/mdeviceprofile.h

SOURCES += $$WORKSPACE_SRC_DIR/mdeviceprofile.cpp
