include(../common_top.pri)

QT += xml

INCLUDEPATH += $$DUISRCDIR/settings/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagewidget.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagewidget.cpp

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagewidget.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagewidget.h

include(../common_bot.pri)
