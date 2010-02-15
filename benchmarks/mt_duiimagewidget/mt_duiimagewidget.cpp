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

#include "mt_duiimagewidget.h"

#include "memorybenchmark.h"
#include "memorybenchmark.inl"

#include <QPainter>
#include <QPixmap>
#include <DuiImageWidget>
#include <DuiApplication>
#include <DuiTheme>

DuiApplication *app;

void Mt_DuiImageWidget::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./mt_duiimage";
    app = new DuiApplication(argc, &app_name);
    DuiTheme::addPixmapDirectory(QCoreApplication::applicationDirPath());
}

void Mt_DuiImageWidget::cleanupTestCase()
{
    delete app;
}

void Mt_DuiImageWidget::memory300over300png()
{
    int width = 300, height = 300;
    QString imagename("test");

    // create pixmap paintdevice
    QPixmap pixmap(width, height);
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    DuiImageWidget *image = new DuiImageWidget();

    image->setImage(imagename);
    image->setGeometry(QRectF(0, 0, width, height));

    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    image->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    delete image;
    endMemoryBenchmark();
}

QTEST_APPLESS_MAIN(Mt_DuiImageWidget)

