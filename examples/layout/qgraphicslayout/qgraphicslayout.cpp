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
 * A simple example of a layout within a layout, using the Qt layout classes.
 */

#include <QGraphicsLinearLayout>
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MButton>
#include <MLabel>
#include <MTheme>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("qgraphicslayout.css");
    MApplicationWindow window;
    MApplicationPage page;

    QGraphicsLinearLayout *outerLayout = new QGraphicsLinearLayout(Qt::Vertical, page.centralWidget());
    QGraphicsLinearLayout *firstRow = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout *secondRow = new QGraphicsLinearLayout(Qt::Horizontal);
    outerLayout->addItem(firstRow);
    outerLayout->addItem(secondRow);
    firstRow->addItem(new MLabel("Label on first row"));
    firstRow->addItem(new MButton("Button"));
    firstRow->addStretch();
    secondRow->addItem(new MLabel("Label on second row"));
    secondRow->addItem(new MButton("Button 1"));
    secondRow->addItem(new MButton("Button 2"));
    secondRow->addStretch();

    page.appear(&window);
    window.show();
    return app.exec();
}
