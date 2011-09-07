include(../common_top.pri)
TARGET = ut_mtexteditview

SOURCES += \
    ut_mtexteditview.cpp \
    $$MSRCDIR/views/mtexteditview.cpp \
    $$MSRCDIR/views/meditortoolbar.cpp \
    $$MSRCDIR/views/meditortoolbararrow.cpp \
    $$MSRCDIR/views/mtopleveloverlay.cpp

HEADERS += \
    ut_mtexteditview.h \
    mtextmagnifier.h \
    $$MSRCDIR/views/mtexteditview.h \
    $$MSRCDIR/views/mtexteditview_p.h \
    $$MSRCDIR/views/meditortoolbar.h \
    $$MSRCDIR/views/meditortoolbararrow.h \
    $$MSRCDIR/views/mtopleveloverlay.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets/ \

# Use stubbed versions of mtextmagnifier.h
# when compiling mtexteditview.cpp.
QMAKE_CXXFLAGS += -include mtextmagnifier.h

support_files.files += \
    ut_mtexteditview.css \

include(../common_bot.pri)
