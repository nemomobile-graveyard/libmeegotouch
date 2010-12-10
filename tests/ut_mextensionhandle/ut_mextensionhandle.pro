include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style
QT += xml

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    CONFIG += link_pkgconfig
    PKGCONFIG += contextsubscriber-1.0
}

MODEL_HEADERS += \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h

STYLE_HEADERS += $$MSRCDIR/extensions/style/mextensionhandlestyle.h

SOURCES += ut_mextensionhandle.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle_p.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h \
    $$MSRCDIR/extensions/style/mextensionhandlestyle.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS

include(../common_bot.pri)
