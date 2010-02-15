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

#ifndef DUIAPPLETICONCHANGEDMESSAGE_H
#define DUIAPPLETICONCHANGEDMESSAGE_H

#include <QString>
#include "duiappletmessage.h"

/*!
 * Applet icon changed message.
 */
class DUI_EXPORT DuiAppletIconChangedMessage : public DuiAppletMessage
{
private:
    QString _icon;

public:
    /*!
     * Constructor.
     *
     */
    DuiAppletIconChangedMessage();

    /*!
     * Destructor.
     */
    virtual ~DuiAppletIconChangedMessage();

    //! \reimp
    virtual void serialize(QDataStream &stream) const;
    //! \reimp_end

    //! \reimp
    virtual void unserialize(QDataStream &stream);
    //! \reimp_end

    QString icon() const;
    void setIcon(const QString &newIcon);
};

#endif // DUIAPPLETICONCHANGEDESSAGE_H
