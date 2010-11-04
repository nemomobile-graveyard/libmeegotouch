include(../mkspecs/common.pri)

TEMPLATE = app
TARGET = mmoc
DESTDIR = $$M_BUILD_TREE/mmoc
target.path = $$M_INSTALL_BIN
INCLUDEPATH += .
DEPENDPATH += $$INCLUDEPATH

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

# Dont generate mmoc.app bundle for Mac OS X
macx:CONFIG -= app_bundle
CONFIG += \
    qt \
    
CONFIG += console

SOURCES += \
    mmoc.cpp \

HEADERS += \

mmoc_perl.files += mmoc.pl
mmoc_perl.path = $$M_INSTALL_BIN

INSTALLS += \
     target\
     mmoc_perl

win32: {
    DEFINES += QT_MOC_PATH=\\\"\"moc.exe\"\\\"
} else {
    DEFINES += QT_MOC_PATH=\'$$quote(\"$$QMAKE_MOC\")\'
}

win32 {
    !exists ($$M_BUILD_TREE/mmoc/mmoc.pl) {
        system (copy $$M_SOURCE_TREE\\mmoc\\mmoc.pl $$M_BUILD_TREE\\mmoc\\mmoc.pl)
    }
} else {
    !exists ($$M_BUILD_TREE/mmoc/mmoc.pl) {
        system (cp -f $$M_SOURCE_TREE/mmoc/mmoc.pl $$M_BUILD_TREE/mmoc/mmoc.pl)
    }
}

QMAKE_EXTRA_TARGETS += check
check.depends = $${TARGET}
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $${TARGET}
check-xml.commands = $$system(true)
