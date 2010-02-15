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
 * A simple example of the QGraphicsLinearLayout class.
 */

#include <QGraphicsLinearLayout>
#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiLabel>
#include <DuiTheme>

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiTheme::loadCSS("qgraphicslinearlayout.css");
    DuiApplicationWindow window;
    DuiApplicationPage page;
    /* Create a linear layout */
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);

    /* Add 3 items to the layout, arranging them vertically stacked on top of each other */
    for (int i = 0; i < 3; ++i) {
        DuiLabel *label = new DuiLabel(QString("Item %1").arg(i + 1));
        layout->addItem(label);
        label->setObjectName("item");
        label->setAlignment(Qt::AlignCenter);
    }

    /* Attach the layout to the page */
    page.centralWidget()->setLayout(layout);
    window.show();
    page.appearNow();
    return app.exec();
}
