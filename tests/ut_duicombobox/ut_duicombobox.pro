
include(../common_top.pri)
TARGET = ut_duicombobox

# unit

# unit test and unit
SOURCES += \
    ut_duicombobox.cpp \

# unit test and unit
HEADERS += \
    ut_duicombobox.h

INCLUDEPATH += \
    ../../src/corelib/widgets/views \
    ../../src/corelib/widgets \
    ../../src/views \

include(../common_bot.pri)
