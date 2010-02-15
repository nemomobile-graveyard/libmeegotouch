###############################################################################
# DuiServiceFwIf module
# This module contains classes needed for applications to comminucate with
# the servicemapper
############################################################################### 
SERVICEFWIF_SRC_DIR=./servicefwif
INCLUDEPATH+=./servicefwif/include
HEADERS += \
    $$SERVICEFWIF_SRC_DIR/duiservicefwbaseif.h \
    $$SERVICEFWIF_SRC_DIR/include/duiservicefwproxy.h \

SOURCES += \
    $$SERVICEFWIF_SRC_DIR/duiservicefwbaseif.cpp \
    $$SERVICEFWIF_SRC_DIR/duiservicefwproxy.cpp \

