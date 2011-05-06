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

#ifndef MLISTINDEXMODEL_H
#define MLISTINDEXMODEL_H

#include "mwidgetmodel.h"

class MList;

class MListIndexModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MListIndexModel)

public:
    M_MODEL_PTR_PROPERTY(MList*, list, List, true, NULL)
    M_MODEL_PROPERTY(int, displayMode, DisplayMode, true, 0)
};

#endif
