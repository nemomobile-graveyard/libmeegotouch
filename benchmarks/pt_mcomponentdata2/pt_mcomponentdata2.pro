include(../common_top.pri)
include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)

DEFINES+=M_VERSION

QT += network
contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

INCLUDEPATH += $$MSRCDIR/include \
    $$MSRCDIR/corelib/core \
    $$MSRCDIR/corelib/feedback \
    $$MSRCDIR/corelib/events \
    $$MSRCDIR/corelib/theme \

DEPENDPATH += $$INCLUDEPATH
TARGET = pt_mcomponentdata2

SOURCES += \
    pt_mcomponentdata2.cpp \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer.cpp \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer_p.cpp \


HEADERS += pt_mcomponentdata2.h \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer_p.h \
