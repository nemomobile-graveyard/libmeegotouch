include(../common_top.pri)

TARGET = ut_mtextedit

contains(DEFINES, HAVE_MALIIT) {
    CONFIG += link_pkgconfig
    PKGCONFIG += maliit-1.0
}

SOURCES += \
    ut_mtextedit.cpp \
    $$MSRCDIR/corelib/widgets/mtextedit.cpp \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.cpp \

HEADERS += \
    $$STUBSDIR/mpannableviewport_stub.h \
    $$STUBSDIR/mpannablewidget_stub.h \
    $$STUBSDIR/mtheme_stub.h \
    ut_mtextedit.h \
    $$MSRCDIR/corelib/core/minputmethodstate.h \
    $$MSRCDIR/corelib/widgets/mtextedit.h \
    $$MSRCDIR/corelib/widgets/mtextedit_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets/ \
    $$MSRCDIR/../tests/ \

include(../common_bot.pri)
