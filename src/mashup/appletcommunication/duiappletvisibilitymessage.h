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

#ifndef DUIAPPLETVISIBILITYMESSAGE_H
#define DUIAPPLETVISIBILITYMESSAGE_H

#include "duiappletmessage.h"

/*!
 * An applet message for telling an applet when it gets visible/invisible.
 */
class DUI_EXPORT DuiAppletVisibilityMessage : public DuiAppletMessage
{
private:
    //! Visible or invisible
    bool _visible;

public:
    /*!
     * Constructor.
     *
     * The visibility property can be set as a parameter.
     * The default is \c true (visible).
     * \param visible the visibility.
     */
    DuiAppletVisibilityMessage(bool visible = true);

    /*!
     * Destructor.
     */
    virtual ~DuiAppletVisibilityMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    /*!
     * Returns the visibility value of the message. Visible is \c true, invisible
     * is \c false.
     * \return the visibility.
     */
    bool visible() const;

    /*!
     * Sets the visibility value of the message. Visible is \c true, invisible
     * is \c false.
     * \param visible the visibility.
     */
    void setVisible(bool visible = true);
};

#endif // DUIAPPLETVISIBILITYMESSAGE_H
