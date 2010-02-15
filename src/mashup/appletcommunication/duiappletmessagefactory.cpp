/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <DuiDebug>
#include "duiappletmessagefactory.h"

#include "duiappletvisibilitymessage.h"
#include "duiappletorientationmessage.h"
#include "duiappletmousemessage.h"
#include "duiappletsetgeometrymessage.h"
#include "duiappletpixmaptakenintousemessage.h"
#include "duiappletalivemessagerequest.h"
#include "duiappletalivemessageresponse.h"
#include "duiappleticonchangedmessage.h"
#include "duiapplettitlechangedmessage.h"
#include "duiapplettextchangedmessage.h"
#include "duiappletupdategeometrymessage.h"
#include "duiappletpixmapmodifiedmessage.h"
#include "duiappletcancelmessage.h"
#include "duiappletobjectmenurequestmessage.h"
#include "duiappletobjectmenumessage.h"
#include "duiappletobjectmenuactionselectedmessage.h"

DuiAppletMessageFactory::DuiAppletMessageFactory()
{
}

DuiAppletMessage *DuiAppletMessageFactory::create(DuiAppletMessage::DuiAppletMessageType messageType)
{
    switch (messageType) {
    case DuiAppletMessage::VISIBILITY_MESSAGE:
        return new DuiAppletVisibilityMessage();

    case DuiAppletMessage::ORIENTATION_MESSAGE:
        return new DuiAppletOrientationMessage();

    case DuiAppletMessage::MOUSE_PRESS_MESSAGE:
        return new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_PRESS_MESSAGE);

    case DuiAppletMessage::MOUSE_RELEASE_MESSAGE:
        return new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_RELEASE_MESSAGE);

    case DuiAppletMessage::MOUSE_MOVE_MESSAGE:
        return new DuiAppletMouseMessage(DuiAppletMessage::MOUSE_MOVE_MESSAGE);

    case DuiAppletMessage::CANCEL_MESSAGE:
        return new DuiAppletCancelMessage();

    case DuiAppletMessage::SET_GEOMETRY_MESSAGE:
        return new DuiAppletSetGeometryMessage();

    case DuiAppletMessage::PIXMAP_TAKEN_INTO_USE_MESSAGE:
        return new DuiAppletPixmapTakenIntoUseMessage();

    case DuiAppletMessage::APPLET_ALIVE_MESSAGE_REQUEST:
        return new DuiAppletAliveMessageRequest();

    case DuiAppletMessage::APPLET_ALIVE_MESSAGE_RESPONSE:
        return new DuiAppletAliveMessageResponse();

    case DuiAppletMessage::APPLET_ICON_MESSAGE:
        return new DuiAppletIconChangedMessage();

    case DuiAppletMessage::APPLET_TITLE_MESSAGE:
        return new DuiAppletTitleChangedMessage();

    case DuiAppletMessage::APPLET_TEXT_MESSAGE:
        return new DuiAppletTextChangedMessage();

    case DuiAppletMessage::UPDATE_GEOMETRY_MESSAGE:
        return new DuiAppletUpdateGeometryMessage();

    case DuiAppletMessage::PIXMAP_MODIFIED_MESSAGE:
        return new DuiAppletPixmapModifiedMessage();

    case DuiAppletMessage::OBJECT_MENU_REQUEST_MESSAGE:
        return new DuiAppletObjectMenuRequestMessage();

    case DuiAppletMessage::OBJECT_MENU_MESSAGE:
        return new DuiAppletObjectMenuMessage();

    case DuiAppletMessage::OBJECT_MENU_ACTION_SELECTED_MESSAGE:
        return new DuiAppletObjectMenuActionSelectedMessage();

    default:
        duiWarning("DuiAppletMessageFactory") << __func__ << "type" << messageType << "not registered";
        return NULL;
    }
}
