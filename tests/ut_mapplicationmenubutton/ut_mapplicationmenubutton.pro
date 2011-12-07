include(../common_top.pri)

TARGET = ut_mapplicationmenubutton

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/views/ \
               $$MSRCDIR/views/widgets \

SOURCES += \
    ut_mapplicationmenubutton.cpp \
    $$MSRCDIR/views/widgets/mapplicationmenubutton.cpp \
    $${M_BUILD_TREE}/src/views/.gen/gen_mapplicationmenubuttonmodeldata.cpp \

HEADERS += \
    ut_mapplicationmenubutton.h \
    $$MSRCDIR/views/widgets/mapplicationmenubutton.h \
    $$MSRCDIR/views/widgets/mapplicationmenubuttonmodel.h \

include(../common_bot.pri)
