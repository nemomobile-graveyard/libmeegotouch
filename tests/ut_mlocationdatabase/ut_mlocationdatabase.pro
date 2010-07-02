include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/i18n

QT += xml

TARGET = ut_mlocationdatabase

HEADERS += ut_mlocationdatabase.h
SOURCES += ut_mlocationdatabase.cpp

support_files.files += \
    *.txt

include(../common_bot.pri)
