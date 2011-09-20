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

#include "transparentheadersheet.h"
#include "sheetspage.h"
#include "utils.h"

#include <MImageWidget>

#include <QAction>
#include <QDir>
#include <QGraphicsAnchorLayout>
#include <QTimer>

TransparentHeaderSheet::TransparentHeaderSheet()
    : MSheet()
{
    createHeaderWidget();
    createCentralWidget();
}

TransparentHeaderSheet::~TransparentHeaderSheet()
{
}

void TransparentHeaderSheet::createHeaderWidget()
{
    MBasicSheetHeader *headerWidget = new MBasicSheetHeader(this);
    headerWidget->setStyleName("Inverted");

    //% "Cancel"
    headerWidget->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_cancel"), headerWidget));
    connect(headerWidget->negativeAction(), SIGNAL(triggered()), SLOT(dismiss()));

    setHeaderWidget(headerWidget);
    setHeaderFloating(true);
}

void TransparentHeaderSheet::createCentralWidget()
{
    ClickableImageWidget *image = new ClickableImageWidget;
    image->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    image->setPixmap(QPixmap(Utils::mediaArtDir() + QDir::separator() + "photo_450x450_021.jpg"));
    connect(image, SIGNAL(clicked()), this, SLOT(toggleHeaderVisibility()));

    setCentralWidget(image);
}

void TransparentHeaderSheet::toggleHeaderVisibility()
{
    setHeaderVisible(!isHeaderVisible());
}
