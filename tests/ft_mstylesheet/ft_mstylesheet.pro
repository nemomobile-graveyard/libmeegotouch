include(../common_top.pri)
TARGET = ft_mstylesheet
QT += dbus svg network

LIBRARYPATH += $$MSRCDIR

INCLUDEPATH += $$MSRCDIR/corelib/core/

STYLE_HEADERS += testobjectstyle.h testobject2style.h testobject3style.h

SOURCES += \
	ft_mstylesheet.cpp \
    $$MSRCDIR/corelib/theme/mlogicalvalues.cpp \
    $$MSRCDIR/corelib/style/mstylesheetattribute.cpp \
    $$MSRCDIR/corelib/core/msystemdirectories.cpp \
    $$MSRCDIR/corelib/style/muniquestringcache.cpp

HEADERS += \
	ft_mstylesheet.h \
	testobjectstyle.h \
	testobject2style.h \
	testobject3style.h \
    testwidget.h \
    testwidget2.h \
    testwidget3.h \
    $$MSRCDIR/corelib/theme/mlogicalvalues.h \
    $$MSRCDIR/corelib/style/mstylesheetattribute.h \
    $$MSRCDIR/corelib/core/msystemdirectories.cpp \
    $$MSRCDIR/corelib/style/muniquestringcache.h \
    $$STYLE_HEADERS

support_files.files += \
        *.css
	
include(../common_bot.pri)
