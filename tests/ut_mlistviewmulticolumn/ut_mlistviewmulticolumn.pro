include(../common_top.pri)

TARGET = ut_mlistviewmulticolumn

INCLUDEPATH += ../mlistviewcommon \
        $$MSRCDIR/corelib/widgets \
        $$MSRCDIR/views/widgets \
        $$MSRCDIR/views \
        $$MSRCDIR/corelib/.gen

HEADERS += ut_mlistviewmulticolumn.h \
        ../mlistviewcommon/myindexedmodel.h \
        $$MSRCDIR/views/widgets/mlistindexmodel.h \
        $$MSRCDIR/views/style/mlistindexstyle.h \

SOURCES += ut_mlistviewmulticolumn.cpp  \
        ../mlistviewcommon/myindexedmodel.cpp \
        $$MSRCDIR/views/mlistview_p.cpp \
        $$MSRCDIR/views/.moc/moc_mlistview_p.cpp \
        $$MSRCDIR/views/widgets/mlistindex.cpp \
        $$MSRCDIR/views/.moc/moc_mlistindex.cpp \
        $$MSRCDIR/views/.gen/gen_mlistindexmodeldata.cpp \
        $$MSRCDIR/views/mlistindexview.cpp \
        $$MSRCDIR/views/.moc/moc_mlistindexview.cpp \
        $$MSRCDIR/views/.moc/moc_mlistindexview_p.cpp \
        $$MSRCDIR/views/.gen/gen_mlistindexstyledata.cpp \

include(../common_bot.pri)
