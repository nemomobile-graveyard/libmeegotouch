include(../common_top.pri)

TARGET = ut_mapplicationmenubuttonview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views \
               $$MSRCDIR/views/widgets

SOURCES += \
    ut_mapplicationmenubuttonview.cpp \
    $$MSRCDIR/views/mbuttonview.cpp \
    $$MSRCDIR/views/mbuttondefaulttransition.cpp \
    $$MSRCDIR/views/mbuttonexpandingbackgroundtransition.cpp \
    $$MSRCDIR/views/mbuttonimplodingbackgroundtransition.cpp \
    $$MSRCDIR/views/mbuttontabtransition.cpp \
    $$MSRCDIR/views/mapplicationmenubuttonview.cpp \
    $$MSRCDIR/views/widgets/mapplicationmenubutton.cpp \

HEADERS += \
    ut_mapplicationmenubuttonview.h \
    $$MSRCDIR/views/mbuttontransition.h \
    $$MSRCDIR/views/mbuttondefaulttransition.h \
    $$MSRCDIR/views/mbuttonexpandingbackgroundtransition.h \
    $$MSRCDIR/views/mbuttonimplodingbackgroundtransition.h \
    $$MSRCDIR/views/mbuttontabtransition.h \
    $$MSRCDIR/views/mbuttonview.h \
    $$MSRCDIR/views/mbuttonview_p.h \
    $$MSRCDIR/views/mapplicationmenubuttonview.h \
    $$MSRCDIR/views/widgets/mapplicationmenubutton.h \
    $$MSRCDIR/views/widgets/mapplicationmenubuttonmodel.h \

include(../common_bot.pri)
