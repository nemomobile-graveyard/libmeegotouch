include(../common_top.pri)

TARGET = ut_duipannableviewport


support_files.files += \
    ut_duipannableviewport_image.svg \
    ut_duipannableviewport_template.css\

TEST_SOURCES += \ 

# unit test and unit classes
SOURCES += \
    ut_duipannableviewport.cpp \

# unit test and unit classes
HEADERS += \
    ut_duipannableviewport.h \

include(../common_bot.pri)
