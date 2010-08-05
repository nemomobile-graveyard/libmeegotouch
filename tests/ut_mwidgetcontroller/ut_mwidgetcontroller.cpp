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

#include <QGraphicsWidget>
#include "ut_mwidgetcontroller.h"
#include <MApplication>
#include <MWidgetView>
#include <MWidgetController>
#include <MWidgetModel>
#include <MTheme>
#include <QGraphicsLinearLayout>

int qQGraphicsWidgetSizeHintCallCount;
int modelSetupCount;
int updateDataCount;
bool Ut_MWidgetController::viewCreatesChildWidgets = false;
bool Ut_MWidgetController::viewSetsItselfActive = false;

class MockWidgetView : public MWidgetView
{
public:
    MockWidgetView(MWidgetController *controller, QSizeF sizeHint = QSizeF(-1, -1), bool createChildWidgets = false, bool setWidgetActive = false);
    void setActive(bool active);
    bool active;
protected:
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
private:
    QSizeF sh;
};

MockWidgetView::MockWidgetView(MWidgetController *controller, QSizeF sizeH, bool createChildWidgets, bool setWidgetActive) : MWidgetView(controller),
    active(false)
{
    sh = sizeH;
    if (createChildWidgets) {
        // Create child widget
        MWidget *child1 = new MWidget();

        // Create another child widget
        MWidget *child2 = new MWidget();

        // Create layout and assign children to the layout
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
        layout->addItem(child1);
        layout->addItem(child2);

        controller->setLayout(layout);
    }
    if (setWidgetActive) {
        controller->setActive(true);
    }
}

void MockWidgetView::setActive(bool a)
{
    active = a;
}

QSizeF MockWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    return sh;
}

class TestWidgetController : public MWidgetController
{
public:
    void setView(MWidgetView *view);
    void useModel();
protected:
    virtual void setupModel();
    virtual void updateData(const QList<const char *>& modifications);
        
};

void TestWidgetController::setView(MWidgetView *view)
{
    MWidgetController::setView(view);
}

// dummy method to make sure the model has been set up.
void TestWidgetController::useModel()
{
    QString name = model()->objectName();
    if (name.isEmpty())
        return;
}

void TestWidgetController::setupModel()
{
    modelSetupCount++;
}

void TestWidgetController::updateData(const QList<const char *>& modifications)
{
    Q_UNUSED(modifications);
    updateDataCount++;
}


// QGraphicsWidget stubs (used by MWidgetController)
QSizeF QGraphicsWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    qQGraphicsWidgetSizeHintCallCount++;
    return QSizeF();
}

void Ut_MWidgetController::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mwidgetcontroller" };
    app = new MApplication(argc, app_name);
}

void Ut_MWidgetController::cleanupTestCase()
{
    delete app;
}

void Ut_MWidgetController::init()
{
    modelSetupCount = 0;
    updateDataCount = 0;
    qQGraphicsWidgetSizeHintCallCount = 0;
    controller = new TestWidgetController;
    viewCreatesChildWidgets = false;
    viewSetsItselfActive = false;
}

void Ut_MWidgetController::cleanup()
{
    delete controller;
}

void Ut_MWidgetController::testBoundingRect()
{
    QVERIFY((controller->boundingRect() == QRectF()));
}

void Ut_MWidgetController::testSetGeometry_data()
{
    QTest::addColumn<QRectF>("rect");
    QTest::newRow("0") << QRectF(100, 100, 50, 50);
    QTest::newRow("1") << QRectF(10, 10, 5, 5);
}

void Ut_MWidgetController::testSetGeometry()
{
    QFETCH(QRectF, rect);
    controller->setGeometry(rect);
    QVERIFY(controller->geometry() == rect);
}

void Ut_MWidgetController::testSizeHint()
{
    // Test size hint with invalid view
    MWidgetView *view = new MockWidgetView(controller);
    //view->updateStyle();
    controller->setView(view);

    QCOMPARE(modelSetupCount, 0);

    qQGraphicsWidgetSizeHintCallCount = 0;
    controller->sizeHint(Qt::MinimumSize);
    controller->sizeHint(Qt::PreferredSize);
    controller->sizeHint(Qt::MaximumSize);
    QCOMPARE(qQGraphicsWidgetSizeHintCallCount, 3);

    QCOMPARE(modelSetupCount, 0);

    // Test size hint with valid view
    MWidgetView *view2 = new MockWidgetView(controller, QSizeF(12, 34));
    //view2->updateStyle();
    controller->setView(view2);
    QSizeF size(12 + view2->marginLeft() + view2->marginRight(), 34 + view2->marginTop() + view2->marginBottom());
    QCOMPARE(controller->sizeHint(Qt::MinimumSize), size);
    QCOMPARE(controller->sizeHint(Qt::PreferredSize), size);
    QCOMPARE(controller->sizeHint(Qt::MaximumSize), size);

    QCOMPARE(modelSetupCount, 0);
    controller->useModel();
    QCOMPARE(modelSetupCount, 1);
    controller->useModel();
    QCOMPARE(modelSetupCount, 1);
}

void Ut_MWidgetController::testUpdateData()
{
    controller->model()->setObjectName("Hello");
    QCOMPARE(updateDataCount, 1);
}

void Ut_MWidgetController::testActiveStateWhenViewIsSet()
{
    MockWidgetView *view = new MockWidgetView(controller);
    controller->setView(view);

    // Verify that controller is not in active state.
    QCOMPARE(controller->isActive(), false);
    QCOMPARE(view->active, false);

    // Verify that when controller is set to active state also the view is set to active state.
    controller->setActive(true);
    QCOMPARE(controller->isActive(), true);
    QCOMPARE(view->active, true);

    // Verify that when controller is set to inactive state also the view is set to inactive state.
    controller->setActive(false);
    QCOMPARE(controller->isActive(), false);
    QCOMPARE(view->active, false);
}

void Ut_MWidgetController::testActiveStateWhenViewIsUnset()
{
    // Verify that we can set controller active without a view
    controller->setActive(true);
    QCOMPARE(controller->isActive(), true);

    // Verify that when we set a view to an already active controller the view becomes active
    MockWidgetView *view = new MockWidgetView(controller);
    controller->setView(view);
    QCOMPARE(view->active, true);
}

QTEST_APPLESS_MAIN(Ut_MWidgetController);

