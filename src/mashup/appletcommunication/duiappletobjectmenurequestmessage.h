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

#ifndef DUIAPPLETOBJECTMENUREQUESTMESSAGE_H
#define DUIAPPLETOBJECTMENUREQUESTMESSAGE_H

#include "duiappletmessage.h"
#include <duinamespace.h>

/*!
 * Send Request to applet runner for a context menu.
 */
class DUI_EXPORT DuiAppletObjectMenuRequestMessage : public DuiAppletMessage
{
private:
    /// position where the event occured
    QPointF _pos ;

public:
    /*!
     * Constructor.
     *
     * The point of event is a parameter.
     * \param pos point which was received by the host process in context menu event.
     */
    DuiAppletObjectMenuRequestMessage(QPointF pos = QPointF(-1, -1));

    /*!
     * Destroys the DuiAppletObjectMenuRequestMessage
     */
    virtual ~DuiAppletObjectMenuRequestMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the position of the context menu event.
     * \return position of the event.
     */
    QPointF pos() const;
};

#endif // DUIAPPLETOBJECTMENUREQUESTMESSAGE_H
