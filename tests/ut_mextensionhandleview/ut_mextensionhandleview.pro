include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/widgets/core \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/style

MODEL_HEADERS += $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h

STYLE_HEADERS += $$MSRCDIR/extensions/style/mextensionhandlestyle.h

SOURCES += ut_mextensionhandleview.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionhandleview.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp \
    dummy.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mextensionhandleview.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandleview.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetview_p.h \
    $$MSRCDIR/extensions/style/mextensionhandlestyle.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS

include(../common_bot.pri)
