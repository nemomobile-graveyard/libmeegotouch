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

#ifndef TESTOBJECT3STYLE_H
#define TESTOBJECT3STYLE_H

#include <QFont>
#include <QPoint>
#include <QTextCharFormat>

#include "testobject2style.h"

class QPixmap;
class DuiScalableImage;

class TestObject3Style : public TestObject2Style
{
    Q_OBJECT
    DUI_STYLE(TestObject3Style)
};

class TestObject3StyleContainer : public TestObject2StyleContainer
{
    DUI_STYLE_CONTAINER(TestObject3Style)
};

#endif

