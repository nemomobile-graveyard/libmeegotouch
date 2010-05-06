include(../common_top.pri)

TARGET = ut_mlistviewseparators

INCLUDEPATH += ../ut_mlistviewcommon \
    ../mlistviewcommon
HEADERS += ut_mlistviewseparators.h \
        ../mlistviewcommon/myindexedmodel.h

SOURCES += ut_mlistviewseparators.cpp  \
        ../mlistviewcommon/myindexedmodel.cpp \
        ../../src/views/mlistview_p.cpp \
        ../../src/views/.moc/moc_mlistview_p.cpp

include(../common_bot.pri)
