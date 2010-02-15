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
 * A simple example of a layout within a layout, using the Qt layout classes.
 */

#include <QGraphicsLinearLayout>
#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiButton>
#include <DuiLabel>
#include <DuiTheme>

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiTheme::loadCSS("qgraphicslayout.css");
    DuiApplicationWindow window;
    DuiApplicationPage page;

    QGraphicsLinearLayout *outerLayout = new QGraphicsLinearLayout(Qt::Vertical, page.centralWidget());
    QGraphicsLinearLayout *firstRow = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsLinearLayout *secondRow = new QGraphicsLinearLayout(Qt::Horizontal);
    outerLayout->addItem(firstRow);
    outerLayout->addItem(secondRow);
    firstRow->addItem(new DuiLabel("Label on first row"));
    firstRow->addItem(new DuiButton("Button"));
    firstRow->addStretch();
    secondRow->addItem(new DuiLabel("Label on second row"));
    secondRow->addItem(new DuiButton("Button 1"));
    secondRow->addItem(new DuiButton("Button 2"));
    secondRow->addStretch();

    window.show();
    page.appearNow();
    return app.exec();
}
