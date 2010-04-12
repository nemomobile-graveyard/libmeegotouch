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

#include "SeparatorTestPage.h"

#include <MApplicationPage>
#include <MButton>
#include <MContainer>
#include <MGridLayoutPolicy>
#include <MLocale>
#include <MLabel>
#include <MLayout>
#include <MMashupCanvas>
#include <MSeparator>
#include <MWidgetView>

SeparatorTestPage::SeparatorTestPage()
{
    //% "Separator"
    setTitle(qtTrId("xx_separator_test_page_title"));
    setObjectName("separatortestpage");
}

SeparatorTestPage::~SeparatorTestPage()
{
}

void SeparatorTestPage::createContent()
{
    MApplicationPage::createContent();
    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    panel->setLayout(layout);
    MGridLayoutPolicy *grid = new MGridLayoutPolicy(layout);

    //  1  |  3
    //  –  5  –
    //  7  |  9
    //
    grid->addItem(createButton("1"), 0, 0);
    grid->addItem(createSeparator(Qt::Vertical), 0, 1);
    grid->addItem(createButton("3"), 0, 2);
    grid->addItem(createSeparator(Qt::Horizontal), 1, 0);
    grid->addItem(createButton("5"), 1, 1);
    grid->addItem(createSeparator(Qt::Horizontal), 1, 2);
    grid->addItem(createButton("7"), 2, 0);
    grid->addItem(createSeparator(Qt::Vertical), 2, 1);
    grid->addItem(new MContainer("9", panel), 2, 2);
}

MButton *SeparatorTestPage::createButton(const QString &text)
{
    MButton *button = NULL;
    button = new MButton(text);
    QSizeF size(130.0, 130.0);
    button->setMinimumSize(size);
    button->setMaximumSize(size);
    button->setPreferredSize(size);
    return button;
}

MSeparator *SeparatorTestPage::createSeparator(Qt::Orientation orientation)
{
    MSeparator *separator = NULL;
    separator = new MSeparator(centralWidget(), orientation);
    return separator;
}
