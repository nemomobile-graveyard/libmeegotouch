include(../common_top.pri)
TARGET = ut_mlistindexfloatingview

INCLUDEPATH += $$MSRCDIR/corelib/widgets \
               $$MSRCDIR/corelib/style \
               $$MSRCDIR/views/widgets \
               $$MSRCDIR/views \
               ../../src/views/.gen \
               ../../src/corelib/.gen \
               ../../src/views/.moc \

# unit test and unit
SOURCES += \
    ut_mlistindexfloatingview.cpp \
    $$MSRCDIR/views/mlistindexview.cpp \
    $$MSRCDIR/views/mlistindexfloatingview.cpp \
    $$MSRCDIR/views/widgets/mlistindex.cpp \
    ../../src/views/.gen/gen_mlistindexmodeldata.cpp \
    $$MSRCDIR/views/widgets/mlistindextooltip.cpp \
    ../../src/views/.gen/gen_mlistindexstyledata.cpp \
    ../../src/views/.gen/gen_mlistindextooltipstyledata.cpp \

# unit test and unit
HEADERS += \
    ut_mlistindexfloatingview.h \
    $$MSRCDIR/views/mlistindexview.h \
    $$MSRCDIR/views/mlistindexfloatingview.h \
    $$MSRCDIR/views/mlistindexfloatingview_p.h \
    $$MSRCDIR/views/widgets/mlistindex.h \
    $$MSRCDIR/views/widgets/mlistindexmodel.h \
    $$MSRCDIR/views/widgets/mlistindextooltip.h \
    $$MSRCDIR/views/style/mlistindexstyle.h \
    $$MSRCDIR/views/style/mlistindextooltipstyle.h \

support_files.files += ut_mlistindexfloatingview.css

include(../common_bot.pri)
