include(../common_top.pri)

TARGET = meegotouchviews

DEFINES += M_BUILD_VIEWS

INCLUDEPATH += \
    .moc \
    ../include \
    ../corelib/animation/widget/core \
    ../corelib/style \
    ../corelib/widgets/views \
    ../corelib/widgets \
    ../corelib/core \
    ../corelib/.gen \

include(effects/effects.pri)
include(views.pri)
include(widgets/widgets.pri)
include(animations/animations.pri)
include(style/style.pri)

LIBS += $$mAddLibrary(meegotouchcore)
SOURCES += mviewslibrary.cpp

include(../common_bot.pri)
