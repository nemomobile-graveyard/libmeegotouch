include(../common_top.pri)

QT += xml

INCLUDEPATH += $$MSRCDIR/settings/settingslanguage

# unit test and unit classes
SOURCES += \
    ut_msettingslanguagewidget.cpp \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagewidget.cpp

# unit test and unit classes
HEADERS += \
    ut_msettingslanguagewidget.h \
    $$MSRCDIR/settings/settingslanguage/msettingslanguagewidget.h

include(../common_bot.pri)
