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
 * A simple example of a layout with both a DuiLinearLayoutPolicy and a DuiFlowLayoutPolicy.
 */

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiLabel>
#include <DuiLinearLayoutPolicy>
#include <DuiFlowLayoutPolicy>
#include <DuiLayout>
#include <DuiTheme>
#include <DuiSceneManager>
#include <QObject>

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiTheme::loadCSS("multiplepolicies.css");
    DuiApplicationWindow window;
    DuiApplicationPage page;

    /* Create a DuiLayout that we set the policies for */
    DuiLayout *layout = new DuiLayout(page.centralWidget());
    DuiLinearLayoutPolicy *linearPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    DuiFlowLayoutPolicy *flowPolicy = new DuiFlowLayoutPolicy(layout);
    for (int i = 0; i < 10; ++i) {
        DuiLabel *label = new DuiLabel(QString("Item %1").arg(i + 1));
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

    window.show();
    page.appearNow();
    return app.exec();
}
