include(../common_top.pri)

TARGET = meegotouchviews

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

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework meegotouchcore
    }
    win32:LIBS += -lmeegotouchcore0
}
else:LIBS += -lmeegotouchcore

SOURCES += mviewslibrary.cpp

include(../common_bot.pri)
