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

#ifndef MAPPLETOBJECTMENUMESSAGE_H
#define MAPPLETOBJECTMENUMESSAGE_H

#include "mappletmessage.h"
#include <mnamespace.h>

class QAction;

/*!
 * Object menu message which provides list of actions from applet.
 */
class M_EXTENSIONS_EXPORT MAppletObjectMenuMessage : public MAppletMessage
{
private:
    /// The list of action names.
    QList<QString> actionNamesList;

public:
    /*!
     * Constructor.
     *
     * List of QAction is given which are translated to list of QString and passed along.
     * \param actions List of actions on widget.
     */
    MAppletObjectMenuMessage(const QList<QAction *> actions = QList<QAction *>());

    /*!
     * Destructor.
     */
    virtual ~MAppletObjectMenuMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    //! Gives the list of names of the actions.
    QList<QString> actionList() const;
};

#endif // MAPPLETORIENTATIONMESSAGE_H
