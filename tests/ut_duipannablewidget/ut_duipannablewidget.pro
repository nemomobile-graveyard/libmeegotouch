include(../common_top.pri)

TARGET = ut_duipannablewidget

support_files.files += \
    ut_duipannablewidget_image.svg \
    ut_duipannablewidget_template.css \

# Input
HEADERS += \
    ut_duipannablewidget.h \
#    $$STUBSDIR/duipannableviewport_stub.h \
#    $$STUBSDIR/duistyledescription_stub.h \
#    $$STUBSDIR/duistyle_stub.h \
#    $$STUBSDIR/duitheme_stub.h \

TEST_SOURCES += \
#    $$DUISRCDIR/duipannablewidget.cpp \

SOURCES += ut_duipannablewidget.cpp \
#    stubbase.cpp \

include(../common_bot.pri)
