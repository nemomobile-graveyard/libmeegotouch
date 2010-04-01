include(../common_top.pri)

TARGET = ut_duilistviewplain

INCLUDEPATH += ../ut_duilistviewcommon
HEADERS += ut_duilistviewplain.h

SOURCES += ut_duilistviewplain.cpp  \
        ../../src/corelib/widgets/views/duilistview_p.cpp \
        ../../src/corelib/widgets/duicell.cpp \
        ../../src/corelib/.moc/moc_duilistview_p.cpp

include(../common_bot.pri)
