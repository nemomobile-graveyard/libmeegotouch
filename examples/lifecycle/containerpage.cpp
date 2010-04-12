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

#include "containerpage.h"
#include <MApplicationPage>
#include <MMashupCanvas>
#include <MContainer>
#include <MButton>
#include <MLocale>
#include <QGraphicsLinearLayout>
#include <MDebug>

ContainerPage::ContainerPage()
{
    setTitle("MMashupCanvas");
}

ContainerPage::~ContainerPage()
{
}

void ContainerPage::createContent()
{
    MApplicationPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    QGraphicsLinearLayout *vbox = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(vbox);

    MMashupCanvas *canvas = new MMashupCanvas("ContainerPage");
    canvas->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    vbox->addItem(canvas);
}

void ContainerPage::exitDisplayEvent()
{
    mDebug("") << "lifecycle: ContainerPage hidden";
}

void ContainerPage::enterDisplayEvent()
{
    mDebug("") << "lifecycle: ContainerPage visible";
}
