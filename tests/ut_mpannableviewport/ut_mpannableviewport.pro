include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/widgets
TARGET = ut_mpannableviewport


support_files.files += \
    ut_mpannableviewport_image.svg \
    ut_mpannableviewport_template.css\

TEST_SOURCES += \ 

# unit test and unit classes
SOURCES += \
    ut_mpannableviewport.cpp \

# unit test and unit classes
HEADERS += \
    ut_mpannableviewport.h \

include(../common_bot.pri)
