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

#ifndef MAPPLETOBJECTMENUACTIONSELECTEDMESSAGE_H
#define MAPPLETOBJECTMENUACTIONSELECTEDMESSAGE_H

#include "mappletmessage.h"
#include <mnamespace.h>

/*!
 * Send Response to applet with index of action selected from context menu actions displayed in host process
 */
class M_EXTENSIONS_EXPORT MAppletObjectMenuActionSelectedMessage : public MAppletMessage
{

private:
    /// index of action selected.
    uint actionIndex;

public:
    /*!
     * Constructor.
     *
     * The index can be set as a parameter.
     * \param index the index of action selected.
     */
    MAppletObjectMenuActionSelectedMessage(uint index = 0);

    /*!
     * Destroys the MAppletObjectMenuActionSelectedMessage.
     */

    virtual ~MAppletObjectMenuActionSelectedMessage();
    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the index of action selected.
     * \return the selected index.
     */
    uint index() const;
};

#endif // MAPPLETOBJECTMENUACTIONSELECTEDMESSAGE_H
