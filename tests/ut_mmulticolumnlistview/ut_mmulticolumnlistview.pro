include(../common_top.pri)

TARGET = ut_mmulticolumnlistview

HEADERS += \
    ut_mmulticolumnlistview.h \
    myindexedmodel.h

SOURCES += \
    ut_mmulticolumnlistview.cpp \
    myindexedmodel.cpp \
    $$MSRCDIR/corelib/widgets/views/mlistview_p.cpp \
    $$MSRCDIR/corelib/.moc/moc_mlistview_p.cpp

include(../common_bot.pri)
