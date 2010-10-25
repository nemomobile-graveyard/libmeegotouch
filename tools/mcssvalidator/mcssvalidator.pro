include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = mcssvalidator
INCLUDEPATH += . \
    ../../src/include \
    ../../src/corelib/notification

DEPENDPATH += $$INCLUDEPATH
LIBS += $$mAddLibrary(meegotouchcore)

SOURCES += mcssvalidator.cpp
target.path = $$M_INSTALL_BIN
INSTALLS += target
