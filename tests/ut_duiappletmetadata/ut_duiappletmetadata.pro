include(../common_top.pri)
include(../common_duiextensions.pri)

# unit test and unit classes
SOURCES += \
    ut_duiappletmetadata.cpp \
    $$DUISRCDIR/extensions/mashup/appletinterface/duiappletmetadata.cpp

# unit test and unit classes
HEADERS += \
    ut_duiappletmetadata.h \
    $$DUISRCDIR/extensions/mashup/appletinterface/duiappletmetadata.h

DEFINES += DUI_INSTALL_LIBS=\\\"\"$$DUI_INSTALL_LIBS\"\\\"

#DEFINES += APPLET_LIBS=\'$$quote(\"$$DUI_APPLET_DIR/lib/dui/applets/\")\'

include(../common_bot.pri)
