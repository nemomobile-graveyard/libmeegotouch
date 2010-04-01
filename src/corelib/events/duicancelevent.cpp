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

#include "duicancelevent.h"

static int duiCancelEventNumber = -1;

// DuiCancelEventPrivate is currently emtpy - for future use (prevent ABI break)

DuiCancelEvent::DuiCancelEvent()
    : QGraphicsSceneEvent(DuiCancelEvent::eventNumber()),
      d_ptr(0)
{

}

//Static
QEvent::Type DuiCancelEvent::eventNumber()
{
    if (duiCancelEventNumber < 0)
        duiCancelEventNumber = QEvent::registerEventType();
    return (QEvent::Type)duiCancelEventNumber;
}
