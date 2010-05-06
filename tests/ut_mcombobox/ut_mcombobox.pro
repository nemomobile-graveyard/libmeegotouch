
include(../common_top.pri)
TARGET = ut_mcombobox

# unit

# unit test and unit
SOURCES += \
    ut_mcombobox.cpp \

# unit test and unit
HEADERS += \
    ut_mcombobox.h

INCLUDEPATH += \
    ../../src/corelib/widgets/views \
    ../../src/corelib/widgets \
    ../../src/views \

include(../common_bot.pri)
