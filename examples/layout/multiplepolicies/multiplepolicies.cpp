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
 * A simple example of a layout with both a MLinearLayoutPolicy and a MFlowLayoutPolicy.
 */

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <MFlowLayoutPolicy>
#include <MLayout>
#include <MTheme>
#include <MSceneManager>
#include <QObject>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MTheme::loadCSS("multiplepolicies.css");
    MApplicationWindow window;
    MApplicationPage page;

    /* Create a MLayout that we set the policies for */
    MLayout *layout = new MLayout(page.centralWidget());
    MLinearLayoutPolicy *linearPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    MFlowLayoutPolicy *flowPolicy = new MFlowLayoutPolicy(layout);
    for (int i = 0; i < 10; ++i) {
        MLabel *label = new MLabel(QString("Item %1").arg(i + 1));
        label->setAlignment(Qt::AlignCenter);
        label->setObjectName("item"); //Set CSS name for styling

        flowPolicy->addItem(label); // Add all of the items to the flow policy
        if (i < 3)  // But only add the first 3 items to the linear policy
            linearPolicy->addItem(label);
        else
            label->setObjectName("flowonly"); //Set CSS name, for styling flow layout items differently
    }
    /* Switch from a linear policy in landscape mode to a flow policy in portrait mode */
    layout->setLandscapePolicy(linearPolicy);
    layout->setPortraitPolicy(flowPolicy);

    page.appear(&window);
    window.show();
    return app.exec();
}
