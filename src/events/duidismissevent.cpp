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

#include "duidismissevent.h"

static int _duiDismissEventTypeValue = -1;

// DuiDismissEventPrivate is currently emtpy - for future use (prevent ABI break)

DuiDismissEvent::DuiDismissEvent()
    : QEvent(eventType()),
      d_ptr(0)
{

}

QEvent::Type DuiDismissEvent::eventType()
{
    if (_duiDismissEventTypeValue < 0) {
        _duiDismissEventTypeValue = QEvent::registerEventType();
    }
    return (QEvent::Type) _duiDismissEventTypeValue;
}

