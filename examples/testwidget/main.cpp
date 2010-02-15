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
 * testwidgetview.h
 *
 *  Created on: May 19, 2009
 *      Author: Janne Koivuranta
 */



#include <DuiTheme>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>

#include "testwidget.h"
#include "testwidgetview.h"

int main(int argc, char **argv)
{
    // Create main dui application.
    DuiApplication application(argc, argv);

    // Create application window and make it visible.
    DuiApplicationWindow window;
    window.show();

    // Create application page and make it visible.
    DuiApplicationPage page;
    page.appear();

    // Load widget style from CSS file.
    DuiTheme::loadCSS("testwidgetstyle.css");

    // Create widget controller and view.
    TestWidget widget;
    TestWidgetView view(&widget);
    widget.setView(&view);

    // Set geometry (active area) of widget.
    widget.setGeometry(QRectF(100, 100, 200, 200));

    // Attach widget to parent widget so it will be drawn and run dui application.
    widget.setParentItem(page.centralWidget());
    return application.exec();
}











