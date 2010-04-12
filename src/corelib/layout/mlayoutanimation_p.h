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

#ifndef MLAYOUTANIMATION_P_H
#define MLAYOUTANIMATION_P_H

#include "mlayoutanimation.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QList>

/** Private animator class. */
class MLayoutAnimationPrivate
{
    Q_DECLARE_PUBLIC(MLayoutAnimation)

public:
    /** Constructor */
    MLayoutAnimationPrivate();
    /** Destructor */
    virtual ~MLayoutAnimationPrivate() {}

    MLayout *layout;

protected:
    // Shared d_ptr related code:
    MLayoutAnimation *q_ptr;
};

#endif // Header Guard
