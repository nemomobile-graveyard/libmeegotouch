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

#ifndef MANIMATION_P_H
#define MANIMATION_P_H

class MAnimationStyleContainer;

class MAnimationPrivate
{
    Q_DECLARE_PUBLIC(MAnimation)
public:
    virtual ~MAnimationPrivate() {}
protected:
    class MAnimation *q_ptr;
private:
    MAnimationStyleContainer *styleContainer;
};

#endif
