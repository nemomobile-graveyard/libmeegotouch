include(../common_top.pri)
INCLUDEPATH +=  \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/scene \
    $$MSRCDIR/corelib/core \

TARGET = ut_mpannableviewportscroller

# unit test and unit
SOURCES = \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/corelib/widgets/mpannablewidget.cpp \
    $$MSRCDIR/corelib/widgets/mpannableviewportlayout.cpp \
    $$MSRCDIR/corelib/widgets/mpannableviewport.cpp \
    $$MSRCDIR/corelib/core/mabstractscroller.cpp \
    $$MSRCDIR/corelib/core/mpannableviewportscroller.cpp \
    $$MSRCDIR/corelib/scene/mscene.cpp \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.cpp \
    ut_mpannableviewportscroller.cpp

# unit test and unit
HEADERS += \
    $$MSRCDIR/corelib/widgets/mpannableviewportlayout.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
    $$MSRCDIR/corelib/widgets/mpannablewidget.h \
    $$MSRCDIR/corelib/widgets/mpannableviewport.h \
    $$MSRCDIR/corelib/widgets/mpannableviewport_p.h \
    $$MSRCDIR/corelib/core/mabstractscroller.h \
    $$MSRCDIR/corelib/scene/mscene.h \
    $$MSRCDIR/corelib/scene/mscene_p.h \
    $$MSRCDIR/corelib/core/mpannableviewportscroller.h \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.h \
    ut_mpannableviewportscroller.h


include(../common_bot.pri)
