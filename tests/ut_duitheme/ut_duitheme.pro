include(../common_top.pri)

SOURCES += \
    ut_duitheme.cpp \
#    $$DUISRCDIR/duitheme.cpp \
#    $$DUISRCDIR/duistyledata.cpp \
#    $$DUISRCDIR/duistyledescription.cpp \

SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_duitheme.h \ 
#    $$DUISRCDIR/duitheme.h \

HEADERS += \
    $$STUBSDIR/stubbase.h \

HEADERS += \
#    $$DUISRCDIR/duiapplicationmenuview.h \
#    $$DUISRCDIR/duibuttoniconview.h \
#    $$DUISRCDIR/duibuttonview.h \
#    $$DUISRCDIR/duinavigationbarview.h \
#    $$DUISRCDIR/duipixmaploader.h \
#    $$DUISRCDIR/duisliderdotview.h \
#    $$DUISRCDIR/duistyle.h \
#    $$DUISRCDIR/duistylesheet.h \
#    $$DUISRCDIR/duistylesheetparser.h \
#    $$DUISRCDIR/duitoolbarview.h \
#    $$DUISRCDIR/duiviewfactory.h \
#    $$DUISRCDIR/duiwidgetview.h \
    $$STUBSDIR/duiapplicationmenuview_stub.h \
    $$STUBSDIR/duibuttoniconview_stub.h \
    $$STUBSDIR/duibuttonview_stub.h \
    $$STUBSDIR/duinavigationbarview_stub.h \
    $$STUBSDIR/duipixmaploader_stub.h \
    $$STUBSDIR/duisliderdotview_stub.h \
    $$STUBSDIR/duistylesheetparser_stub.h \
    $$STUBSDIR/duistylesheet_stub.h \
    $$STUBSDIR/duistyle_stub.h \
    $$STUBSDIR/duitoolbarview_stub.h \
    $$STUBSDIR/duiviewfactory_stub.h \
    $$STUBSDIR/duiwidgetview_stub.h \

include(../common_bot.pri)
