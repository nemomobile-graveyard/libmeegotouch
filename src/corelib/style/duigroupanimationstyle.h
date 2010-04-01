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

#ifndef DUIGROUPANIMATIONSTYLE_H
#define DUIGROUPANIMATIONSTYLE_H

#include <duianimationstyle.h>

class DUI_EXPORT DuiGroupAnimationStyle : public DuiAnimationStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiGroupAnimationStyle)
};

class DuiGroupAnimationStyleContainer : public DuiAnimationStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiGroupAnimationStyle)
};

#endif
