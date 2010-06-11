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
#include <MTheme>
#include <MLabel>
#include <MLabelView>
#include <MApplication>

#include "pt_mlabel.h"

MApplication *app;

void Pt_MLabel::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./pt_mlabel";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
}

void Pt_MLabel::cleanupTestCase()
{
    delete app;
}

void Pt_MLabel::init()
{
    QFETCH(QString, text);

    // create widget, set size
    subject = new MLabel(text);
    subject->setMaximumWidth(864);
    currentView = new MLabelView(subject);
    subject->setView(currentView);   // transfers ownership to controller

    // wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

    // create pixmap paintdevice
    pixmap = new QPixmap(846, 480);
    pixmap->fill(QColor(0, 0, 0, 0));
    painter = new QPainter(pixmap);
}

void Pt_MLabel::cleanup()
{
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << "view_"
            << ".png";
    if (!written.contains(kuva)) {
        pixmap->save(kuva, "png", -1);
        written.append(kuva);
    }
#endif

    delete subject;
    subject = 0;
    delete painter;
    painter = 0;
    delete pixmap;
    pixmap = 0;
}

void Pt_MLabel::paintPerformance()
{
    QBENCHMARK {
        currentView->paint(painter, NULL);
    }
}

void Pt_MLabel::paintPerformance_data()
{
    QTest::addColumn<QString>("text");

    QTest::newRow("plaintext") << "Silence!";
    QTest::newRow("richtext") << "<span>Silence! I <b>kill</b> you!</span>";
    QTest::newRow("veryrich") << "<h5>Very rich text, multi line label</h5><p>The phrase \"<b>to be, or not to be</b>\" comes from <font color=\"white\">William Shakespeare's <i>Hamlet</i></font> (written about 1600), act three, scene one. It is one of the most famous quotations in <small>world literature</small> and the <u>best-known of this particular play</u>...<h6>And tables...</h6><table border=1><tr><td>Cell 1</td><td>Cell 2</td></tr></table></p>";
}

QTEST_APPLESS_MAIN(Pt_MLabel)
