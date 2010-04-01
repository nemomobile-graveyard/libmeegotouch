include(../common_top.pri)

# FIXME holger check

INCLUDEPATH += $$DUISRCDIR/corelib/feedback

TARGET = ut_duifeedback

TESTED_SOURCES = $$DUISRCDIR/corelib/feedback/duifeedback.cpp \
                 $$DUISRCDIR/corelib/feedback/duifeedback_p.cpp

TESTED_HEADERS = $$DUISRCDIR/corelib/feedback/duifeedback.h \
                 $$DUISRCDIR/corelib/feedback/duifeedback_p.h

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
    ut_duifeedback.cpp \
    duifeedbackplayer_mock.cpp \
    duiapplication_mock.cpp

HEADERS += \
    ut_duifeedback.h \
    duifeedbackplayer.h \
    duifeedbackplayer_mock.h \
    duiapplication.h \
    duiapplication_mock.h

include(../common_bot.pri)
