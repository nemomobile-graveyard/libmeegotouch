MROOT = ../../../../..
include($$MROOT/mkspecs/common.pri)

# for M_DBUS_INTERFACES_DIR
include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)

system($$M_BUILD_TREE/tools/m-servicefwgen -p com.nokia.TextProcessorInterface)

MLIB = $$MROOT/lib
MSIF = $$MROOT/examples/servicefw
MSFW = $$MROOT/mservicemapper
MSIFLIB = $$MSIF/lib
MSIFINCLUDE = $$MSIF/include
MSFWINCLUDE = $$MROOT/src/servicefwif

INCLUDEPATH += \
    $$MSFWINCLUDE \
    $$MSFWINCLUDE/include \
    $$MSIFINCLUDE \
    $$MROOT/src/include \

DEPENDPATH += $$INCLUDEPATH
TARGET = textprocessor
DESTDIR = $$MSIFLIB
TEMPLATE = lib
SOURCES += \
    textprocessorinterfaceproxy.cpp \
    textprocessorinterface.cpp \

HEADERS += \
    textprocessorinterfaceproxy.h \
    textprocessorinterface.h \

target.path += $$[QT_INSTALL_LIBS]

headers.target = .dummy
headers.commands = touch $$headers.target
headers.path = $$[QT_INSTALL_HEADERS]
headers.files = $$HEADERS

xml.target = .dummy
xml.commands = touch $$xml.target
xml.path = $$M_DBUS_INTERFACES_DIR
xml.files = com.nokia.TextProcessorInterface.xml

INSTALLS += \
    target \
    headers \
    xml \
