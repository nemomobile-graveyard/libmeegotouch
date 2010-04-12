###############################################################################
# Feedback module
# This module contains all classes which deal with feedback to user input  
############################################################################### 
FEEDBACK_SRC_DIR=./feedback
INCLUDEPATH+= ./feedback

PUBLIC_HEADERS += \
    $$FEEDBACK_SRC_DIR/mfeedback.h \
    $$FEEDBACK_SRC_DIR/mfeedbackplayer.h \

PRIVATE_HEADERS += \
    $$FEEDBACK_SRC_DIR/mfeedbackplayer_p.h \
    $$FEEDBACK_SRC_DIR/mfeedback_p.h

SOURCES += $$FEEDBACK_SRC_DIR/mfeedback.cpp \
    $$FEEDBACK_SRC_DIR/mfeedback_p.cpp \
    $$FEEDBACK_SRC_DIR/mfeedbackplayer.cpp \
    $$FEEDBACK_SRC_DIR/mfeedbackplayer_p.cpp
