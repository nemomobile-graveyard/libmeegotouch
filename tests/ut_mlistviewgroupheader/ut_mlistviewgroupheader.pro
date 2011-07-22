include(../common_top.pri)

TARGET = ut_mlistviewgroupheader

INCLUDEPATH += ../mlistviewcommon \
        $$MSRCDIR/corelib/widgets \
        $$MSRCDIR/views/widgets \
        $$MSRCDIR/views \
        ../../src/corelib/.gen \
        ../../src/views/.moc \
        $$MSRCDIR/views/animations \

HEADERS += ut_mlistviewgroupheader.h \
        ../mlistviewcommon/myindexedmodel.h \
        $$MSRCDIR/views/widgets/mlistindexmodel.h \
        $$MSRCDIR/views/style/mlistindexstyle.h \
        $$MSRCDIR/views/style/mlistindextooltipstyle.h \
        $$MSRCDIR/views/style/mlistheaderstyle.h \
        $$MSRCDIR/views/style/mbasiclistitemdeletionanimationstyle.h \
        $$MSRCDIR/views/style/mbasiclistiteminsertionanimationstyle.h \

SOURCES += ut_mlistviewgroupheader.cpp  \
        ../mlistviewcommon/myindexedmodel.cpp \
        $$MSRCDIR/views/mlistview_p.cpp \
        ../../src/views/.moc/moc_mlistview_p.cpp \
        $$MSRCDIR/views/widgets/mlistindex.cpp \
        $$MSRCDIR/views/widgets/mlistindextooltip.cpp \
        ../../src/views/.gen/gen_mlistindexmodeldata.cpp \
        $$MSRCDIR/views/mlistindexview.cpp \
        ../../src/views/.gen/gen_mlistindexstyledata.cpp \
        ../../src/views/.gen/gen_mlistindextooltipstyledata.cpp \
        $$MSRCDIR/views/mapplicationpageview.cpp \
        $$MSRCDIR/views/widgets/mlistheader.cpp \
        ../../src/views/.moc/moc_mlistheader.cpp \
        ../../src/views/.gen/gen_mlistheaderstyledata.cpp \
        $$MSRCDIR/views/animations/mbasiclistitemdeletionanimation.cpp \
        ../../src/views/.gen/gen_mbasiclistitemdeletionanimationstyledata.cpp \
        $$MSRCDIR/views/animations/mbasiclistiteminsertionanimation.cpp \
        ../../src/views/.gen/gen_mbasiclistiteminsertionanimationstyledata.cpp \

include(../common_bot.pri)
