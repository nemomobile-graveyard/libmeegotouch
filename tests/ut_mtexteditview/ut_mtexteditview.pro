include(../common_top.pri)
TARGET = ut_mtexteditview

SOURCES += \
    ut_mtexteditview.cpp \
    $$MSRCDIR/views/mtexteditview.cpp

HEADERS += \
    ut_mtexteditview.h \
    mtextmagnifier.h \
    $$MSRCDIR/views/mtexteditview.h \
    $$MSRCDIR/views/mtexteditview_p.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets/ \

# Use stubbed mtextmagnifier.h in mtexteditview.cpp.
QMAKE_CXXFLAGS += -include mtextmagnifier.h

include(../common_bot.pri)
