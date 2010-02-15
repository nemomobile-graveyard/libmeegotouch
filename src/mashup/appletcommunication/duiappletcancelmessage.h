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

#ifndef DUIAPPLETCANCELMESSAGE_H
#define DUIAPPLETCANCELMESSAGE_H

#include "duiappletmessage.h"

/*!
 * Cancel message sent to the applet when the previously
 * sent mouse press message needs to be cancelled.
 */
class DUI_EXPORT DuiAppletCancelMessage : public DuiAppletMessage
{

public:
    /*!
     * Cancel message constructor. The message does not contain
     * any attributes, so no parameter is required.
     */
    explicit DuiAppletCancelMessage();

    /*!
     * Destructor.
     */
    virtual ~DuiAppletCancelMessage();
};

#endif // DUIAPPLETCANCELMESSAGE_H
