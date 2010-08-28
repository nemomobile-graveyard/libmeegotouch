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

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <mapplication.h>
#include <mstylablewidget.h>
#include "ut_mstylablewidget.h"

MApplication *app(NULL);

class MyStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MyStyle)

    M_STYLE_ATTRIBUTE(bool, myStyleAttrubute1, myStyleAttrubute1);
    M_STYLE_ATTRIBUTE(QString, myStyleAttrubute2, myStyleAttrubute2);
};

class MyStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MyStyle)
};

class MyStylableClass : public MStylableWidget
{
public:
    MyStylableClass();

private:
    M_STYLABLE_WIDGET(MyStyle)
};

MyStylableClass::MyStylableClass() {}
MyStyleContainer::MyStyleContainer() : d_ptr(NULL) {}

MyStyleContainer::~MyStyleContainer() {}
const char* MyStyleContainer::styleType() const
{
    return "MyStyle";
}


//unit test class
void Ut_MStylableWidget::init()
{
}

void Ut_MStylableWidget::cleanup()
{
}

void Ut_MStylableWidget::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mstylablewidget" };
    app = new MApplication(argc, app_name);
}

void Ut_MStylableWidget::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MStylableWidget::testStyleType()
{
   MStylableWidget *myStylableWidget = new MStylableWidget();
   QVERIFY( myStylableWidget != NULL);
   QCOMPARE( myStylableWidget->styleType(), "MWidgetStyle");
   delete myStylableWidget;
}

void Ut_MStylableWidget::testMyStylableClass()
{
    MyStylableClass *myStylableClass = new MyStylableClass();
    QVERIFY( myStylableClass != NULL);
    QCOMPARE( myStylableClass->styleType(), "MyStyle");
    delete myStylableClass;
}

QTEST_APPLESS_MAIN(Ut_MStylableWidget)
