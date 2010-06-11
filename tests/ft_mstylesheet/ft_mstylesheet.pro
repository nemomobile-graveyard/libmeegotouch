include(../common_top.pri)
TARGET = ft_mstylesheet
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

STYLE_HEADERS += testobjectstyle.h testobject2style.h testobject3style.h

SOURCES += \
	ft_mstylesheet.cpp \
    ../../src/corelib/theme/mlogicalvalues.cpp \
    ../../src/corelib/style/mstylesheetattribute.cpp

HEADERS += \
	ft_mstylesheet.h \
	testobjectstyle.h \
	testobject2style.h \
	testobject3style.h \
    testwidget.h \
    testwidget2.h \
    testwidget3.h \
    ../../src/corelib/theme/mlogicalvalues.h \
    ../../src/corelib/style/mstylesheetattribute.h \
    $$STYLE_HEADERS

support_files.files += \
        *.css
	
include(../common_bot.pri)
