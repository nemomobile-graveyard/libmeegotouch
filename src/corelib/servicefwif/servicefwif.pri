###############################################################################
# ServiceFwIf module
# This module contains classes needed for applications to comminucate with
# the servicemapper
############################################################################### 
SERVICEFWIF_SRC_DIR=./servicefwif
INCLUDEPATH+=./servicefwif/include
PUBLIC_HEADERS += \
    $$SERVICEFWIF_SRC_DIR/mservicefwbaseif.h \
    $$SERVICEFWIF_SRC_DIR/include/mservicefwproxy.h \

PRIVATE_HEADERS += \
    $$SERVICEFWIF_SRC_DIR/mservicefwbaseif_p.h \

SOURCES += \
    $$SERVICEFWIF_SRC_DIR/mservicefwbaseif.cpp \
    $$SERVICEFWIF_SRC_DIR/mservicefwproxy.cpp \

