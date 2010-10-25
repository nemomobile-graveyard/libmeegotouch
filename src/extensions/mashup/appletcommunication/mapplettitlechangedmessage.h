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

#ifndef MAPPLETTITLECHANGEDMESSAGE_H
#define MAPPLETTITLECHANGEDMESSAGE_H

#include <QString>
#include "mappletmessage.h"

/*!
 * Applet title changed message.
 */
class M_EXTENSIONS_EXPORT MAppletTitleChangedMessage : public MAppletMessage
{
private:
    QString _title;

public:
    /*!
     * Constructor.
     *
     */
    MAppletTitleChangedMessage();

    /*!
     * Destructor.
     */
    virtual ~MAppletTitleChangedMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    QString title() const;
    void setTitle(const QString &newTitle);
};

#endif // MAPPLETTITLECHANGEDESSAGE_H
