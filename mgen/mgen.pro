include(../mkspecs/common.pri)

TEMPLATE = app
TARGET = mgen
DESTDIR = $$M_BUILD_TREE/mgen
target.path = $$M_INSTALL_BIN
INCLUDEPATH += .
DEPENDPATH += $$INCLUDEPATH

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

# Dont generate mgen.app bundle for Mac OS X
macx:CONFIG -= app_bundle
CONFIG += \
    qt \

SOURCES += \
    mgen.cpp \
    mgen_processor.cpp \

HEADERS += \


INSTALLS += \
     target\

QMAKE_EXTRA_TARGETS += check
check.depends = $${TARGET}
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $${TARGET}
check-xml.commands = $$system(true)
