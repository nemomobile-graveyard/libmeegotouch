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

#include <QObject>
#include <QDebug>
#include <QStringListModel>
#include <MTheme>
#include <MList>
#include <MApplication>

#include "memorybenchmark.h"
#include "memorybenchmark.inl"
#include "mt_mlist.h"

MApplication *app(NULL);

void Mt_MList::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./mt_mlist";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
}

void Mt_MList::cleanupTestCase()
{
    delete app; app = NULL;
}

void Mt_MList::memoryBenchmark_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<qint32>("labelHeight");
    QTest::addColumn<QStringList>("text");

    QTest::newRow("64x64")    << 64 << 64 << 16 << (QStringList() << "Silence" << "is" << "disturbing!");
    QTest::newRow("128x128")  << 128  << 128 << 40 << (QStringList() << "Silence" << "is" << "not" << "disturbing!");
    QTest::newRow("256x256")  << 256  << 256 << 60 << (QStringList() << "XXXX" << "XXXX" << "XXXX" \
                              << "XXXX" << "XXXX" << "XXXX" \
                              << "XXXX" << "XXXX" << "XXXX");
}

void Mt_MList::memoryBenchmark()
{

    // get size dimensions for test
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(qint32, labelHeight);
    QFETCH(QStringList, text);

    Q_UNUSED(labelHeight);

    // create pixmap paintdevice
    QPixmap *pixmap = new QPixmap(width, height);
    pixmap->fill(QColor(0, 0, 0, 0));
    QPainter *painter = new QPainter(pixmap);
    QStyleOptionGraphicsItem* option = new QStyleOptionGraphicsItem();

    beginMemoryBenchmark();

    // create widget, set size
    MList *list = new MList();
    list->setMinimumSize(width, height);
    list->setMaximumSize(width, height);

    QStringListModel *stringModel = new QStringListModel(text);

    list->setItemModel(stringModel);

    list->paint(painter, option);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    delete list; list = NULL;

    endMemoryBenchmark();

    delete option; option = NULL;
    delete painter;painter = NULL;
    delete pixmap; pixmap = NULL;
}

QTEST_APPLESS_MAIN(Mt_MList)
