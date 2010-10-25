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

#ifndef MAPPLETVISIBILITYMESSAGE_H
#define MAPPLETVISIBILITYMESSAGE_H

#include "mappletmessage.h"

/*!
 * An applet message for telling an applet when it gets visible/invisible.
 */
class M_EXTENSIONS_EXPORT MAppletVisibilityMessage : public MAppletMessage
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
    MAppletVisibilityMessage(bool visible = true);

    /*!
     * Destructor.
     */
    virtual ~MAppletVisibilityMessage();

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
    bool isVisible() const;

    /*!
     * Sets the visibility value of the message. Visible is \c true, invisible
     * is \c false.
     * \param visible the visibility.
     */
    void setVisible(bool visible = true);
};

#endif // MAPPLETVISIBILITYMESSAGE_H
