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

#ifndef MAPPLICATIONMENUANIMATION_H
#define MAPPLICATIONMENUANIMATION_H

#include "mdialoganimation.h"
#include <mapplicationmenuanimationstyle.h>

//! \internal

class MApplicationMenuAnimationPrivate;

class MApplicationMenuAnimation : public MDialogAnimation
{
    Q_OBJECT
    M_ANIMATION_GROUP(MApplicationMenuAnimationStyle)

public:
    MApplicationMenuAnimation(QObject *parent = NULL);

protected:
    MApplicationMenuAnimation(MApplicationMenuAnimationPrivate *dd, QObject *parent = NULL);

private:
    Q_DECLARE_PRIVATE(MApplicationMenuAnimation)
};

//! \internal_end

#endif // MAPPLICATIONMENUANIMATION_H
