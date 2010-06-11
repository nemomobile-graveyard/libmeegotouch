include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/feedback

TARGET = ut_mfeedbackplayer

SOURCES += \
    ut_mfeedbackplayer.cpp \
    qlocalsocket_mock.cpp \
    mapplicationprivate_mock.cpp \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer.cpp \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer_p.cpp

HEADERS += \
    ut_mfeedbackplayer.h \
    QLocalSocket \
    qlocalsocket.h \
    qlocalsocket_mock.h \
    mapplicationprivate_mock.h \
    mexport.h \
    mexport_mock.h \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer.h \
    $$MSRCDIR/corelib/feedback/mfeedbackplayer_p.h

include(../common_bot.pri)
