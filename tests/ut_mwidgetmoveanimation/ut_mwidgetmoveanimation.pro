include(../common_top.pri)
TARGET = ut_mwidgetmoveanimation

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/corelib/theme \
               $$MSRCDIR/corelib/animation \
               $$MSRCDIR/corelib/animation/core \
               $$MSRCDIR/corelib/animation/widget \
               $$MSRCDIR/corelib/animation/widget/core \

STYLE_HEADERS += \
                $$MSRCDIR/corelib/style/mstyle.h \
                $$MSRCDIR/corelib/style/manimationstyle.h \
                $$MSRCDIR/corelib/style/mwidgetmoveanimationstyle.h \

SOURCES += \
           $$MSRCDIR/corelib/style/mstyle.cpp \
           $$MSRCDIR/corelib/animation/widget/mwidgetmoveanimation.cpp \
           ut_mwidgetmoveanimation.cpp \

HEADERS  += \
           $$MSRCDIR/corelib/style/mstyle.h \
           $$MSRCDIR/corelib/style/mstyle_p.h \
           $$MSRCDIR/corelib/style/manimationstyle.h \
           $$MSRCDIR/corelib/animation/widget/mwidgetmoveanimation.h \
           $$MSRCDIR/corelib/animation/widget/mwidgetmoveanimation_p.h \
           $$MSRCDIR/corelib/style/mwidgetmoveanimationstyle.h \
           ut_mwidgetmoveanimation.h \

#include(../common_bot.pri)
