include(../common_top.pri)

TARGET = duiviews

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

LIBS += -lduicore

LIBS += \
    -lduicore \

SOURCES += duiviewslibrary.cpp

include(../common_bot.pri)
