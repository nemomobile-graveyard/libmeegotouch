/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MAPPLETICONCHANGEDMESSAGE_H
#define MAPPLETICONCHANGEDMESSAGE_H

#include <QString>
#include "mappletmessage.h"

/*!
 * Applet icon changed message.
 */
class M_EXTENSIONS_EXPORT MAppletIconChangedMessage : public MAppletMessage
{
private:
    QString _icon;

public:
    /*!
     * Constructor.
     *
     */
    MAppletIconChangedMessage();

    /*!
     * Destructor.
     */
    virtual ~MAppletIconChangedMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    QString icon() const;
    void setIcon(const QString &newIcon);
};

#endif // MAPPLETICONCHANGEDESSAGE_H
