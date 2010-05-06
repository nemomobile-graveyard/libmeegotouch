/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#include <MDebug>
#include "mappletmessagefactory.h"

#include "mappletvisibilitymessage.h"
#include "mappletorientationmessage.h"
#include "mappletmousemessage.h"
#include "mappletsetgeometrymessage.h"
#include "mappletpixmaptakenintousemessage.h"
#include "mappletalivemessagerequest.h"
#include "mappletalivemessageresponse.h"
#include "mappleticonchangedmessage.h"
#include "mapplettitlechangedmessage.h"
#include "mapplettextchangedmessage.h"
#include "mappletupdategeometrymessage.h"
#include "mappletpixmapmodifiedmessage.h"
#include "mappletcancelmessage.h"
#include "mappletobjectmenurequestmessage.h"
#include "mappletobjectmenumessage.h"
#include "mappletobjectmenuactionselectedmessage.h"

MAppletMessageFactory::MAppletMessageFactory()
{
}

MAppletMessage *MAppletMessageFactory::create(MAppletMessage::MAppletMessageType messageType)
{
    switch (messageType) {
    case MAppletMessage::VisibilityMessage:
        return new MAppletVisibilityMessage();

    case MAppletMessage::OrientationMessage:
        return new MAppletOrientationMessage();

    case MAppletMessage::MousePressMessage:
        return new MAppletMouseMessage(MAppletMessage::MousePressMessage);

    case MAppletMessage::MouseReleaseMessage:
        return new MAppletMouseMessage(MAppletMessage::MouseReleaseMessage);

    case MAppletMessage::MouseMoveMessage:
        return new MAppletMouseMessage(MAppletMessage::MouseMoveMessage);

    case MAppletMessage::CancelMessage:
        return new MAppletCancelMessage();

    case MAppletMessage::SetGeometryMessage:
        return new MAppletSetGeometryMessage();

    case MAppletMessage::PixmapTakenIntoUseMessage:
        return new MAppletPixmapTakenIntoUseMessage();

    case MAppletMessage::AppletAliveMessageRequest:
        return new MAppletAliveMessageRequest();

    case MAppletMessage::AppletAliveMessageResponse:
        return new MAppletAliveMessageResponse();

    case MAppletMessage::AppletIconMessage:
        return new MAppletIconChangedMessage();

    case MAppletMessage::AppletTitleMessage:
        return new MAppletTitleChangedMessage();

    case MAppletMessage::AppletTextMessage:
        return new MAppletTextChangedMessage();

    case MAppletMessage::UpdateGeometryMessage:
        return new MAppletUpdateGeometryMessage();

    case MAppletMessage::PixmapModifiedMessage:
        return new MAppletPixmapModifiedMessage();

    case MAppletMessage::ObjectMenuRequestMessage:
        return new MAppletObjectMenuRequestMessage();

    case MAppletMessage::ObjectMenuMessage:
        return new MAppletObjectMenuMessage();

    case MAppletMessage::ObjectMenuActionSelectedMessage:
        return new MAppletObjectMenuActionSelectedMessage();

    default:
        mWarning("MAppletMessageFactory") << __func__ << "type" << messageType << "not registered";
        return NULL;
    }
}
