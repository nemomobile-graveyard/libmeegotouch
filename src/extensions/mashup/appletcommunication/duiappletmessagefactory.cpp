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
    case DuiAppletMessage::VisibilityMessage:
        return new DuiAppletVisibilityMessage();

    case DuiAppletMessage::OrientationMessage:
        return new DuiAppletOrientationMessage();

    case DuiAppletMessage::MousePressMessage:
        return new DuiAppletMouseMessage(DuiAppletMessage::MousePressMessage);

    case DuiAppletMessage::MouseReleaseMessage:
        return new DuiAppletMouseMessage(DuiAppletMessage::MouseReleaseMessage);

    case DuiAppletMessage::MouseMoveMessage:
        return new DuiAppletMouseMessage(DuiAppletMessage::MouseMoveMessage);

    case DuiAppletMessage::CancelMessage:
        return new DuiAppletCancelMessage();

    case DuiAppletMessage::SetGeometryMessage:
        return new DuiAppletSetGeometryMessage();

    case DuiAppletMessage::PixmapTakenIntoUseMessage:
        return new DuiAppletPixmapTakenIntoUseMessage();

    case DuiAppletMessage::AppletAliveMessageRequest:
        return new DuiAppletAliveMessageRequest();

    case DuiAppletMessage::AppletAliveMessageResponse:
        return new DuiAppletAliveMessageResponse();

    case DuiAppletMessage::AppletIconMessage:
        return new DuiAppletIconChangedMessage();

    case DuiAppletMessage::AppletTitleMessage:
        return new DuiAppletTitleChangedMessage();

    case DuiAppletMessage::AppletTextMessage:
        return new DuiAppletTextChangedMessage();

    case DuiAppletMessage::UpdateGeometryMessage:
        return new DuiAppletUpdateGeometryMessage();

    case DuiAppletMessage::PixmapModifiedMessage:
        return new DuiAppletPixmapModifiedMessage();

    case DuiAppletMessage::ObjectMenuRequestMessage:
        return new DuiAppletObjectMenuRequestMessage();

    case DuiAppletMessage::ObjectMenuMessage:
        return new DuiAppletObjectMenuMessage();

    case DuiAppletMessage::ObjectMenuActionSelectedMessage:
        return new DuiAppletObjectMenuActionSelectedMessage();

    default:
        duiWarning("DuiAppletMessageFactory") << __func__ << "type" << messageType << "not registered";
        return NULL;
    }
}
