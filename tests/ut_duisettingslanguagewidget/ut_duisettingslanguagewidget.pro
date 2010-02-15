include(../common_top.pri)

QT += xml

TARGET = ut_duisettingslanguagewidget
INCLUDEPATH += $$DUISRCDIR/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagewidget.cpp \
    $$DUISRCDIR/settingslanguage/duisettingslanguagewidget.cpp

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagewidget.h \
    $$DUISRCDIR/settingslanguage/duisettingslanguagewidget.h

include(../common_bot.pri)
