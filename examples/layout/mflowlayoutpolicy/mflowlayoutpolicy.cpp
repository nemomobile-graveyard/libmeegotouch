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
 * A simple example of the MFlowLayoutPolicy class.
 */

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MLabel>
#include <MFlowLayoutPolicy>
#include <MLayout>
#include <MTheme>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("mflowlayoutpolicy.css");
    MApplicationWindow window;
    MApplicationPage page;

    /* Create a MLayout that we set the policy for */
    MLayout *layout = new MLayout;

    MFlowLayoutPolicy *policy = new MFlowLayoutPolicy(layout);
    policy->setStyleName("example");
    policy->setRowLimit(2); //Only show 2 rows of items

    /* Add 20 items to the policy */
    for (int i = 1; i <= 20; ++i) {
        MLabel *label = new MLabel(QString("Item %1").arg(i));
        policy->addItem(label);
        policy->setAlignment(label, Qt::AlignCenter);
        label->setObjectName("item"); //Set CSS name, for styling
        label->setAlignment(Qt::AlignCenter);
        int random = i*10;
        label->setMaximumHeight(random);
        label->setMinimumHeight(random);
    }

    /* Attach the layout to the page */
    page.centralWidget()->setLayout(layout);
    page.appear(&window);
    window.show();
    return app.exec();
}
