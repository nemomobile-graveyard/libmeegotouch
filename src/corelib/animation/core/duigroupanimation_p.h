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

#ifndef DUIGROUPANIMATION_P_H
#define DUIGROUPANIMATION_P_H

class QAnimationGroup;

#include "duianimation_p.h"
#include "duigroupanimation.h"

class DuiGroupAnimationPrivate : public DuiAnimationPrivate
{
    Q_DECLARE_PUBLIC(DuiGroupAnimation)
private:
    class QAnimationGroup *group;

public:
    void init(DuiGroupAnimation::Type type);
};

#endif
