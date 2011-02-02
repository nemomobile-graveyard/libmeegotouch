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
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MButton>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsScene>
#include <MApplication>

#include "mclassfactory.h"
#include "pt_mlayout.h"

MApplication *app(NULL);

struct MySquareWidget : public QGraphicsWidget
{
    MySquareWidget() : sizeHintCount(0) {}
    virtual QSizeF  sizeHint ( Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const
    {
        if (which != Qt::PreferredSize)
            return QGraphicsWidget::sizeHint(which, constraint);
        sizeHintCount++;
        if (constraint.width() < 0)
            return QGraphicsWidget::sizeHint(which, constraint);
        return QSizeF(constraint.width(), constraint.width());
    }

    mutable int sizeHintCount;
};

void Pt_MLayout::initTestCase()
{
    static int argc = 1;
    char *argv[1] = { (char *) "./pt_mlayout" };
    app = new MApplication(argc, argv);

}

void Pt_MLayout::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Pt_MLayout::init()
{
    m_scene = new QGraphicsScene;
    m_form = new QGraphicsWidget;
    m_scene->addItem(m_form);

}

void Pt_MLayout::cleanup()
{
}
void Pt_MLayout::linearLayoutPerformance_data()
{
    QTest::addColumn<bool>("qtOnly");
    QTest::addColumn<bool>("testSizeHint");

    QTest::newRow("MLayout") << false << false;
    QTest::newRow("MLayout with sizeHint call") << false << true;
    QTest::newRow("QGraphicsLayout") << true << false;
    QTest::newRow("QGraphicsLayout with sizeHint call") << true << true;
}

void Pt_MLayout::linearLayoutPerformance()
{
    QFETCH(bool, qtOnly);
    QFETCH(bool, testSizeHint);

    QGraphicsLinearLayout *qlayout(NULL);
    MLayout *layout(NULL);
    MLinearLayoutPolicy *policy(NULL);
    if(qtOnly) {
        qlayout = new QGraphicsLinearLayout(Qt::Vertical, m_form);
    } else {
        layout = new MLayout(m_form);
        policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    }

    for(int i = 0; i < 100; i++) {
        MButton *button = new MButton;
        if (qtOnly)
            qlayout->addItem(button);
        else
            policy->addItem(button);
    }

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    QBENCHMARK {
        if (qtOnly) {
            qlayout->invalidate();
            qlayout->activate();
            if(testSizeHint)
                (void)qlayout->preferredSize();
        } else {
            layout->invalidate();
            layout->activate();
            if(testSizeHint)
                (void)layout->preferredSize();
        }
    }
}

void Pt_MLayout::heightForWidthPerformance_data()
{
    QTest::addColumn<bool>("qtOnly");
    QTest::addColumn<bool>("hfw");
    QTest::addColumn<bool>("nested");

    QTest::newRow("MLayout, hfw") << false << true << false;
    QTest::newRow("MLayout, hfw, nested") << false << true << true;
    QTest::newRow("MLayout, not hfw") << false << false << false;
    QTest::newRow("MLayout, not hfw, nested") << false << false << true;
    QTest::newRow("QGraphicsLayout, hfw") << true << true << false;
    QTest::newRow("QGraphicsLayout, hfw, nested") << true << true << true;
    QTest::newRow("QGraphicsLayout, not hfw") << true << false << false;
    QTest::newRow("QGraphicsLayout, not hfw, nested") << true << false << true;
}

void Pt_MLayout::heightForWidthPerformance()
{
    QFETCH(bool, qtOnly);
    QFETCH(bool, hfw);
    QFETCH(bool, nested);

    QGraphicsLinearLayout *outerlayout = NULL;
    if(nested) {
       outerlayout = new QGraphicsLinearLayout(m_form);
       for(int i = 0; i < 5; i++) {
           QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
           outerlayout->addItem(layout);
           outerlayout = layout;
       }
    }

    QGraphicsLinearLayout *qlayout = NULL;
    MLayout *layout = NULL;
    MLinearLayoutPolicy *policy = NULL;
    if(qtOnly) {
        qlayout = new QGraphicsLinearLayout(Qt::Vertical);
        if (nested)
            outerlayout->addItem(qlayout);
        else
            m_form->setLayout(qlayout);
    } else {
        layout = new MLayout;
        policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
        if (nested)
            outerlayout->addItem(layout);
        else
            m_form->setLayout(layout);
    }

    for(int i = 0; i < 1; i++) {
        MySquareWidget *widget = new MySquareWidget;
        QSizePolicy sizepolicy = widget->sizePolicy();
        sizepolicy.setHeightForWidth(hfw);
        widget->setSizePolicy(sizepolicy);
        if (qtOnly)
            qlayout->addItem(widget);
        else
            policy->addItem(widget);
    }

    while (MTheme::hasPendingRequests()) {
        usleep(10);
        QCoreApplication::processEvents();
    }

    QBENCHMARK {
        if (qtOnly) {
            qlayout->invalidate();
            QCoreApplication::processEvents();
            (void)qlayout->preferredSize();
        } else {
            layout->invalidate();
            QCoreApplication::processEvents();
            (void)layout->preferredSize();
        }
    }
}
void Pt_MLayout::gridLayoutPerformance_data()
{
    QTest::addColumn<bool>("qtOnly");
    QTest::addColumn<bool>("testSizeHint");

    QTest::newRow("MLayout") << false << false;
    QTest::newRow("MLayout with sizeHint call") << false << true;
    QTest::newRow("QGraphicsLayout") << true << false;
    QTest::newRow("QGraphicsLayout with sizeHint call") << true << true;
}

void Pt_MLayout::gridLayoutPerformance()
{
    QFETCH(bool, qtOnly);
    QFETCH(bool, testSizeHint);

    QGraphicsGridLayout *qlayout(NULL);
    MLayout *layout(NULL);
    MGridLayoutPolicy *policy(NULL);
    if(qtOnly) {
        qlayout = new QGraphicsGridLayout(m_form);
    } else {
        layout = new MLayout(m_form);
        policy = new MGridLayoutPolicy(layout);
    }

    for(int i = 0; i < 100; i++) {
        MButton *button = new MButton;
        if (qtOnly)
            qlayout->addItem(button,i/10,i%10);
        else
            policy->addItem(button,i/10,i%10);
    }

    while (MTheme::hasPendingRequests()) {
        usleep(10);
        QCoreApplication::processEvents();
    }

    QBENCHMARK {
        if (qtOnly) {
            qlayout->invalidate();
            qlayout->activate();
            if(testSizeHint)
                (void)qlayout->preferredSize();
        } else {
            layout->invalidate();
            layout->activate();
            if(testSizeHint)
                (void)layout->preferredSize();
        }
    }
}


QTEST_APPLESS_MAIN(Pt_MLayout)
