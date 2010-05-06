include(../common_top.pri)

#FIXME holger check

INCLUDEPATH += $$MSRCDIR/corelib/feedback

TARGET = ut_mfeedbackplayer

TESTED_SOURCES = $$MSRCDIR/corelib/feedback/mfeedbackplayer.cpp \
                 $$MSRCDIR/corelib/feedback/mfeedbackplayer_p.cpp

TESTED_HEADERS = $$MSRCDIR/corelib/feedback/mfeedbackplayer.h \
                 $$MSRCDIR/corelib/feedback/mfeedbackplayer_p.h

copysourcefiles.input = TESTED_SOURCES
copysourcefiles.output = ${QMAKE_FILE_BASE}.cpp
copysourcefiles.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_BASE}.cpp
copysourcefiles.variable_out = SOURCES
copysourcefiles.CONFIG = target_predeps no_link
QMAKE_EXTRA_COMPILERS += copysourcefiles

copyheaderfiles.input = TESTED_HEADERS
copyheaderfiles.output = ${QMAKE_FILE_BASE}.h
copyheaderfiles.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_BASE}.h
copyheaderfiles.variable_out = HEADERS
copyheaderfiles.CONFIG = target_predeps no_link
QMAKE_EXTRA_COMPILERS += copyheaderfiles

mocheaderfiles.input = TESTED_HEADERS
mocheaderfiles.output = moc_${QMAKE_FILE_BASE}.cpp
mocheaderfiles.commands = $$QMAKE_MOC $(DEFINES) $(INCPATH) ${QMAKE_FILE_BASE}.h -o moc_${QMAKE_FILE_BASE}.cpp
mocheaderfiles.variable_out = SOURCES
mocheaderfiles.CONFIG = target_predeps no_link
QMAKE_EXTRA_COMPILERS += mocheaderfiles

SOURCES += \
    ut_mfeedbackplayer.cpp \
    qlocalsocket_mock.cpp \
    mapplicationprivate_mock.cpp

HEADERS += \
    ut_mfeedbackplayer.h \
    QLocalSocket \
    qlocalsocket.h \
    qlocalsocket_mock.h \
    mapplicationprivate_mock.h \
    mexport.h \
    mexport_mock.h

include(../common_bot.pri)
