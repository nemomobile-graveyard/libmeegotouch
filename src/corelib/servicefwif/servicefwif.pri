###############################################################################
# DuiServiceFwIf module
# This module contains classes needed for applications to comminucate with
# the servicemapper
############################################################################### 
SERVICEFWIF_SRC_DIR=./servicefwif
INCLUDEPATH+=./servicefwif/include
PUBLIC_HEADERS += \
    $$SERVICEFWIF_SRC_DIR/duiservicefwbaseif.h \
    $$SERVICEFWIF_SRC_DIR/include/duiservicefwproxy.h \

PRIVATE_HEADERS += \
    $$SERVICEFWIF_SRC_DIR/duiservicefwbaseif_p.h \

SOURCES += \
    $$SERVICEFWIF_SRC_DIR/duiservicefwbaseif.cpp \
    $$SERVICEFWIF_SRC_DIR/duiservicefwproxy.cpp \

