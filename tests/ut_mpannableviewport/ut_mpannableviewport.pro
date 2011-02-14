include(../common_top.pri)
INCLUDEPATH +=  \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/scene \

TARGET = ut_mpannableviewport


support_files.files += \
    ut_mpannableviewport_image.svg \
    ut_mpannableviewport_template.css\

TEST_SOURCES += \ 

# unit test and unit classes
SOURCES += \
    ut_mpannableviewport.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidget.cpp \
    $$MSRCDIR/corelib/widgets/mpannablewidget.cpp \
    $$MSRCDIR/corelib/widgets/mpannableviewportlayout.cpp \
    $$MSRCDIR/corelib/widgets/mpannableviewport.cpp \
    $$MSRCDIR/corelib/scene/mscene.cpp \

# unit test and unit classes
HEADERS += \
    ut_mpannableviewport.h \
    $$MSRCDIR/corelib/widgets/mpannablewidget.h \
    $$MSRCDIR/corelib/widgets/mpannableviewportlayout.h \
    $$MSRCDIR/corelib/widgets/mpannableviewport.h \
    $$MSRCDIR/corelib/widgets/mpannableviewport_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
    $$MSRCDIR/corelib/scene/mscene.h \

support_files.files += \
    *.css \

include(../common_bot.pri)
