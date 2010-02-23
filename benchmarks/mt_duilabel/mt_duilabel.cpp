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

#include <DuiLabelView>
#include <DuiLabel>
#include "memorybenchmark.h"
#include "memorybenchmark.inl"

#include <QPainter>
#include <QPixmap>
#include <DuiApplication>
#include <DuiTheme>
#include <QTextDocument>
#include "mt_duilabel.h"

DuiApplication *app(NULL);

void Mt_DuiLabel::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./mt_duilabel";
    argv[0] = appName;
    app = new DuiApplication(argc, (char **)argv);
}

void Mt_DuiLabel::cleanupTestCase()
{
    delete app; app = NULL;
}

void Mt_DuiLabel::testLabelVsDuiWidgetController()
{
    // Create DuiWidgetController + DuiWidgetView combination
    beginMemoryBenchmark();
    DuiWidgetModel *m = new DuiWidgetModel;
    DuiWidgetController *c = new DuiWidgetController(m, NULL);
    DuiWidgetView *view = new DuiWidgetView(c);
    c->setView(view);
    qDebug() << "Memory consumption of DuiWidgetController + DuiWidgetView:";
    outputAllocatedMemorySize();

    delete c; c = NULL;
    QCoreApplication::processEvents();
    endMemoryBenchmark();

    // Create DuiLabel + DuiLabelView combination
    beginMemoryBenchmark();
    DuiLabelModel *lm = new DuiLabelModel;
    DuiLabel *l = new DuiLabel(NULL, lm);
    DuiLabelView *lv = new DuiLabelView(l);
    l->setView(lv);
    qDebug() << "Memory consumption of DuiLabel + DuiLabelView:";
    outputAllocatedMemorySize();

    delete l; l = NULL;
    QCoreApplication::processEvents();
    endMemoryBenchmark();
}

void Mt_DuiLabel::memoryBenchmark()
{
    QFETCH(QString, text);
    QFETCH(int, count);

    /*
    We want to measure how much memory will be used by label
    when it's painted on a screen (buffer). We don't take into
    account pixmap and painter. Also since graphics resource
    loading is asynchronous we need to make sure that DuTheme
    deamon loaded all pictures.
    */
    QPixmap pixmap(100, 30);
    QPainter painter(&pixmap);

    DuiLabel **labels = new DuiLabel*[count];
    beginMemoryBenchmark();

    // Create as many labels as needed
    for (int i = 0; i < count; ++i) {
        labels[i] = new DuiLabel(text);
        labels[i]->boundingRect();
        labels[i]->setGeometry(QRectF(0, 0, 100, 30));
    }

    // Paint all labels
    for (int i = 0; i < count; ++i) {
        //labels[i]->paint(&painter, NULL);
    }

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release all labels
    for (int i = 0; i < count; ++i) {
        delete labels[i]; labels[i] = NULL;
    }
    delete[] labels;

    QCoreApplication::processEvents();
    endMemoryBenchmark();
}

void Mt_DuiLabel::memoryBenchmark_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("count");

    for (int i = 1; i < 5; ++i) {
        QTest::newRow(qPrintable(QString("plaintext - %1 labels").arg(i))) << "Silence!" << i;
        QTest::newRow(qPrintable(QString("richtext - %1 labels").arg(i))) << "<span>Silence! I <b>kill</b> you!</span>" << i;
        QTest::newRow(qPrintable(QString("veryrichtext - %1 labels").arg(i))) << "<h5>Very rich text, multi line label</h5><p>The phrase \"<b>to be, or not to be</b>\" comes from <font color=\"white\">William Shakespeare's <i>Hamlet</i></font> (written about 1600), act three, scene one. It is one of the most famous quotations in <small>world literature</small> and the <u>best-known of this particular play</u>...<h6>And tables...</h6><table border=1><tr><td>Cell 1</td><td>Cell 2</td></tr></table></p>" << i;
    }
}

QTEST_APPLESS_MAIN(Mt_DuiLabel)
