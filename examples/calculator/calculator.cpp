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
 * This demonstrates a simple calculator widget that can work in both portrait and landscape mode.
 */

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiTheme>
#include "calculatorwidget.h"

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiTheme::loadCSS("calculator.css");
    //Load the example svg file in the same directory
    DuiTheme::addPixmapDirectory(".", Dui::NonRecursive);
    DuiApplicationWindow window;
    DuiApplicationPage page;

    CalculatorWidget *calculatorWidget = new CalculatorWidget;

    page.setCentralWidget(calculatorWidget);
    window.show();
    page.appearNow();
    return app.exec();
}
