include(../common_top.pri)

TARGET = ut_mcolorcomboboxbutton

INCLUDEPATH += $$MSRCDIR/views/widgets \
    $$MSRCDIR/views \

SOURCES += \
    ut_mcolorcomboboxbutton.cpp \
    $$MSRCDIR/views/widgets/mcolorcomboboxbutton.cpp \
    $$MSRCDIR/views/mcolorwidget.cpp \
    $${M_BUILD_TREE}/src/views/.gen/gen_mcolorwidgetstyledata.cpp

HEADERS += \
    ut_mcolorcomboboxbutton.h \
    $$MSRCDIR/views/widgets/mcolorcomboboxbutton.h \
    $$MSRCDIR/views/mcolorwidget.h \
    $$MSRCDIR/views/style/mcolorwidgetstyle.h

include(../common_bot.pri)
