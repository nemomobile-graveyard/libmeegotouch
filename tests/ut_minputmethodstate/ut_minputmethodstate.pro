include(../common_top.pri)
TARGET = ut_minputmethodstate

contains(DEFINES, HAVE_MALIIT) {
    CONFIG += link_pkgconfig
    PKGCONFIG += maliit-1.0
}

# unit test and unit classes
SOURCES += \
    ut_minputmethodstate.cpp \
    $$MSRCDIR/corelib/core/minputmethodstate.cpp

# unit test and unit classes
HEADERS += \
   ut_minputmethodstate.h \
   $$MSRCDIR/corelib/core/minputmethodstate.h \
   $$MSRCDIR/corelib/core/minputmethodstate_p.h

INCLUDEPATH += $$MSRCDIR/corelib/core

include(../common_bot.pri)
