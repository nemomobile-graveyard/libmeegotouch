include(../mashup.pri)

MASHUP_APPLETCOMMUNICATION_SRC_DIR = $$MASHUP_SRC_DIR/appletcommunication
INCLUDEPATH += $$MASHUP_SRC_DIR/appletcommunication

PUBLIC_HEADERS += $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletalivemessagerequest.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletalivemessageresponse.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletcancelmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletclient.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletcommunicator.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappleticonchangedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletmessagefactory.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletmousemessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletobjectmenuactionselectedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletobjectmenumessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletobjectmenurequestmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletorientationmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletpixmapmodifiedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletpixmaptakenintousemessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletserver.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletsetgeometrymessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mapplettextchangedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mapplettitlechangedmessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletupdategeometrymessage.h \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletvisibilitymessage.h

SOURCES += $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletalivemessagerequest.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletalivemessageresponse.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletcommunicator.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletserver.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletclient.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletmessagefactory.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletmousemessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletorientationmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletobjectmenurequestmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletobjectmenumessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletobjectmenuactionselectedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletsetgeometrymessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletpixmaptakenintousemessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletvisibilitymessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappleticonchangedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mapplettitlechangedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mapplettextchangedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletupdategeometrymessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletpixmapmodifiedmessage.cpp \
    $$MASHUP_APPLETCOMMUNICATION_SRC_DIR/mappletcancelmessage.cpp \

