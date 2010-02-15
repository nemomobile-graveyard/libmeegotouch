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

#include "ft_duiwidget.h"
#include <QtTest>
#include <QImage>
#include "duiapplication.h"
#include "duislider.h"
#include "duibutton.h"
#include "duitheme.h"
#include "duitextedit.h"
#include "duinavigationbar.h"
#include "duitime.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include "duiimage.h"
#include "duiimagelabel.h"
#include "duiimagewidget.h"
#include "duilabel.h"
#include "duipannablewidget.h"
#include "duipositionindicator.h"
#include "duitoolbar.h"
#include "duipannableviewport.h"
#include <Qt>


//#define DEBUG

Ft_DuiWidget::Ft_DuiWidget()
{

}

void Ft_DuiWidget::paintWidgetWithForcedResize()
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


void Ft_DuiWidget::constructAndDeleteWidgets()
{
    createWidgetsUnderTest();

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        delete widgetsUnderTest.at(i);
    }
}

void Ft_DuiWidget::paintWidgetWithoutForcedResize()
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

void Ft_DuiWidget::initialSize()
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

void Ft_DuiWidget::resizeAndSizeLookup()
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


QSizeF Ft_DuiWidget::paintedSize(QGraphicsWidget *widget)
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

void Ft_DuiWidget::initTestCase()
{
    int i = 0;
    app = new DuiApplication(i, (char **)0);
    DuiTheme::loadCSS("conf_ft_duiwidget.css");

}

void Ft_DuiWidget::createWidgetsUnderTest()
{
    widgetsUnderTest.clear();

    //TODO: initialize widgets correctly here

    QPushButton *pb = new QPushButton;
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    proxy->setWidget(pb);

    widgetsUnderTest.append(proxy);

    DuiButton *b = new DuiButton();
    b->setObjectName("ft_test_button");
    widgetsUnderTest.append(b);

    DuiTextEdit *t = new DuiTextEdit;
    widgetsUnderTest.append(t);

    DuiTime *time = new DuiTime;
    widgetsUnderTest.append(time);

    widgetsUnderTest.append(new DuiToolbar);
    widgetsUnderTest.append(new DuiSlider);
    widgetsUnderTest.append(new DuiPositionIndicator(Qt::Horizontal));

    DuiButton *b3 = new DuiButton();
    b3->setObjectName("ft_test_button");

    widgetsUnderTest.append(new DuiNavigationBar);
    widgetsUnderTest.append(new DuiLabel);

    for (int i = 0; i < widgetsUnderTest.size(); ++i) {
        widgetsUnderTest.at(i)->setMinimumSize(0, 0);
        widgetsUnderTest.at(i)->setMaximumSize(999, 999);
    }
    return;

    //segfaulting widgets below
    DuiPannableViewport *vp = new DuiPannableViewport(Qt::Vertical);
    vp->setWidget(vp);
    widgetsUnderTest.append(vp);

    widgetsUnderTest.append(new DuiImageWidget);
    widgetsUnderTest.append(new DuiImageLabel);





}


QTEST_APPLESS_MAIN(Ft_DuiWidget)


