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
    char appName[] = "./eternia";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
    MTheme::instance()->changeTheme("common");

    currentViewIndex = 0;
}

void Pt_MLabel::cleanupTestCase()
{
    delete app;
}

void Pt_MLabel::init()
{
    // get size dimensions for test
    QFETCH(QString, text);
    QFETCH(qint32, viewIndex);

    this->currentViewIndex = viewIndex;

    // create widget, set size
    m_subject = new MLabel(text);

    m_subject->setMaximumWidth(864);

    switch (this->currentViewIndex) {
    case View:
        this->currentView = new MLabelView(m_subject);
        break;
    }

    this->currentView->updateStyle();
    //this->currentView->styleUpdated();

    // There is no MLabel::setView() so this is the one from
    // MWidgetController, which is private, so need to be friends
    m_subject->setView(this->currentView);   // transfers ownership to controller

    // wait for the image loading
    usleep(1000000);
    QCoreApplication::processEvents();

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
            << this->currentViewIndex
            << "_"
            << m_subject->size().width()
            << "x"
            << m_subject->size().height()
            << ".png";
    if (!written.contains(kuva)) {
        pixmap->save(kuva, "png", -1);
        written.append(kuva);
    }
#endif

    delete m_subject;
    m_subject = 0;

    delete painter;
    painter = 0;
    delete pixmap;
    pixmap = 0;

}

void Pt_MLabel::paintPerformance()
{
    // actual benchmark
    QBENCHMARK {
        this->currentView->paint(painter, NULL);
    }
}

void Pt_MLabel::paintPerformance_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<qint32>("viewIndex");

    for (qint32 viewIndex = 0; viewIndex < NoViews; viewIndex++) {
        // typical icon sizes
        QTest::newRow("plaintext") << "Silence!" << viewIndex;
        QTest::newRow("richtext") << "<span>Silence! I <b>kill</b> you!</span>" << viewIndex;
        QTest::newRow("veryrich") << "<h5>Very rich text, multi line label</h5><p>The phrase \"<b>to be, or not to be</b>\" comes from <font color=\"white\">William Shakespeare's <i>Hamlet</i></font> (written about 1600), act three, scene one. It is one of the most famous quotations in <small>world literature</small> and the <u>best-known of this particular play</u>...<h6>And tables...</h6><table border=1><tr><td>Cell 1</td><td>Cell 2</td></tr></table></p>" << viewIndex;
    }
}

QTEST_APPLESS_MAIN(Pt_MLabel)
