include(../mashup.pri)

MASHUP_APPLETCOMMUNICATION_SRC_DIR = $$MASHUP_SRC_DIR/appletcommunication
INCLUDEPATH += $$MASHUP_SRC_DIR/appletcommunication

HEADERS += $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletalivemessagerequest.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletalivemessageresponse.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletcommunicator.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletserver.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletclient.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletmessagefactory.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletmousemessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletorientationmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletobjectmenurequestmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletobjectmenumessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletobjectmenuactionselectedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletsetgeometrymessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletpixmaptakenintousemessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletvisibilitymessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappleticonchangedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiapplettitlechangedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiapplettextchangedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletupdategeometrymessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletpixmapmodifiedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletcancelmessage.h \

SOURCES += $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletalivemessagerequest.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletalivemessageresponse.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletcommunicator.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletserver.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletclient.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletmessagefactory.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletmousemessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletorientationmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletobjectmenurequestmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletobjectmenumessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletobjectmenuactionselectedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletsetgeometrymessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletpixmaptakenintousemessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletvisibilitymessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappleticonchangedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiapplettitlechangedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiapplettextchangedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletupdategeometrymessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletpixmapmodifiedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/duiappletcancelmessage.cpp \

