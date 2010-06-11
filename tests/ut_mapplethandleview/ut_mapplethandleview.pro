include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/widgets/core \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/style

MODEL_HEADERS += \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandlemodel.h

STYLE_HEADERS += \
    $$MSRCDIR/extensions/style/mextensionhandlestyle.h \
    $$MSRCDIR/extensions/style/mapplethandlestyle.h

SOURCES += ut_mapplethandleview.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionhandleview.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandleview.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp \
    dummy.cpp \
    ../stubs/stubbase.cpp

HEADERS += ut_mapplethandleview.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandleview.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandleview.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandle.h \
    $$MSRCDIR/extensions/mashup/mashup/mapplethandlemodel.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetview_p.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS

include(../common_bot.pri)
