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

#include <MLabelView>
#include <MLabel>
#include "memorybenchmark.h"
#include "memorybenchmark.inl"

#include <QPainter>
#include <QPixmap>
#include <MApplication>
#include <MTheme>
#include <QTextDocument>
#include "mt_mlabel.h"

MApplication *app(NULL);

void Mt_MLabel::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./mt_mlabel";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
}

void Mt_MLabel::cleanupTestCase()
{
    delete app; app = NULL;
}

void Mt_MLabel::testLabelVsMWidgetController()
{
    // Create MWidgetController + MWidgetView combination
    beginMemoryBenchmark();
    MWidgetModel *m = new MWidgetModel;
    MWidgetController *c = new MWidgetController(m, NULL);
    MWidgetView *view = new MWidgetView(c);
    c->setView(view);
    qDebug() << "Memory consumption of MWidgetController + MWidgetView:";
    outputAllocatedMemorySize();

    delete c; c = NULL;
    QCoreApplication::processEvents();
    endMemoryBenchmark();

    // Create MLabel + MLabelView combination
    beginMemoryBenchmark();
    MLabelModel *lm = new MLabelModel;
    MLabel *l = new MLabel(NULL, lm);
    MLabelView *lv = new MLabelView(l);
    l->setView(lv);
    qDebug() << "Memory consumption of MLabel + MLabelView:";
    outputAllocatedMemorySize();

    delete l; l = NULL;
    QCoreApplication::processEvents();
    endMemoryBenchmark();
}

void Mt_MLabel::memoryBenchmark()
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

    MLabel **labels = new MLabel*[count];
    beginMemoryBenchmark();

    // Create as many labels as needed
    for (int i = 0; i < count; ++i) {
        labels[i] = new MLabel(text);
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

void Mt_MLabel::memoryBenchmark_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("count");

    for (int i = 1; i < 5; ++i) {
        QTest::newRow(qPrintable(QString("plaintext - %1 labels").arg(i))) << "Silence!" << i;
        QTest::newRow(qPrintable(QString("richtext - %1 labels").arg(i))) << "<span>Silence! I <b>kill</b> you!</span>" << i;
        QTest::newRow(qPrintable(QString("veryrichtext - %1 labels").arg(i))) << "<h5>Very rich text, multi line label</h5><p>The phrase \"<b>to be, or not to be</b>\" comes from <font color=\"white\">William Shakespeare's <i>Hamlet</i></font> (written about 1600), act three, scene one. It is one of the most famous quotations in <small>world literature</small> and the <u>best-known of this particular play</u>...<h6>And tables...</h6><table border=1><tr><td>Cell 1</td><td>Cell 2</td></tr></table></p>" << i;
    }
}

QTEST_APPLESS_MAIN(Mt_MLabel)
