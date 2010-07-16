include(../common_top.pri)

TARGET = ut_mlistviewgroupheader

INCLUDEPATH += ../mlistviewcommon \
        ../../src/corelib/widgets \
        ../../src/views/widgets \
        ../../src/views \
        ../../src/corelib/.gen

HEADERS += ut_mlistviewgroupheader.h \
        ../mlistviewcommon/myindexedmodel.h \
        ../../src/views/widgets/mlistindexmodel.h \
        ../../src/views/style/mlistindexstyle.h \

SOURCES += ut_mlistviewgroupheader.cpp  \
        ../mlistviewcommon/myindexedmodel.cpp \
        ../../src/views/mlistview_p.cpp \
        ../../src/views/.moc/moc_mlistview_p.cpp \
        ../../src/views/widgets/mlistindex.cpp \
        ../../src/views/.moc/moc_mlistindex.cpp \
        ../../src/views/.gen/gen_mlistindexmodeldata.cpp \
        ../../src/views/mlistindexview.cpp \
        ../../src/views/.moc/moc_mlistindexview.cpp \
        ../../src/views/.moc/moc_mlistindexview_p.cpp \
        ../../src/views/.gen/gen_mlistindexstyledata.cpp \

include(../common_bot.pri)
