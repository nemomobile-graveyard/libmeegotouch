include(../common_top.pri)
include(../common_mextensions.pri)

# unit test and unit classes
SOURCES += \
    ut_mappletmetadata.cpp \
    $$MSRCDIR/extensions/mashup/appletinterface/mappletmetadata.cpp

# unit test and unit classes
HEADERS += \
    ut_mappletmetadata.h \
    $$MSRCDIR/extensions/mashup/appletinterface/mappletmetadata.h

DEFINES += M_INSTALL_LIBS=\\\"\"$$M_INSTALL_LIBS\"\\\"

#DEFINES += APPLET_LIBS=\'$$quote(\"$$M_APPLET_DIR/lib/m/applets/\")\'

include(../common_bot.pri)
