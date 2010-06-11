include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/corelib/feedback

TARGET = ut_mfeedback

SOURCES += \
    ut_mfeedback.cpp \
    mfeedbackplayer_mock.cpp \
    mapplication_mock.cpp \
    $$MSRCDIR/corelib/feedback/mfeedback.cpp \
    $$MSRCDIR/corelib/feedback/mfeedback_p.cpp

HEADERS += \
    ut_mfeedback.h \
    mfeedbackplayer.h \
    mfeedbackplayer_mock.h \
    mapplication.h \
    mapplication_mock.h \
    $$MSRCDIR/corelib/feedback/mfeedback.h \
    $$MSRCDIR/corelib/feedback/mfeedback_p.h

include(../common_bot.pri)
