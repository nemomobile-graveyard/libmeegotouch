include(../common_top.pri)
TARGET = ut_mtexteditview

SOURCES += \
    ut_mtexteditview.cpp \
    $$MSRCDIR/views/mtexteditview.cpp \
    $$MSRCDIR/views/melidedtext.cpp \

HEADERS += \
    ut_mtexteditview.h \
    mtextmagnifier.h \
    meditortoolbar.h \
    $$MSRCDIR/views/mtexteditview.h \
    $$MSRCDIR/views/mtexteditview_p.h \
    $$MSRCDIR/views/melidedtext.h \
    mtextselectionoverlay.h

OTHER_FILES += ut_mtexteditview.css

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets/ \
    $$MSRCDIR/../tests/ \

# Use stubbed versions of mtextmagnifier.h and meditortoolbar.h
# when compiling mtexteditview.cpp.
QMAKE_CXXFLAGS += -include mtextmagnifier.h \
                  -include meditortoolbar.h \
                  -include mtextselectionoverlay.h

support_files.files += \
    ut_mtexteditview.css \

include(../common_bot.pri)
