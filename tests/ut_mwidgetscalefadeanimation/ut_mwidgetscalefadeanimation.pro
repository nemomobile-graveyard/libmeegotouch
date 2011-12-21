include(../common_top.pri)
TARGET = ut_mwidgetscalefadeanimation

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
                $$MSRCDIR/corelib/style/mabstractwidgetanimationstyle.h \
                $$MSRCDIR/corelib/style/mwidgetscalefadeanimationstyle.h \

SOURCES += \
           $$MSRCDIR/corelib/style/mstyle.cpp \
           $$MSRCDIR/corelib/animation/widget/core/mabstractwidgetanimation.cpp \
           $$MSRCDIR/corelib/animation/widget/mwidgetscalefadeanimation.cpp \
           ut_mwidgetscalefadeanimation.cpp \

HEADERS  += \
           $$MSRCDIR/corelib/style/mstyle.h \
           $$MSRCDIR/corelib/style/mstyle_p.h \
           $$MSRCDIR/corelib/style/manimationstyle.h \
           $$MSRCDIR/corelib/animation/widget/mabstractwidgetanimation.h \
           $$MSRCDIR/corelib/animation/widget/mabstractwidgetanimation_p.h \
           $$MSRCDIR/corelib/style/mabstractwidgetanimationstyle.h \
           $$MSRCDIR/corelib/animation/widget/mwidgetscalefadeanimation.h \
           $$MSRCDIR/corelib/animation/widget/mwidgetscalefadeanimation_p.h \
           $$MSRCDIR/corelib/style/mwidgetscalefadeanimationstyle.h \
           ut_mwidgetscalefadeanimation.h \

#include(../common_bot.pri)
