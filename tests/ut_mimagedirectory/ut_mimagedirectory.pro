include(../common_top.pri)

INCLUDEPATH += \
    $${M_SOURCE_TREE}/src/corelib/theme \
    $${M_SOURCE_TREE}/src/corelib/core

SOURCES += \
    ut_mimagedirectory.cpp \
    $${M_SOURCE_TREE}/src/corelib/theme/mimagedirectory.cpp

HEADERS += \
    ut_mimagedirectory.h \
    $${M_SOURCE_TREE}/src/corelib/theme/mimagedirectory.h

locale_files.files += ut_mimagedirectory-samples/*
locale_files.path = $$[QT_INSTALL_LIBS]/libmeegotouch-tests/ut_mimagedirectory-samples
INSTALLS += locale_files

include(../common_bot.pri)
