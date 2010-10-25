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
 * testwidgetmodel.h
 *
 *  Created on: May 19, 2009
 *      Author: Janne Koivuranta
 */


#ifndef testwidgetmodel_h
#define testwidgetmodel_h

#include <MApplication>
#include <MWidgetController>
#include <mwidgetmodel.h>

class TestWidgetModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(TestWidgetModel)

public:
    enum ButtonState {
        Pressed,
        Released
    };


private:
    // Index of current style. Fourth parameter is false so public handler methods are not automatically generated.
    // They are implemented in source file.
    M_MODEL_PROPERTY(int, styleIndex, StyleIndex, false, 0)
};


#endif

