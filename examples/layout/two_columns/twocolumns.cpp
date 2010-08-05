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
 * A simple example of having a two-column layout
 */

#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MButton>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MTextEdit>
#include <MTheme>
#include <QDebug>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    MApplicationPage page;
    MTheme::loadCSS("twocolumns.css");
    /* Create a MLayout that we set the policy for */
    MLayout *layout = new MLayout(page.centralWidget());
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);

    /* Setup first layout with a label and text edit */
    MLayout *nameLayout = new MLayout;
    MLinearLayoutPolicy *namePolicy = new MLinearLayoutPolicy(nameLayout, Qt::Horizontal);
    MLabel *textEditLabel = new MLabel("Name:");
    MTextEdit *textEdit = new MTextEdit(MTextEditModel::MultiLine);
    namePolicy->addItem(textEditLabel);  //Add the label and textedit
    namePolicy->addItem(textEdit);
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    /* Setup second layout with a large label */
    MLayout *labelLayout = new MLayout;
    MLinearLayoutPolicy *labelPolicy = new MLinearLayoutPolicy(labelLayout, Qt::Horizontal);
    MLabel *label = new MLabel("Enter the name of the person who likes to listen to music while sorting their socks!");
    label->setObjectName("nameLabel");
    labelPolicy->addItem(label);
    label->setWordWrap(true);

    /* Add the two layouts to the layout */
    policy->addItem(nameLayout);
    policy->addItem(labelLayout);

    /* Make the two layouts have an equal preferred size, so that they get an equal amount of space */
    nameLayout->setPreferredWidth(1);
    labelLayout->setPreferredWidth(1);

    page.appear(&window);
    window.show();

    return app.exec();
}
