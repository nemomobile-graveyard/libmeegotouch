include(../common_top.pri)

TARGET = ut_mpannablewidget

support_files.files += \
    ut_mpannablewidget_image.svg \
    ut_mpannablewidget_template.css \

# Input
HEADERS += \
    ut_mpannablewidget.h \
#    $$STUBSDIR/mpannableviewport_stub.h \
#    $$STUBSDIR/mstyledescription_stub.h \
#    $$STUBSDIR/mstyle_stub.h \
#    $$STUBSDIR/mtheme_stub.h \

TEST_SOURCES += \
#    $$MSRCDIR/mpannablewidget.cpp \

SOURCES += ut_mpannablewidget.cpp \
#    stubbase.cpp \

include(../common_bot.pri)
