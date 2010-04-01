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

#ifndef DUIREMOTEACTION_P_H
#define DUIREMOTEACTION_P_H

#include "duiaction_p.h"
#include <QString>

class DuiRemoteAction;
class QDBusInterface;

class DuiRemoteActionPrivate : public DuiActionPrivate
{
    Q_DECLARE_PUBLIC(DuiRemoteAction)

public:
    DuiRemoteActionPrivate();
    virtual ~DuiRemoteActionPrivate();

    //! The name of the D-Bus service to call
    QString serviceName;
    //! The path of the D-Bus object to call
    QString objectPath;
    //! The name of the D-Bus interface to call
    QString interface;
    //! The name of the D-Bus method to call
    QString methodName;
    //! The arguments of the D-Bus call
    QList<QVariant> arguments;
};

#endif
