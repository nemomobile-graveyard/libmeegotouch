SERVICE_SRC_DIR = ./service
INCLUDEPATH += ./service

PUBLIC_HEADERS += \
    $$SERVICE_SRC_DIR/duiserviceaction.h \
    $$SERVICE_SRC_DIR/duiserviceinvoker.h \

PRIVATE_HEADERS += \
    $$SERVICE_SRC_DIR/duiserviceaction_p.h \

SOURCES += \
    $$SERVICE_SRC_DIR/duiserviceaction.cpp \
    $$SERVICE_SRC_DIR/duiserviceinvoker.cpp
