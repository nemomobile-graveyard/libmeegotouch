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
 * A simple example of having a two-column layout
 */

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiButton>
#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiTextEdit>
#include <DuiTheme>
#include <QDebug>

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiApplicationWindow window;
    DuiApplicationPage page;
    DuiTheme::loadCSS("twocolumns.css");
    /* Create a DuiLayout that we set the policy for */
    DuiLayout *layout = new DuiLayout(page.centralWidget());
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);

    /* Setup first layout with a label and text edit */
    DuiLayout *nameLayout = new DuiLayout;
    DuiLinearLayoutPolicy *namePolicy = new DuiLinearLayoutPolicy(nameLayout, Qt::Horizontal);
    DuiLabel *textEditLabel = new DuiLabel("Name:");
    DuiTextEdit *textEdit = new DuiTextEdit(DuiTextEditModel::MultiLine);
    namePolicy->addItem(textEditLabel);  //Add the label and textedit
    namePolicy->addItem(textEdit);
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    /* Setup second layout with a large label */
    DuiLayout *labelLayout = new DuiLayout;
    DuiLinearLayoutPolicy *labelPolicy = new DuiLinearLayoutPolicy(labelLayout, Qt::Horizontal);
    DuiLabel *label = new DuiLabel("Enter the name of the person who likes to listen to music while sorting their socks!");
    label->setObjectName("nameLabel");
    labelPolicy->addItem(label);
    label->setWordWrap(true);

    /* Add the two layouts to the layout */
    policy->addItem(nameLayout);
    policy->addItem(labelLayout);

    /* Make the two layouts have an equal preferred size, so that they get an equal amount of space */
    nameLayout->setPreferredWidth(1);
    labelLayout->setPreferredWidth(1);

    /* Attach the layout to the page */
    window.show();
    page.appearNow();

    return app.exec();
}
