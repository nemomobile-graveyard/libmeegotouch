###############################################################################
# DuiFeedback module
# This module contains all classes which deal with feedback to user input  
############################################################################### 
FEEDBACK_SRC_DIR=./feedback
INCLUDEPATH+= ./feedback

PUBLIC_HEADERS += \
    $$FEEDBACK_SRC_DIR/duifeedback.h \
    $$FEEDBACK_SRC_DIR/duifeedbackplayer.h \

PRIVATE_HEADERS += \
    $$FEEDBACK_SRC_DIR/duifeedbackplayer_p.h \
    $$FEEDBACK_SRC_DIR/duifeedback_p.h

SOURCES += $$FEEDBACK_SRC_DIR/duifeedback.cpp \
    $$FEEDBACK_SRC_DIR/duifeedback_p.cpp \
    $$FEEDBACK_SRC_DIR/duifeedbackplayer.cpp \
    $$FEEDBACK_SRC_DIR/duifeedbackplayer_p.cpp
