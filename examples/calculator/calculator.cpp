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
 * This demonstrates a simple calculator widget that can work in both portrait and landscape mode.
 */

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MTheme>
#include "calculatorwidget.h"

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("calculator.css");
    //Load the example svg file in the same directory
    MTheme::addPixmapDirectory(".", M::NonRecursive);
    MApplicationWindow window;
    MApplicationPage page;

    CalculatorWidget *calculatorWidget = new CalculatorWidget;

    page.setCentralWidget(calculatorWidget);
    page.appear(&window);
    window.show();
    return app.exec();
}
