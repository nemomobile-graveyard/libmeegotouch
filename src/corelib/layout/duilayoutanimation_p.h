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

#ifndef DUILAYOUTANIMATION_P_H
#define DUILAYOUTANIMATION_P_H

#include "duilayoutanimation.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QList>

/** Private animator class. */
class DuiLayoutAnimationPrivate
{
    Q_DECLARE_PUBLIC(DuiLayoutAnimation)

public:
    /** Constructor */
    DuiLayoutAnimationPrivate();
    /** Destructor */
    virtual ~DuiLayoutAnimationPrivate() {}

    DuiLayout *layout;

protected:
    // Shared d_ptr related code:
    DuiLayoutAnimation *q_ptr;
};

#endif // Header Guard
