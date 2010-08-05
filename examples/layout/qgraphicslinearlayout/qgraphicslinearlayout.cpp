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
 * A simple example of the QGraphicsLinearLayout class.
 */

#include <QGraphicsLinearLayout>
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MLabel>
#include <MTheme>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("qgraphicslinearlayout.css");
    MApplicationWindow window;
    MApplicationPage page;
    /* Create a linear layout */
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);

    /* Add 3 items to the layout, arranging them vertically stacked on top of each other */
    for (int i = 0; i < 3; ++i) {
        MLabel *label = new MLabel(QString("Item %1").arg(i + 1));
        layout->addItem(label);
        label->setObjectName("item");
        label->setAlignment(Qt::AlignCenter);
    }

    /* Attach the layout to the page */
    page.centralWidget()->setLayout(layout);
    page.appear(&window);
    window.show();
    return app.exec();
}
