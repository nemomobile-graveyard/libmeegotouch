
include(../common_top.pri)
TARGET = ut_mcombobox

# unit

# unit test and unit
SOURCES += \
    ut_mcombobox.cpp \
    ../../src/views/mcomboboxview.cpp

# unit test and unit
HEADERS += \
    ut_mcombobox.h

INCLUDEPATH += \
    ../../src/corelib/widgets \
    ../../src/views \
    ../../src/views/.moc

include(../common_bot.pri)
