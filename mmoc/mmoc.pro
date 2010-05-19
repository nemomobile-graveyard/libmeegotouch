include(../mkspecs/common.pri)

TEMPLATE = app
TARGET = mmoc
target.path = $$M_INSTALL_BIN
INCLUDEPATH += .
DEPENDPATH += $$INCLUDEPATH

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

win32-msvc*:{
    target.commands += copy release\mmoc.exe .
}

# Dont generate mmoc.app bundle for Mac OS X
macx:CONFIG -= app_bundle
CONFIG += \
    qt \

SOURCES += \
    mmoc.cpp \

HEADERS += \


INSTALLS += \
     target\

win32: {
    DEFINES += QT_MOC_PATH=\\\"\"moc.exe\"\\\"
} else {
    DEFINES += QT_MOC_PATH=\'$$quote(\"$$QMAKE_MOC\")\'
}

QMAKE_EXTRA_TARGETS += check
check.depends = $${TARGET}
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $${TARGET}
check-xml.commands = $$system(true)
