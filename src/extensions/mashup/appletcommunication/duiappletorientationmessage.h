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

#ifndef DUIAPPLETORIENTATIONMESSAGE_H
#define DUIAPPLETORIENTATIONMESSAGE_H

#include "duiappletmessage.h"
#include <duinamespace.h>

/*!
 * Screen orientation change applet message.
 */
class DUI_EXPORT DuiAppletOrientationMessage : public DuiAppletMessage
{
private:
    /// The new orientation.
    Dui::Orientation _orientation;

public:
    /*!
     * Constructor.
     *
     * The orientation can be set as a parameter. The default is \c Dui::Landscape.
     * \param orientation the orientation property of the message.
     */
    DuiAppletOrientationMessage(Dui::Orientation orientation = Dui::Landscape);

    /*!
     * Destructor.
     */
    virtual ~DuiAppletOrientationMessage();

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
    Dui::Orientation orientation() const;

    /*!
     * Sets the orientation property of the message.
     * \param orientation the new orientation.
     */
    void setOrientation(Dui::Orientation orientation);
};

#endif // DUIAPPLETORIENTATIONMESSAGE_H
