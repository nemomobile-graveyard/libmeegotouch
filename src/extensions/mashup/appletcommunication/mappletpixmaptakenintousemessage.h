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

#ifndef MAPPLETPIXMAPTAKENINTOUSEMESSAGE_H
#define MAPPLETPIXMAPTAKENINTOUSEMESSAGE_H

#include "mappletmessage.h"

/*!
 * An applet message for notifying that the pixmap has been taken into use.
 */
class M_EXTENSIONS_EXPORT MAppletPixmapTakenIntoUseMessage : public MAppletMessage
{
private:
    //! The X pixmap handle.
    Qt::HANDLE _handle;

public:
    /*!
     * Constructs a MAppletPixmapTakenIntoUseMessage.
     *
     * \param handle the pixmap handle
     */
    MAppletPixmapTakenIntoUseMessage(Qt::HANDLE handle = 0);

    /*!
     * Destroys the MAppletPixmapTakenIntoUseMessage.
     */
    virtual ~MAppletPixmapTakenIntoUseMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the X pixmap handle property of the message.
     * \return the X pixmap handle.
     */
    Qt::HANDLE handle() const;

    /*!
     * Sets the X pixmap handle property of the message.
     * \param newHandle the new X pixmap handle.
     */
    void setHandle(Qt::HANDLE newHandle);
};

#endif // MAPPLETPIXMAPTAKENINTOUSEMESSAGE_H
