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

#ifndef DUIAPPLETMESSAGEFACTORY_H_
#define DUIAPPLETMESSAGEFACTORY_H_

#include "duiexport.h"

#include <duiappletmessage.h>

/*!
 * Factory class for creating applet messages.
 *
 * The factory class offers a service to create applet messages of the correct type.
 * The applet message type is passed as a parameter to the \c create() function and
 * the function constructs and returns a correct message object. If an invalid message
 * type is requested, the factory returns \c NULL.
 *
 * \note The \c creator() function is \c static so you don't need an instance of the
 * factory. Actually you can't even create instances of it since the constructor is
 * hidden.
 */
class DUI_EXPORT DuiAppletMessageFactory
{
private:
    /// Hidden constructor to prevent instantiating.
    DuiAppletMessageFactory();

public:
    /*!
     * Creates an applet message object of the requested type.
     *
     * The function constructs and returns a correct message object. If an invalid message
     * type is requested, the factory returns \c NULL.
     *
     * \note The ownership of the returned object is transformed to the caller so the calling
     * code is responsible of deleting the object.
     * \param messageType the type of the requested message.
     * \return An applet message or \c NULL on invalid request.
     */
    static DuiAppletMessage *create(DuiAppletMessage::DuiAppletMessageType messageType);
};

#endif /* DUIAPPLETMESSAGEFACTORY_H_ */
