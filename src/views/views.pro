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
    ../corelib/.gen \

include(views.pri)
include(widgets/widgets.pri)
include(style/style.pri)

LIBS += -lmeegotouchcore

LIBS += \
    -lmeegotouchcore \

SOURCES += mviewslibrary.cpp

include(../common_bot.pri)
