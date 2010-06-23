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
#include "imthemedaemon.h"
#include <MTheme>
#include "mtheme_p.h"
#include <MButton>
#include <MButtonView>
#include <MButtonIconView>
#include <MApplication>

#include "mclassfactory.h"
#include "pt_mbutton.h"

MApplication *app(NULL);
const int W = 250;
const int H = 100;


void Pt_MButton::initTestCase()
{
    static int argc = 1;
    char *argv[1] = { (char *) "./pt_mbutton" };
    app = new MApplication(argc, argv);

    //warmup
    QPixmap pixmap(W, H);
    QPainter painter(&pixmap);
    MButton warmupButton;
    warmupButton.setText("text");
    warmupButton.setIconID("icon-m-framework-home");
    warmupButton.setToggledIconID("icon-m-framework-close");
    warmupButton.setGeometry(QRectF(0, 0, W, H));
    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    warmupButton.paint(&painter, NULL);
}

void Pt_MButton::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Pt_MButton::init()
{
    m_subject = new MButton();
}

void Pt_MButton::cleanup()
{
    delete m_subject;
    m_subject = NULL;

}
void Pt_MButton::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("view");
    QTest::addColumn<QString>("icon1");
    QTest::addColumn<QString>("icon2");
    QTest::addColumn<QString>("text");

    QTest::newRow("250x100 MButtonView") << W << H << "MButtonView" << "" << "" << "";
    QTest::newRow("250x100 MButtonView Text") << W << H << "MButtonView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonView Icon") << W << H << "MButtonView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("250x100 MButtonView Icon Text") << W << H << "MButtonView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonView Icon ToggledIcon") << W << H << "MButtonView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("250x100 MButtonView Icon ToggledIcon Text") << W << H << "MButtonView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";

    QTest::newRow("250x100 MButtonDefaultView") << W << H << "MButtonDefaultView" << "" << "" << "";
    QTest::newRow("250x100 MButtonDefaultView Text") << W << H << "MButtonDefaultView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonDefaultView Icon") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("250x100 MButtonDefaultView Icon Text") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonDefaultView Icon ToggledIcon") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("250x100 MButtonDefaultView Icon ToggledIcon Text") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";

    QTest::newRow("250x100 MButtonIconView") << W << H << "MButtonIconView" << "" << "" << "";
    QTest::newRow("250x100 MButtonIconView Text") << W << H << "MButtonIconView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonIconView Icon") << W << H << "MButtonIconView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("250x100 MButtonIconView Icon Text") << W << H << "MButtonIconView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonIconView Icon ToggledIcon") << W << H << "MButtonIconView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("250x100 MButtonIconView Icon ToggledIcon Text") << W << H << "MButtonIconView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";
}

void Pt_MButton::paintPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, width, height), QColor(0, 0, 0));

    MWidgetView *buttonView = MClassFactory::instance()->createView(view.toStdString().c_str(), m_subject);
    m_subject->setView(buttonView);

    m_subject->setGeometry(QRectF(0, 0, width, height));
    if (!text.isEmpty()) {
        m_subject->setText(text);
        m_subject->setTextVisible(true);
    } else
        m_subject->setTextVisible(false);

    if (!icon1.isEmpty()) {
        m_subject->setIconID(icon1);
        m_subject->setIconVisible(true);
    } else
        m_subject->setIconVisible(false);

    if (!icon2.isEmpty()) {
        m_subject->setToggledIconID(icon2);
        m_subject->setIconVisible(true);
    }
    //else
    //    button->setIconVisible(false);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    // actual benchmark
    QBENCHMARK {
        painter.save();
        m_subject->paint(&painter, NULL);
        painter.restore();
    }

    // save a shot (for debugging)
//#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << view
            << "_"
            << "\"" << icon1 << "\""
            << "_"
            << "\"" << icon2 << "\""
            << "_"
            << "\"" << text << "\""
            << "_"
            << width
            << "x"
            << height
            << ".png";
    pixmap.save(kuva, "png", -1);
#endif
}

void Pt_MButton::toggledPaintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("view");
    QTest::addColumn<QString>("icon1");
    QTest::addColumn<QString>("icon2");
    QTest::addColumn<QString>("text");

    QTest::newRow("250x100 MButtonView") << W << H << "MButtonView" << "" << "" << "";
    QTest::newRow("250x100 MButtonView Text") << W << H << "MButtonView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonView Icon") << W << H << "MButtonView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("250x100 MButtonView Icon Text") << W << H << "MButtonView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonView Icon ToggledIcon") << W << H << "MButtonView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("250x100 MButtonView Icon ToggledIcon Text") << W << H << "MButtonView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";

    QTest::newRow("250x100 MButtonDefaultView") << W << H << "MButtonDefaultView" << "" << "" << "";
    QTest::newRow("250x100 MButtonDefaultView Text") << W << H << "MButtonDefaultView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonDefaultView Icon") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("250x100 MButtonDefaultView Icon Text") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonDefaultView Icon ToggledIcon") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("250x100 MButtonDefaultView Icon ToggledIcon Text") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";

    QTest::newRow("250x100 MButtonIconView") << W << H << "MButtonIconView" << "" << "" << "";
    QTest::newRow("250x100 MButtonIconView Text") << W << H << "MButtonIconView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonIconView Icon") << W << H << "MButtonIconView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("250x100 MButtonIconView Icon Text") << W << H << "MButtonIconView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("250x100 MButtonIconView Icon ToggledIcon") << W << H << "MButtonIconView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("250x100 MButtonIconView Icon ToggledIcon Text") << W << H << "MButtonIconView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";
}

void Pt_MButton::toggledPaintPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, width, height), QColor(0, 0, 0));

    MWidgetView *buttonView = MClassFactory::instance()->createView(view.toStdString().c_str(), m_subject);
    m_subject->setView(buttonView);

    m_subject->setGeometry(QRectF(0, 0, width, height));
    if (!text.isEmpty()) {
        m_subject->setText(text);
        m_subject->setTextVisible(true);
    } else
        m_subject->setTextVisible(false);

    if (!icon1.isEmpty()) {
        m_subject->setIconID(icon1);
        m_subject->setIconVisible(true);
    } else
        m_subject->setIconVisible(false);

    if (!icon2.isEmpty()) {
        m_subject->setToggledIconID(icon2);
        m_subject->setIconVisible(true);
    }
    //else
    //    button->setIconVisible(false);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_subject->setDown(true);
    m_subject->setDown(false);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    // actual benchmark
    QBENCHMARK {
        painter.save();
        m_subject->paint(&painter, NULL);
        painter.restore();
    }

    // save a shot (for debugging)
//#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << view
            << "_toggled"
            << "\"" << icon1 << "\""
            << "_"
            << "\"" << icon2 << "\""
            << "_"
            << "\"" << text << "\""
            << "_"
            << width
            << "x"
            << height
            << ".png";
    pixmap.save(kuva, "png", -1);
#endif
}

QTEST_APPLESS_MAIN(Pt_MButton)
