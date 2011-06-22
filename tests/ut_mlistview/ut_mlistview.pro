include(../common_top.pri)

INCLUDEPATH += \
        $${M_SOURCE_TREE}/src/views \
        $${M_SOURCE_TREE}/src/views/widgets \
        $${M_SOURCE_TREE}/src/views/animations \
        $${M_SOURCE_TREE}/src/corelib/widgets \
        $${M_BUILD_TREE}/src/corelib/.gen \
        $${M_BUILD_TREE}/src/views/.moc \

SOURCES += \
        ut_mlistview.cpp  \
        $${M_SOURCE_TREE}/src/views/mlistview_p.cpp \
        $${M_SOURCE_TREE}/src/views/widgets/mlistindex.cpp \
        $${M_SOURCE_TREE}/src/views/widgets/mlistindextooltip.cpp \
        $${M_SOURCE_TREE}/src/views/mlistindexview.cpp \
        $${M_SOURCE_TREE}/src/views/mapplicationpageview.cpp \
        $${M_SOURCE_TREE}/src/views/widgets/mlistheader.cpp \
        $${M_SOURCE_TREE}/src/views/animations/mbasiclistitemdeletionanimation.cpp \
        $${M_BUILD_TREE}/src/views/.gen/gen_mlistindexmodeldata.cpp \
        $${M_BUILD_TREE}/src/views/.gen/gen_mlistindexstyledata.cpp \
        $${M_BUILD_TREE}/src/views/.gen/gen_mlistindextooltipstyledata.cpp \
        $${M_BUILD_TREE}/src/views/.gen/gen_mlistheaderstyledata.cpp \
        $${M_BUILD_TREE}/src/views/.gen/gen_mbasiclistitemdeletionanimationstyledata.cpp \
        $${M_BUILD_TREE}/src/views/.moc/moc_mlistheader.cpp \
        $${M_BUILD_TREE}/src/views/.moc/moc_mlistview_p.cpp \

HEADERS += \
        ut_mlistview.h \
        $${M_SOURCE_TREE}/src/views/mlistview.cpp \
        $${M_SOURCE_TREE}/src/views/widgets/mlistindexmodel.h \
        $${M_SOURCE_TREE}/src/views/style/mlistindexstyle.h \
        $${M_SOURCE_TREE}/src/views/style/mlistindextooltipstyle.h \
        $${M_SOURCE_TREE}/src/views/style/mlistheaderstyle.h \
        $${M_SOURCE_TREE}/src/views/style/mbasiclistitemdeletionanimationstyle.h \

include(../common_bot.pri)
