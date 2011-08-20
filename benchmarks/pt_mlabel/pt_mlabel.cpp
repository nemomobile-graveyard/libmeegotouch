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
    QFETCH(bool, textElide);

    // create widget, set size
    subject = new MLabel(text);
    subject->resize(864, 480);
    currentView = new MLabelView(subject);
    subject->setView(currentView);   // transfers ownership to controller
    subject->setTextElide(textElide);

    // wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

    // create pixmap paintdevice
    pixmap = new QPixmap(864, 480);
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
    data();
}

void Pt_MLabel::multiplePaintPerformance()
{
    QBENCHMARK {
        // To verify whether the internal usage of QStaticText improves the performance,
        // several painting operations without init() and cleanup() must be done
        for (int i = 0; i < 1000; ++i) {
            currentView->paint(painter, NULL);
        }
    }
}

void Pt_MLabel::multiplePaintPerformance_data()
{
    data();
}

void Pt_MLabel::elidePerformance_data()
{
    data();
}

void Pt_MLabel::elidePerformance()
{
    subject->setPreferredLineCount(2);
    QBENCHMARK {
        subject->resize(subject->sizeHint(Qt::PreferredSize, QSizeF(100,-1)));
        currentView->paint(painter, NULL);
        subject->resize(subject->sizeHint(Qt::PreferredSize, QSizeF(150,-1)));
        currentView->paint(painter, NULL);
    }
}

void Pt_MLabel::sizeHint()
{
    QFETCH(QSizeF, constraint);
    QBENCHMARK {
        subject->sizeHint(Qt::MinimumSize, constraint);
        subject->sizeHint(Qt::PreferredSize, constraint);
        subject->sizeHint(Qt::MaximumSize, constraint);
    }
}

void Pt_MLabel::sizeHint_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("textElide");
    QTest::addColumn<QSizeF>("constraint");

    const QString text("Longer text will get wrapped above several lines inside MLabel.");
    QTest::newRow("none") << text << false << QSizeF(-1, -1);
    QTest::newRow("50/50") << text << false << QSizeF(50, 50);
    QTest::newRow("100/100") << text << false << QSizeF(100, 100);

    QTest::newRow("none with elide") << text << true << QSizeF(-1, -1);
    QTest::newRow("50/50 with elide") << text << true << QSizeF(50, 50);
    QTest::newRow("100/100 with elide") << text << true << QSizeF(100, 100);

}

void Pt_MLabel::data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("textElide");

    QTest::newRow("plaintext without elide") << "Silence!" << false;
    QTest::newRow("multilength-strings without elide") << "Very very long text" + QLatin1Char(0x9c) + "Very long text" +  QLatin1Char(0x9c) + "Long text" +  QLatin1Char(0x9c) + "text" << false;
    QTest::newRow("richtext without elide") << "<span>Silence! I <b>kill</b> you!</span>" << false;
    QTest::newRow("veryrich without elide") << "<h5>Very rich text, multi line label</h5><p>The phrase \"<b>to be, or not to be</b>\" comes from <font color=\"white\">William Shakespeare's <i>Hamlet</i></font> (written about 1600), act three, scene one. It is one of the most famous quotations in <small>world literature</small> and the <u>best-known of this particular play</u>...<h6>And tables...</h6><table border=1><tr><td>Cell 1</td><td>Cell 2</td></tr></table></p>" << false;
    QTest::newRow("plaintext with elide") << "Silence!" << true;
    QTest::newRow("veryrich with elide") << "<h5>Very rich text, multi line label</h5><p>The phrase \"<b>to be, or not to be</b>\" comes from <font color=\"white\">William Shakespeare's <i>Hamlet</i></font> (written about 1600), act three, scene one. It is one of the most famous quotations in <small>world literature</small> and the <u>best-known of this particular play</u>...<h6>And tables...</h6><table border=1><tr><td>Cell 1</td><td>Cell 2</td></tr></table></p>" << true;
    QString string = "All work and no play makes Jack a dull boy, ";
    for(int i = 0; i < 5; i++)
        string = string + string;
    QTest::newRow("very long plaintext without elide") << string << false;
    QTest::newRow("very long richtext without elide") << "<qt>" + string << false;
    QTest::newRow("very long plaintext with elide") << string << true;
    QTest::newRow("very long richtext with elide") << "<qt>" + string << true;
}

QTEST_APPLESS_MAIN(Pt_MLabel)
