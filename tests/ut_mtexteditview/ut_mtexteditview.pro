include(../common_top.pri)
TARGET = ut_mtexteditview

SOURCES += \
    ut_mtexteditview.cpp \
    $$MSRCDIR/views/mtexteditview.cpp

HEADERS += \
    ut_mtexteditview.h \
    mtextmagnifier.h \
    meditortoolbar.h \
    $$MSRCDIR/views/mtexteditview.h \
    $$MSRCDIR/views/mtexteditview_p.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets/ \

# Use stubbed versions of mtextmagnifier.h and meditortoolbar.h
# when compiling mtexteditview.cpp.
QMAKE_CXXFLAGS += -include mtextmagnifier.h \
                  -include meditortoolbar.h

include(../common_bot.pri)
