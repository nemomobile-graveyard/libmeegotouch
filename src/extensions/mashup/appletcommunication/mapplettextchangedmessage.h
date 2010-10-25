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

#ifndef MAPPLETTEXTCHANGEDMESSAGE_H
#define MAPPLETTEXTCHANGEDMESSAGE_H

#include <QString>
#include "mappletmessage.h"

/*!
 * Applet text changed message.
 */
class M_EXTENSIONS_EXPORT MAppletTextChangedMessage : public MAppletMessage
{
private:
    QString _text;

public:
    /*!
     * Constructor.
     *
     */
    MAppletTextChangedMessage();

    /*!
     * Destructor.
     */
    virtual ~MAppletTextChangedMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    QString text() const;
    void setText(const QString &newText);
};

#endif // MAPPLETTEXTCHANGEDESSAGE_H
