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
 * A simple example of the MFreestyleLayoutPolicy class.
 */

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MLabel>
#include <MFreestyleLayoutPolicy>
#include <MLayout>
#include <MTheme>
#include <math.h> //For sin and cos
int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("mfreestylelayoutpolicy.css");

    /* Create a MLayout that we set the policy for */
    MLayout *layout = new MLayout;
    MFreestyleLayoutPolicy *policy = new MFreestyleLayoutPolicy(layout);
    policy->setSpacing(10);

    MApplicationWindow window;
    MApplicationPage page;
    /* Attach the layout to the page */
    page.centralWidget()->setLayout(layout);
    page.appear(&window);
    window.show();

    int n = 20;
    for (int i = 0; i < n; ++i) {
        MLabel *label = new MLabel(QString("Item %1").arg(i + 1));
        label->setObjectName("item");
        //Place the items in an ellipse with the width and height of the page
        int center_x = label->preferredWidth() / 2 + (page.geometry().width() - label->preferredWidth()) / 2 * (1 - cos(2 * 3.13 * i / n));
        int center_y = 25 + (page.geometry().height() - 50) / 3 * (1 - sin(2 * 3.13 * i / n));
        policy->addItemAtGeometry(label, QRect(center_x - label->preferredWidth() / 2, center_y - label->preferredHeight() / 2, label->preferredWidth(), label->preferredHeight()));
        label->setAlignment(Qt::AlignCenter);
    }

    return app.exec();
}
