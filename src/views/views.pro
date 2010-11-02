include(../common_top.pri)

TARGET = $$qtLibraryTarget(meegotouchviews)
DEFINES += M_BUILD_VIEWS

INCLUDEPATH += \
    .moc \
    ../include \
    ../corelib/animation/widget/core \
    ../corelib/style \
    ../corelib/widgets/views \
    ../corelib/widgets \
    ../corelib/core \
    ../corelib/video \
    ../corelib/.gen \

include(effects/effects.pri)
include(views.pri)
include(widgets/widgets.pri)
include(animations/animations.pri)
include(style/style.pri)
include(video/video.pri)

LIBS += $$mAddLibrary(meegotouchcore)
SOURCES += mviewslibrary.cpp

include(../common_bot.pri)
