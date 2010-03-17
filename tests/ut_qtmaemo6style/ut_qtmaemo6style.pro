include(../common_top.pri)
TARGET = ut_qtmaemo6style
INCLUDEPATH += $$DUISRCDIR/widgets $$DUISRCDIR/style $$DUISRCDIR/../plainqt/style

LIBS += ../../plainqt/style/libduiqtstyleplugin.so

TEST_SOURCES = \
    $$DUISRCDIR/../plainqt/style/qtmaemo6style.cpp \

# unit test and unit
SOURCES += \
    ut_qtmaemo6style.cpp \

# base classes
SOURCES += \


# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_qtmaemo6style.h \

# base classes
HEADERS += \


# service classes
HEADERS += \


include(../common_bot.pri)
