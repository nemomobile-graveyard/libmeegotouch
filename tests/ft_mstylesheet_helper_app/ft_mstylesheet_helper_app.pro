include(../common_top.pri)
TARGET = ft_mstylesheet_helper_app

STYLE_HEADERS += testobjectstyle.h

SOURCES += \
	ft_mstylesheet_helper_app.cpp

HEADERS += \
	testobjectstyle.h \
        $$STYLE_HEADERS

include(../common_bot.pri)
