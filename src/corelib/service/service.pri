SERVICE_SRC_DIR = ./service
INCLUDEPATH += ./service

PUBLIC_HEADERS += \
    $$SERVICE_SRC_DIR/mserviceaction.h \
    $$SERVICE_SRC_DIR/mserviceinvoker.h \

PRIVATE_HEADERS += \
    $$SERVICE_SRC_DIR/mserviceaction_p.h \

SOURCES += \
    $$SERVICE_SRC_DIR/mserviceaction.cpp \
    $$SERVICE_SRC_DIR/mserviceinvoker.cpp
