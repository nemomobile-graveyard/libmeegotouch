include(../common_top.pri)
TARGET = ut_mwidgetrectangularclipanimation

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/corelib/theme \
               $$MSRCDIR/corelib/effects \
               $$MSRCDIR/corelib/animation \
               $$MSRCDIR/corelib/animation/core \
               $$MSRCDIR/corelib/animation/widget \
               $$MSRCDIR/corelib/animation/widget/core \

STYLE_HEADERS += \
                $$MSRCDIR/corelib/style/mstyle.h \
                $$MSRCDIR/corelib/style/manimationstyle.h \
                $$MSRCDIR/corelib/style/mabstractwidgetanimationstyle.h \
                $$MSRCDIR/corelib/style/mwidgetrectangularclipanimationstyle.h \

SOURCES += \
           $$MSRCDIR/corelib/style/mstyle.cpp \
           $$MSRCDIR/corelib/animation/widget/core/mabstractwidgetanimation.cpp \
           $$MSRCDIR/corelib/animation/widget/mwidgetrectangularclipanimation.cpp \
           $$MSRCDIR/corelib/effects/mgraphicsclipeffect.cpp \
           ut_mwidgetrectangularclipanimation.cpp \

HEADERS  += \
           $$MSRCDIR/corelib/style/mstyle.h \
           $$MSRCDIR/corelib/style/mstyle_p.h \
           $$MSRCDIR/corelib/style/manimationstyle.h \
           $$MSRCDIR/corelib/animation/widget/mabstractwidgetanimation.h \
           $$MSRCDIR/corelib/animation/widget/mabstractwidgetanimation_p.h \
           $$MSRCDIR/corelib/effects/mgraphicsclipeffect.h \
           $$MSRCDIR/corelib/effects/mgraphicsclipeffect_p.h \
           $$MSRCDIR/corelib/style/mabstractwidgetanimationstyle.h \
           $$MSRCDIR/corelib/animation/widget/mwidgetrectangularclipanimation.h \
           $$MSRCDIR/corelib/animation/widget/mwidgetrectangularclipanimation_p.h \
           $$MSRCDIR/corelib/style/mwidgetrectangularclipanimationstyle.h \
           ut_mwidgetrectangularclipanimation.h \

#include(../common_bot.pri)
