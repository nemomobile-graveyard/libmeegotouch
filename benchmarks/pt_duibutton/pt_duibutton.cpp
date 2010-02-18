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

#include <QObject>
#include "iduithemedaemon.h"
#include <DuiTheme>
#include "duitheme_p.h"
#include <DuiButton>
#include <DuiButtonView>
#include <DuiButtonIconView>
#include <DuiApplication>

#include "duiclassfactory.h"
#include "pt_duibutton.h"

DuiApplication *app(NULL);
const int W = 250;
const int H = 100;


void Pt_DuiButton::initTestCase()
{
    static int argc = 1;
    char *argv[1] = { (char *) "./pt_duibutton" };
    app = new DuiApplication(argc, argv);

    //warmup
    QPixmap pixmap(W, H);
    QPainter painter(&pixmap);
    DuiButton warmupButton;
    warmupButton.setText("text");
    warmupButton.setIconID("Icon-home");
    warmupButton.setToggledIconID("Icon-close");
    warmupButton.setGeometry(QRectF(0, 0, W, H));
    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    warmupButton.paint(&painter, NULL);
}

void Pt_DuiButton::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Pt_DuiButton::init()
{
    m_subject = new DuiButton();
}

void Pt_DuiButton::cleanup()
{
    delete m_subject;
    m_subject = NULL;

}
void Pt_DuiButton::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("view");
    QTest::addColumn<QString>("icon1");
    QTest::addColumn<QString>("icon2");
    QTest::addColumn<QString>("text");

    QTest::newRow("250x100 DuiButtonView") << W << H << "DuiButtonView" << "" << "" << "";
    QTest::newRow("250x100 DuiButtonView Text") << W << H << "DuiButtonView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonView Icon") << W << H << "DuiButtonView" << "Icon-home" << "" << "";
    QTest::newRow("250x100 DuiButtonView Icon Text") << W << H << "DuiButtonView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonView Icon ToggledIcon") << W << H << "DuiButtonView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("250x100 DuiButtonView Icon ToggledIcon Text") << W << H << "DuiButtonView" << "Icon-home" << "Icon-close" << "ButtonText";

    QTest::newRow("250x100 DuiButtonDefaultView") << W << H << "DuiButtonDefaultView" << "" << "" << "";
    QTest::newRow("250x100 DuiButtonDefaultView Text") << W << H << "DuiButtonDefaultView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonDefaultView Icon") << W << H << "DuiButtonDefaultView" << "Icon-home" << "" << "";
    QTest::newRow("250x100 DuiButtonDefaultView Icon Text") << W << H << "DuiButtonDefaultView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonDefaultView Icon ToggledIcon") << W << H << "DuiButtonDefaultView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("250x100 DuiButtonDefaultView Icon ToggledIcon Text") << W << H << "DuiButtonDefaultView" << "Icon-home" << "Icon-close" << "ButtonText";

    QTest::newRow("250x100 DuiButtonIconView") << W << H << "DuiButtonIconView" << "" << "" << "";
    QTest::newRow("250x100 DuiButtonIconView Text") << W << H << "DuiButtonIconView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonIconView Icon") << W << H << "DuiButtonIconView" << "Icon-home" << "" << "";
    QTest::newRow("250x100 DuiButtonIconView Icon Text") << W << H << "DuiButtonIconView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonIconView Icon ToggledIcon") << W << H << "DuiButtonIconView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("250x100 DuiButtonIconView Icon ToggledIcon Text") << W << H << "DuiButtonIconView" << "Icon-home" << "Icon-close" << "ButtonText";
}

void Pt_DuiButton::paintPerformance()
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

    DuiWidgetView *buttonView = DuiClassFactory::instance()->createView(view.toStdString().c_str(), m_subject);
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

    while (DuiTheme::hasPendingRequests()) {
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

void Pt_DuiButton::toggledPaintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("view");
    QTest::addColumn<QString>("icon1");
    QTest::addColumn<QString>("icon2");
    QTest::addColumn<QString>("text");

    QTest::newRow("250x100 DuiButtonView") << W << H << "DuiButtonView" << "" << "" << "";
    QTest::newRow("250x100 DuiButtonView Text") << W << H << "DuiButtonView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonView Icon") << W << H << "DuiButtonView" << "Icon-home" << "" << "";
    QTest::newRow("250x100 DuiButtonView Icon Text") << W << H << "DuiButtonView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonView Icon ToggledIcon") << W << H << "DuiButtonView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("250x100 DuiButtonView Icon ToggledIcon Text") << W << H << "DuiButtonView" << "Icon-home" << "Icon-close" << "ButtonText";

    QTest::newRow("250x100 DuiButtonDefaultView") << W << H << "DuiButtonDefaultView" << "" << "" << "";
    QTest::newRow("250x100 DuiButtonDefaultView Text") << W << H << "DuiButtonDefaultView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonDefaultView Icon") << W << H << "DuiButtonDefaultView" << "Icon-home" << "" << "";
    QTest::newRow("250x100 DuiButtonDefaultView Icon Text") << W << H << "DuiButtonDefaultView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonDefaultView Icon ToggledIcon") << W << H << "DuiButtonDefaultView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("250x100 DuiButtonDefaultView Icon ToggledIcon Text") << W << H << "DuiButtonDefaultView" << "Icon-home" << "Icon-close" << "ButtonText";

    QTest::newRow("250x100 DuiButtonIconView") << W << H << "DuiButtonIconView" << "" << "" << "";
    QTest::newRow("250x100 DuiButtonIconView Text") << W << H << "DuiButtonIconView" << "" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonIconView Icon") << W << H << "DuiButtonIconView" << "Icon-home" << "" << "";
    QTest::newRow("250x100 DuiButtonIconView Icon Text") << W << H << "DuiButtonIconView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("250x100 DuiButtonIconView Icon ToggledIcon") << W << H << "DuiButtonIconView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("250x100 DuiButtonIconView Icon ToggledIcon Text") << W << H << "DuiButtonIconView" << "Icon-home" << "Icon-close" << "ButtonText";
}

void Pt_DuiButton::toggledPaintPerformance()
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

    DuiWidgetView *buttonView = DuiClassFactory::instance()->createView(view.toStdString().c_str(), m_subject);
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

    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_subject->setDown(true);
    m_subject->setDown(false);
    usleep(10000);
    QCoreApplication::processEvents();
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

QTEST_APPLESS_MAIN(Pt_DuiButton)
