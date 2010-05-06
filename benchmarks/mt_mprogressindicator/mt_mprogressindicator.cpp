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

#include "memorybenchmark.h"
#include "memorybenchmark.inl"
#include "mt_mprogressindicator.h"
#include <MProgressIndicator>
#include <MProgressIndicatorCircularView>
#include <MProgressIndicatorBarView>
#include <QPainter>
#include <QPixmap>
#include <MApplication>
#include <MTheme>

MApplication *app;

void Mt_MProgressIndicator::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./widgetsgallery";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
    MTheme::instance()->changeTheme("common");

    // We need to warmup QTs painters and probably
    // some other subsystems
    QPixmap pixmap(100, 30);
    QPainter painter(&pixmap);
    MProgressIndicator *progressindicator = new MProgressIndicator();
    progressindicator->setGeometry(QRectF(0, 0, 100, 100));
    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    progressindicator->paint(&painter, NULL);
    delete progressindicator;
}

void Mt_MProgressIndicator::cleanupTestCase()
{
    delete app;
}

void Mt_MProgressIndicator::memoryBenchmark()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(qint32, viewIndex);
    QFETCH(int, count);

    ViewName currentViewName  = (ViewName)viewIndex;
    MWidgetView *currentView;

    /*
    We want to measure how much memory will be used by progressindicator
    when it's painted on a screen (buffer). We don't take into
    account pixmap and painter. Also since graphics resource
    loading is asynchronous we need to make sure that DuTheme
    deamon loaded all pictures.
    */
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);

    QList<MProgressIndicator *> progressindicators;

    beginMemoryBenchmark();

    // Create as many progressindicators as needed
    for (int i = 0; i < count; ++i) {
        MProgressIndicator *progressindicator = new MProgressIndicator();
        progressindicator->setMinimumSize(width, height);
        progressindicator->setMaximumSize(width, height);
        progressindicator->setRange(0, 100);
        progressindicator->setValue(50);
        switch (currentViewName) {
        case Default:
            currentView = new MProgressIndicatorCircularView(progressindicator);
            break;
        case Bar:
            currentView = new MProgressIndicatorBarView(progressindicator);
            break;
        default:
            break;
        }
        if (currentView) {
            progressindicator->setView(currentView);   // transfers ownership to controller
        }
        progressindicators.append(progressindicator);
    }

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    // Paint all progressindicators
    foreach(MProgressIndicator * progressindicator, progressindicators) {
        progressindicator->paint(&painter, NULL);
    }

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release all progressindicators
    qDeleteAll(progressindicators);

    endMemoryBenchmark();


}

void Mt_MProgressIndicator::memoryBenchmark_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<qint32>("viewIndex");
    QTest::addColumn<int>("count");

    for (qint32 viewIndex = FirstView; viewIndex < NoViews; viewIndex++) {
        for (int i = 1; i < 5; ++i) {
            QTest::newRow("864x64") << 864 / 2 << 64 << viewIndex << i;
            QTest::newRow("864x128") << 864 / 2 << 128 << viewIndex << i;
        }
    }
}

QTEST_APPLESS_MAIN(Mt_MProgressIndicator)

