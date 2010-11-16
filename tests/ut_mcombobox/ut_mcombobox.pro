include(../common_top.pri)

TARGET = ut_mcombobox

# unit

# unit test and unit
SOURCES += \
    ut_mcombobox.cpp \
    $$MSRCDIR/views/mcomboboxview.cpp \
    $$MSRCDIR/views/widgets/mcomboboxbutton.cpp \

# unit test and unit
HEADERS += \
    ut_mcombobox.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/views \
    $$MSRCDIR/views/widgets \
    ../../src/views/.moc \

include(../common_bot.pri)
