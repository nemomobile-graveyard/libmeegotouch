include(../common_top.pri)

SOURCES += \
    ut_mtheme.cpp \
#    $$MSRCDIR/mtheme.cpp \
#    $$MSRCDIR/mstyledata.cpp \
#    $$MSRCDIR/mstyledescription.cpp \

SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_mtheme.h \ 
#    $$MSRCDIR/mtheme.h \

HEADERS += \
    $$STUBSDIR/stubbase.h \

HEADERS += \
#    $$MSRCDIR/mapplicationmenuview.h \
#    $$MSRCDIR/mbuttoniconview.h \
#    $$MSRCDIR/mbuttonview.h \
#    $$MSRCDIR/mnavigationbarview.h \
#    $$MSRCDIR/mpixmaploader.h \
#    $$MSRCDIR/msliderdotview.h \
#    $$MSRCDIR/mstyle.h \
#    $$MSRCDIR/mstylesheet.h \
#    $$MSRCDIR/mstylesheetparser.h \
#    $$MSRCDIR/mtoolbarview.h \
#    $$MSRCDIR/mviewfactory.h \
#    $$MSRCDIR/mwidgetview.h \
    $$STUBSDIR/mapplicationmenuview_stub.h \
    $$STUBSDIR/mbuttoniconview_stub.h \
    $$STUBSDIR/mbuttonview_stub.h \
    $$STUBSDIR/mnavigationbarview_stub.h \
    $$STUBSDIR/mpixmaploader_stub.h \
    $$STUBSDIR/msliderdotview_stub.h \
    $$STUBSDIR/mstylesheetparser_stub.h \
    $$STUBSDIR/mstylesheet_stub.h \
    $$STUBSDIR/mstyle_stub.h \
    $$STUBSDIR/mtoolbarview_stub.h \
    $$STUBSDIR/mviewfactory_stub.h \
    $$STUBSDIR/mwidgetview_stub.h \

include(../common_bot.pri)
