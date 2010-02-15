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

#ifndef TESTOBJECT2STYLE_H
#define TESTOBJECT2STYLE_H

#include <QFont>
#include <QPoint>
#include <QTextCharFormat>

#include "testobjectstyle.h"

class QPixmap;
class DuiScalableImage;

class TestObject2Style : public TestObjectStyle
{
    Q_OBJECT
    DUI_STYLE(TestObject2Style)

    DUI_STYLE_ATTRIBUTE(int,        attributeInteger2,       AttributeInteger2)
    DUI_STYLE_ATTRIBUTE(QString,    attributeString2,        AttributeString2)
};

class TestObject2StyleContainer : public TestObjectStyleContainer
{
    DUI_STYLE_CONTAINER(TestObject2Style)
};

#endif

