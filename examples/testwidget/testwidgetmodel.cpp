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

/*
 * testwidgetmodel.cpp
 *
 *  Created on: May 20, 2009
 *      Author: Janne Koivuranta
 */

#include "testwidgetmodel.h"

const int &TestWidgetModel::styleIndex() const
{
    // Call private method to access style index variable.
    return _styleIndex();
}

void TestWidgetModel::setStyleIndex(const int &styleIndex)
{
    // Notice that hand-written setters and getters can perform validity checks, which is not
    // the case for automatically generated handlers.
    _styleIndex() = styleIndex;
    if (_styleIndex() > 5)
        _styleIndex() = 5;
}
