/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "viewheaderpage.h"

#include <MPannableViewport>
#include <MWidgetController>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MPositionIndicator>
#include <MBasicListItem>
#include <MAction>
#include <MContainer>

ViewHeaderPage::ViewHeaderPage()
    : TemplatePage(TemplatePage::ApplicationView)
{
}

ViewHeaderPage::~ViewHeaderPage()
{
}

QString ViewHeaderPage::timedemoTitle()
{
    return "ViewHeader";
}

void ViewHeaderPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    setStyleName(inv("CommonApplicationPage"));

    MLayout *layout = new MLayout();
    MLinearLayoutPolicy *layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->setContentsMargins(0, 0, 0, 0);
    layoutPolicy->setSpacing(0);

    MWidgetController *header = new MWidgetController();
    header->setStyleName(inv("CommonHeaderPanel"));
    header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    headerLabel = new MLabel(header);
    headerLabel->setStyleName(inv("CommonHeader"));
    layoutPolicy->addItem(header);

    fixedContainer = new MContainer();
    fixedContainer->setStyleName(inv("CommonContainer"));

    MLayout *fixedContainerLayout = new MLayout(fixedContainer->centralWidget());
    MLinearLayoutPolicy *fixedContainerPolicy = new MLinearLayoutPolicy(fixedContainerLayout, Qt::Vertical);
    fixedContainerPolicy->setContentsMargins(0, 0, 0, 0);
    fixedContainerPolicy->setSpacing(0);

    addFiller(fixedContainerPolicy);

    layoutPolicy->addItem(fixedContainer);

    viewport = new MPannableViewport();
    viewport->positionIndicator()->setStyleName(inv("CommonPositionIndicator"));
    viewport->setAutoRange(true);
    viewport->setMinimumHeight(500);
    layoutPolicy->addItem(viewport);

    pannableContainer = new MContainer();
    pannableContainer->setStyleName(inv("CommonContainer"));

    MLayout *pannableContainerLayout = new MLayout(pannableContainer->centralWidget());
    MLinearLayoutPolicy *pannableContainerPolicy = new MLinearLayoutPolicy(pannableContainerLayout, Qt::Vertical);
    pannableContainerPolicy->setContentsMargins(0, 0, 0, 0);
    pannableContainerPolicy->setSpacing(0);

    addFiller(pannableContainerPolicy);

    viewport->setWidget(pannableContainer);

    centralWidget()->setLayout(layout);

    /* Set up the toolbar */
    actionNormal = new MAction(this);
    actionNormal->setObjectName("normalAction");
    actionNormal->setLocation(MAction::ToolBarLocation);
    addAction(actionNormal);
    connect(actionNormal, SIGNAL(triggered()), this, SLOT(normal()));

    actionFixed = new MAction(this);
    actionFixed->setObjectName("fixedAction");
    actionFixed->setLocation(MAction::ToolBarLocation);
    addAction(actionFixed);
    connect(actionFixed, SIGNAL(triggered()), this, SLOT(fixed()));

    retranslateUi();
}

void ViewHeaderPage::addFiller(MLinearLayoutPolicy *policy)
{
    for (int i = 0; i < 30; i++) {
        MBasicListItem *li = new MBasicListItem(MBasicListItem::SingleTitle);
        li->setStyleName(inv("CommonBasicListItem"));
        li->setTitle("Filler");
        policy->addItem(li);
    }
}

void ViewHeaderPage::normal()
{
    setPannable(true);
    viewport->setMaximumHeight(0);
    viewport->setVisible(false);
    fixedContainer->setVisible(true);
    fixedContainer->setMaximumHeight(-1);
    //% "View Header"
    headerLabel->setText(qtTrId("xx_viewheader_page_view_header"));
}

void ViewHeaderPage::fixed()
{
    setPannable(false);
    fixedContainer->setMaximumHeight(0);
    fixedContainer->setVisible(false);
    viewport->setVisible(true);
    viewport->setMaximumHeight(-1);
    //% "Fixed View Header"
    headerLabel->setText(qtTrId("xx_viewheader_page_fixed_view_header"));
}

void ViewHeaderPage::retranslateUi()
{
    //% "View Header"
    setTitle(qtTrId("xx_viewheader_page_title"));
    if (!isContentCreated())
        return;

    //% "View Header"
    headerLabel->setText(qtTrId("xx_viewheader_page_view_header"));

    //% "Fixed Content"
    fixedContainer->setTitle(qtTrId("xx_viewheader_page_fixed_content"));

    //% "Pannable Content"
    pannableContainer->setTitle(qtTrId("xx_viewheader_page_pannable_content"));

    //% "Normal"
    actionNormal->setText(qtTrId("xx_view_header_page_normal"));

    //% "Fixed"
    actionFixed->setText(qtTrId("xx_view_header_page_fixed"));
}
