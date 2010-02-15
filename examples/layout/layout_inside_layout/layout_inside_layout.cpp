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
 * A simple example of a layout within a layout where the inner layout can be hidden.
 */

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiLabel>
#include <DuiLinearLayoutPolicy>
#include <QGraphicsGridLayout>
#include <DuiLayout>
#include <DuiTheme>
#include <DuiSceneManager>
#include <QGraphicsWidget>

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiTheme::loadCSS("layout_inside_layout.css");
    DuiApplicationWindow window;
    DuiApplicationPage page;

    /* Create a DuiLayout that we set the policies for */
    DuiLayout *layout = new DuiLayout;
    DuiLinearLayoutPolicy *portraitPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiLinearLayoutPolicy *landscapePolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    for (int i = 0; i < 3; ++i) {
        DuiLabel *label = new DuiLabel(QString("Item %1").arg(i + 1));
        label->setAlignment(Qt::AlignCenter);
        label->setObjectName("item"); //Set CSS name for styling
        portraitPolicy->addItem(label);
        landscapePolicy->addItem(label);
    }
    /* Create a widget to hold the inner layout and to put inside the policy */
    QGraphicsWidget *widget = new QGraphicsWidget;
    /* Create an inner layout.  A QGraphicsGridLayout is used since we don't require
     * multiple policies here, but using a DuiLayout would also work. */
    QGraphicsGridLayout *innerLayout = new QGraphicsGridLayout(widget);
    for (int i = 0; i < 4; ++i) {
        DuiLabel *label = new DuiLabel(QString("Inner Item %1").arg(i + 1));
        label->setAlignment(Qt::AlignCenter);
        label->setObjectName("inneritem"); //Set CSS name for styling
        innerLayout->addItem(label, i / 2, i % 2);
    }
    /* Now add the widget to the landscape policy only, so that the innerLayout is hidden when device is rotated */
    landscapePolicy->addItem(widget);
    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    /* Attach the layout to the page */
    page.centralWidget()->setLayout(layout);
    window.show();
    page.appearNow();
    return app.exec();
}
