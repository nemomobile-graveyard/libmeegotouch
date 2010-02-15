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

/*
 * testwidgetstyle.h
 *
 *  Created on: May 19, 2009
 *      Author: Janne Koivuranta
 */


#ifndef TESTWIDGETSTYLE_H
#define TESTWIDGETSTYLE_H

#include <QColor>
#include <duiwidgetstyle.h>

class TestWidgetStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(TestWidgetStyle)
    DUI_STYLE_ATTRIBUTE(QColor, color, Color)
};

class TestWidgetStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(TestWidgetStyle)
    DUI_STYLE_MODE(Green)
    DUI_STYLE_MODE(Blue)
    DUI_STYLE_MODE(Yellow)
    DUI_STYLE_MODE(Cyan)
    DUI_STYLE_MODE(Purple)
};


#endif
