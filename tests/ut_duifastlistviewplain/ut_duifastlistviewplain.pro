include(../common_top.pri)

TARGET = ut_duifastlistviewplain

INCLUDEPATH += ../ut_duifastlistviewcommon
HEADERS += ut_duifastlistviewplain.h

SOURCES += ut_duifastlistviewplain.cpp  \
        ../../src/widgets/views/duifastlistview_p.cpp \
        ../../src/widgets/duicell.cpp \
        ../../src/.moc/moc_duifastlistview_p.cpp

include(../common_bot.pri)
