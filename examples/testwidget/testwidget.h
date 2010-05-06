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
 * testwidget.h
 *
 *  Created on: May 19, 2009
 *      Author: Janne Koivuranta
 */


#ifndef TESTWIDGET_H
#define TESTWIDGET_H


#include <MWidgetController>
#include "testwidgetmodel.h"

class TestWidget : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(TestWidget)

public:
    TestWidget(MWidget *parent = NULL);
    virtual ~TestWidget();
};

#endif
