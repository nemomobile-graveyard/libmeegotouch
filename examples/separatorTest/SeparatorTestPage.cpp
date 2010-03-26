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

#include "SeparatorTestPage.h"

#include <DuiApplicationPage>
#include <DuiButton>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiMashupCanvas>
#include <DuiSeparator>
#include <DuiWidgetView>

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
    DuiApplicationPage::createContent();
    QGraphicsWidget *panel = centralWidget();

    DuiLayout *layout = new DuiLayout(panel);
    panel->setLayout(layout);
    DuiGridLayoutPolicy *grid = new DuiGridLayoutPolicy(layout);

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
    grid->addItem(new DuiContainer("9", panel), 2, 2);
}

DuiButton *SeparatorTestPage::createButton(const QString &text)
{
    DuiButton *button = NULL;
    button = new DuiButton(text);
    QSizeF size(130.0, 130.0);
    button->setMinimumSize(size);
    button->setMaximumSize(size);
    button->setPreferredSize(size);
    return button;
}

DuiSeparator *SeparatorTestPage::createSeparator(Qt::Orientation orientation)
{
    DuiSeparator *separator = NULL;
    separator = new DuiSeparator(centralWidget(), orientation);
    return separator;
}
