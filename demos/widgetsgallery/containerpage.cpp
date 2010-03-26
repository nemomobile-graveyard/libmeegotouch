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

#include "containerpage.h"
#include <DuiApplicationPage>
#include <DuiMashupCanvas>
#include <DuiContainer>
#include <DuiButton>
#include <DuiLocale>
#include <QGraphicsLinearLayout>

ContainerPage::ContainerPage()
{
    gid = TemplatePage::ContainersAndApplets;
}

ContainerPage::~ContainerPage()
{
}

QString ContainerPage::timedemoTitle()
{
    return "Container";
}

void ContainerPage::createContent()
{
    DuiApplicationPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    QGraphicsLinearLayout *vbox = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(vbox);

#ifdef HAVE_DBUS
    DuiMashupCanvas *canvas = new DuiMashupCanvas("ContainerPage");

    // canvas->setContainerMode(false);

    vbox->addItem(canvas);
#endif

    retranslateUi();
}

void ContainerPage::retranslateUi()
{
    //% "Container"
    setTitle(qtTrId("xx_container_page_title"));
    if (!isContentCreated())
        return;
    // this file has no other calls to qtTrId() except for the title
    // at the moment. If more qtTrId() calls are needed they should
    // be added here.
}

