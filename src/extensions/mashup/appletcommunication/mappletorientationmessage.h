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

#ifndef MAPPLETORIENTATIONMESSAGE_H
#define MAPPLETORIENTATIONMESSAGE_H

#include "mappletmessage.h"
#include <mnamespace.h>

/*!
 * Screen orientation change applet message.
 */
class M_EXTENSIONS_EXPORT MAppletOrientationMessage : public MAppletMessage
{
private:
    /// The new orientation.
    M::Orientation _orientation;

public:
    /*!
     * Constructor.
     *
     * The orientation can be set as a parameter. The default is \c M::Landscape.
     * \param orientation the orientation property of the message.
     */
    MAppletOrientationMessage(M::Orientation orientation = M::Landscape);

    /*!
     * Destructor.
     */
    virtual ~MAppletOrientationMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the orientation property of the message.
     * \return the orientation property.
     */
    M::Orientation orientation() const;

    /*!
     * Sets the orientation property of the message.
     * \param orientation the new orientation.
     */
    void setOrientation(M::Orientation orientation);
};

#endif // MAPPLETORIENTATIONMESSAGE_H
