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

#ifndef DUIAPPLETTEXTCHANGEDMESSAGE_H
#define DUIAPPLETTEXTCHANGEDMESSAGE_H

#include <QString>
#include "duiappletmessage.h"

/*!
 * Applet text changed message.
 */
class DUI_EXPORT DuiAppletTextChangedMessage : public DuiAppletMessage
{
private:
    QString _text;

public:
    /*!
     * Constructor.
     *
     */
    DuiAppletTextChangedMessage();

    /*!
     * Destructor.
     */
    virtual ~DuiAppletTextChangedMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    QString text() const;
    void setText(const QString &newText);
};

#endif // DUIAPPLETTEXTCHANGEDESSAGE_H
