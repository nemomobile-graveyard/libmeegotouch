SERVICE_SRC_DIR = ./service
INCLUDEPATH += ./service
HEADERS += $$SERVICE_SRC_DIR/duiactionprovider.h \
    $$SERVICE_SRC_DIR/duiserviceaction.h \
    $$SERVICE_SRC_DIR/duiserviceinvoker.h \
    $$SERVICE_SRC_DIR/duiserviceaction_p.h

SOURCES += $$SERVICE_SRC_DIR/duiactionprovider.cpp \
    $$SERVICE_SRC_DIR/duiserviceaction.cpp \
    $$SERVICE_SRC_DIR/duiserviceinvoker.cpp
