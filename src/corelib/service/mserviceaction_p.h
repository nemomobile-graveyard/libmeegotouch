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

#ifndef MSERVICEACTION_P_H
#define MSERVICEACTION_P_H

#include "maction_p.h"
#include "mserviceaction.h"

class MServiceActionPrivate : public MActionPrivate
{
    Q_DECLARE_PUBLIC(MServiceAction)

public:
    MServiceActionPrivate();
    virtual ~MServiceActionPrivate();
};

#endif  //MSERVICEACTION_P_H
