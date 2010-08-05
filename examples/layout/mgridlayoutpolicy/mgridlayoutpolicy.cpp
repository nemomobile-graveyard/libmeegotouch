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
 * A simple example of the MGridLayoutPolicy class.
 */

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MLabel>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MTheme>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("mgridlayoutpolicy.css");
    MApplicationWindow window;
    MApplicationPage page;
    /* Create a MLayout that we set the policy for */
    MLayout *layout = new MLayout;
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    policy->setSpacing(10);

    /* Add 7 items to the policy, arranging them into a grid with 3 columns */
    for (int i = 0; i < 7; ++i) {
        MLabel *label = new MLabel(QString("Item %1").arg(i + 1));
        policy->addItem(label, i / 3, i % 3);
        label->setObjectName("item");
        label->setAlignment(Qt::AlignCenter);
    }

    /* Attach the layout to the page */
    page.centralWidget()->setLayout(layout);
    page.appear(&window);
    window.show();
    return app.exec();
}
