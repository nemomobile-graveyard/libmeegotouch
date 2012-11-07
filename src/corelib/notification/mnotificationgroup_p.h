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

#ifndef M_NOTIFICATION_GROUP_P_H
#define M_NOTIFICATION_GROUP_P_H

#include "mnotification_p.h"

/*!
 * A private class for MNotificationGroup
 */
class MNotificationGroupPrivate : public MNotificationPrivate
{
public:
    //! Construct the private class for MNotificationGroup
    MNotificationGroupPrivate();

    //! Returns hints for the notification group
    virtual QVariantHash hints() const;
};

#endif // M_NOTIFICATION_GROUP_P_H
