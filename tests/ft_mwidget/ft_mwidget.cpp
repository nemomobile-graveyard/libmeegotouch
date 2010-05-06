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

#include "ft_mwidget.h"
#include <QtTest>
#include <QImage>
#include "mapplication.h"
#include "mslider.h"
#include "mbutton.h"
#include "mtheme.h"
#include "mtextedit.h"
#include "mnavigationbar.h"
#include "mtime.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include "mimage.h"
#include "mimagelabel.h"
#include "mimagewidget.h"
#include "mlabel.h"
#include "mpannablewidget.h"
#include "mpositionindicator.h"
#include "mtoolbar.h"
#include "mpannableviewport.h"
#include <Qt>


//#define DEBUG

Ft_MWidget::Ft_MWidget()
{

}

void Ft_MWidget::paintWidgetWithForcedResize()
{
    createWidgetsUnderTest();
    int numPass = 0;

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        widgetsUnderTest.at(i)->resize(92, 92);
        QSizeF s = QSizeF(92, 92);
        QSizeF ps = paintedSize(widgetsUnderTest.at(i));
        QSizeF limit = 0.95 * s;

        if (ps.height() <= s.height()
                && ps.width() <= s.width()
                && ps.height() >= limit.height()
                && ps.width() >= limit.height()) {

            ++numPass;

            qDebug() << "## paintWidgetWithForcedResize ok ##" << widgetsUnderTest.at(i)->metaObject()->className() << ps.width() <<
                     ps.height();
        } else {
            qDebug() << "## paintWidgetWithForcedResize fail ##" << widgetsUnderTest.at(i)->metaObject()->className() << ps.width() <<
                     ps.height();
        }

    }

    qDebug() << "---- paintWidgetWithForcedResize, numPass" << numPass << "/" << widgetsUnderTest.size();
    QVERIFY(numPass == widgetsUnderTest.size());
}


void Ft_MWidget::constructAndDeleteWidgets()
{
    createWidgetsUnderTest();

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        delete widgetsUnderTest.at(i);
    }
}

void Ft_MWidget::paintWidgetWithoutForcedResize()
{
    createWidgetsUnderTest();
    int numPass = 0;

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        QSizeF s = widgetsUnderTest.at(i)->size();
        if (s.height() <= 0 && s.width() <= 0) {
            widgetsUnderTest.at(i)->resize(83, 83);
            s = QSizeF(83, 83);
        }

        QSizeF ps = paintedSize(widgetsUnderTest.at(i));
        QSizeF limit = 0.95 * s;

        if (ps.height() <= s.height()
                && ps.width() <= s.width()
                && ps.height() >= limit.height()
                && ps.width() >= limit.height()) {

            ++numPass;

            qDebug() << "## paintWidgetWithoutForcedResize ok" << widgetsUnderTest.at(i)->metaObject()->className() << ps.width() <<
                     ps.height();
        } else {
            qDebug() << "## paintWidgetWithoutForcedResize fail " << widgetsUnderTest.at(i)->metaObject()->className() << ps.width() <<
                     ps.height();
        }
    }

    qDebug() << "---- paintWidgetWithoutForcedResize, numPass" << numPass << "/" << widgetsUnderTest.size();
    QVERIFY(numPass == widgetsUnderTest.size());
}

void Ft_MWidget::initialSize()
{
    createWidgetsUnderTest();
    int numPass = 0;

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        QSizeF s = widgetsUnderTest.at(i)->size();

        if (s.height() < 10 || s.width() < 10) {
            qDebug() << "## initialSize fail" << widgetsUnderTest.at(i)->metaObject()->className() << " initial size " << s;
        } else {
            qDebug() << "## initialSize ok" << widgetsUnderTest.at(i)->metaObject()->className() << " initial size " << s;
            ++numPass;
        }
    }

    qDebug() << "---- initialSize, numPass" << numPass << "/" << widgetsUnderTest.size();
    QVERIFY(numPass == widgetsUnderTest.size());
}

void Ft_MWidget::resizeAndSizeLookup()
{
    createWidgetsUnderTest();

    int numPass = 0;
    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        widgetsUnderTest.at(i)->resize(99, 99);
        QSizeF s = widgetsUnderTest.at(i)->size();
        if (s.height() != 99 || s.width() != 99) {
            qDebug() << " ## resizeAndSizeLookup fail " << widgetsUnderTest.at(i)->metaObject()->className() << " size " << s;
        } else {
            qDebug() << " ## resizeAndSizeLookup ok " << widgetsUnderTest.at(i)->metaObject()->className() << " size " << s;
            ++numPass;
        }
    }

    qDebug() << "---- resizeWidget, numPass" << numPass << "/" << widgetsUnderTest.size();
    QVERIFY(numPass == widgetsUnderTest.size());
}


QSizeF Ft_MWidget::paintedSize(QGraphicsWidget *widget)
{
    QPixmap *p = new QPixmap(500, 500);
    p->fill(QColor(0, 0, 0, 0));

    QPixmap *orig = new QPixmap(500, 500);
    orig->fill(QColor(0, 0, 0, 0));

    QPainter *painter = new QPainter(p);
    QStyleOptionGraphicsItem style;

    style.exposedRect = QRect(0, 0, 500, 500);
    widget->paint(painter, &style, 0);

    QImage origi = orig->toImage();
    QImage pi = p->toImage();

    int width = 0;
    int height = 0;

    for (int y = 0; y < 500; y++) {
        for (int x = 0; x < 500; x++) {
            if (pi.pixel(x, y) != origi.pixel(x, y) && x + 1 > width) {
                width = x + 1;
            }
        }
    }

    for (int x = 0; x < 500; x++) {
        for (int y = 0; y < 500; y++) {
            if (pi.pixel(x, y) != origi.pixel(x, y) && y + 1 > height) {
                height = y + 1;
            }
        }
    }

#ifdef DEBUG
    QString className(widget->metaObject()->className());
    p->save(className + ".png", "PNG");
#endif

    QSizeF retval(width, height);
    return retval;
}

void Ft_MWidget::initTestCase()
{
    int i = 0;
    app = new MApplication(i, (char **)0);
    MTheme::loadCSS("conf_ft_mwidget.css");

}

void Ft_MWidget::createWidgetsUnderTest()
{
    widgetsUnderTest.clear();

    //TODO: initialize widgets correctly here

    QPushButton *pb = new QPushButton;
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    proxy->setWidget(pb);

    widgetsUnderTest.append(proxy);

    MButton *b = new MButton();
    b->setObjectName("ft_test_button");
    widgetsUnderTest.append(b);

    MTextEdit *t = new MTextEdit;
    widgetsUnderTest.append(t);

    MTime *time = new MTime;
    widgetsUnderTest.append(time);

    widgetsUnderTest.append(new MToolbar);
    widgetsUnderTest.append(new MSlider);
    widgetsUnderTest.append(new MPositionIndicator(Qt::Horizontal));

    MButton *b3 = new MButton();
    b3->setObjectName("ft_test_button");

    widgetsUnderTest.append(new MNavigationBar);
    widgetsUnderTest.append(new MLabel);

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        widgetsUnderTest.at(i)->setMinimumSize(0, 0);
        widgetsUnderTest.at(i)->setMaximumSize(999, 999);
    }
    return;

    //segfaulting widgets below
    MPannableViewport *vp = new MPannableViewport(Qt::Vertical);
    vp->setWidget(vp);
    widgetsUnderTest.append(vp);

    widgetsUnderTest.append(new MImageWidget);
    widgetsUnderTest.append(new MImageLabel);





}


QTEST_APPLESS_MAIN(Ft_MWidget)


