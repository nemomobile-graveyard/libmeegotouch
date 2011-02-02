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

#include "ut_mnavigationbarview.h"

#include "mnavigationbarview_p.h"

#include <QGraphicsLayout>
#include <MLinearLayoutPolicy>
#include <mnavigationbarstyle.h>
#include <MToolBar>
#include <MLayout>

Q_DECLARE_METATYPE(MNavigationBarModel::EscapeButtonModeEnum)

void Ut_MNavigationBarView::init()
{
    controller = new MNavigationBar();
    subject = new MNavigationBarView(controller);
    controller->setView(subject);
    model = controller->model();
}

void Ut_MNavigationBarView::cleanup()
{
    delete controller;
    controller = 0;
}

void Ut_MNavigationBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mnavigationbarview" };
    app = new MApplication(argc, app_name);


}

void Ut_MNavigationBarView::cleanupTestCase()
{
    delete app;
    app = 0;
}

void Ut_MNavigationBarView::testCustomContentOwnershipTransfer()
{
    // OBS: The ownership of customContent is transfered by
    // MNavigationBarView code.
    // That's why this test is here and not in Ut_MNavigationBar

    QGraphicsWidget *customContent = new QGraphicsWidget;

    controller->setCustomContent(customContent);

    QVERIFY(controller->isAncestorOf(customContent));

    controller->setCustomContent(0);

    QVERIFY(customContent->scene() == 0);
    QVERIFY(customContent->parent() == 0);

    // Clean up
    delete customContent;
}

void Ut_MNavigationBarView::testCustomContentPolicySelection()
{
    QGraphicsWidget *customContent = new QGraphicsWidget;

    QCOMPARE(subject->d_func()->customContentPolicy->isActive(), false);

    controller->setCustomContent(customContent);

    QCOMPARE(subject->d_func()->customContentPolicy->isActive(), true);

    controller->setCustomContent(0);

    QCOMPARE(subject->d_func()->customContentPolicy->isActive(), false);

    // Clean up
    delete customContent;
}

void Ut_MNavigationBarView::testIsEmptyProperty_data()
{
    QTest::addColumn<bool>("hasTitle");
    QTest::addColumn<bool>("hasCloseButton");
    QTest::addColumn<bool>("hasToolBar");
    QTest::addColumn<bool>("toolBarIsEmpty");
    QTest::addColumn<MNavigationBarModel::EscapeButtonModeEnum>("escapeButtonMode");
    QTest::addColumn<bool>("hasCustomContent");
    QTest::addColumn<bool>("isEmptyExpectedValue");

    QTest::newRow("not empty when with custom content")
            << false // hasTitle
            << false // hasCloseButton
            << false // hasToolBar
            << true // toolBarIsEmpty
            << MNavigationBarModel::EscapeButtonClose // escapeButtonMode
            << true // hasCustomContent
            << false /* isEmptyExpectedValue */;

    QTest::newRow("not empty when it has only a title")
            << true // hasTitle
            << false // hasCloseButton
            << false // hasToolBar
            << true // toolBarIsEmpty
            << MNavigationBarModel::EscapeButtonClose // escapeButtonMode
            << false // hasCustomContent
            << false /* isEmptyExpectedValue */;

    QTest::newRow("empty when there's not even a close button")
            << false // hasTitle
            << false // hasCloseButton
            << false // hasToolBar
            << true // toolBarIsEmpty
            << MNavigationBarModel::EscapeButtonClose // escapeButtonMode
            << false // hasCustomContent
            << true /* isEmptyExpectedValue */;

    QTest::newRow("not empty when there's no close button but we are in back mode")
            << false // hasTitle
            << false // hasCloseButton
            << false // hasToolBar
            << true // toolBarIsEmpty
            << MNavigationBarModel::EscapeButtonBack // escapeButtonMode
            << false // hasCustomContent
            << false /* isEmptyExpectedValue */;

    QTest::newRow("not empty when there's only a populated tool bar")
            << false // hasTitle
            << false // hasCloseButton
            << true // hasToolBar
            << false // toolBarIsEmpty
            << MNavigationBarModel::EscapeButtonClose // escapeButtonMode
            << false // hasCustomContent
            << true /* isEmptyExpectedValue */;

    QTest::newRow("empty when there's only an empty tool bar")
            << false // hasTitle
            << false // hasCloseButton
            << true // hasToolBar
            << true // toolBarIsEmpty
            << MNavigationBarModel::EscapeButtonClose // escapeButtonMode
            << false // hasCustomContent
            << true /* isEmptyExpectedValue */;
}

void Ut_MNavigationBarView::testIsEmptyProperty()
{
    MToolBar *toolBar = 0;
    QFETCH(bool, hasTitle);
    QFETCH(bool, hasCloseButton);
    QFETCH(bool, hasToolBar);
    QFETCH(bool, toolBarIsEmpty);
    QFETCH(MNavigationBarModel::EscapeButtonModeEnum, escapeButtonMode);
    QFETCH(bool, hasCustomContent);
    QFETCH(bool, isEmptyExpectedValue);

    MNavigationBarStyle *style = (MNavigationBarStyle *)subject->style().operator->();

    style->setHasTitle(hasTitle);
    style->setHasCloseButton(hasCloseButton);
    subject->applyStyle();

    if (hasToolBar) {
        toolBar = new MToolBar;
        toolBar->setProperty("isEmpty", toolBarIsEmpty);
        model->setToolBar(toolBar);
    }

    model->setEscapeButtonMode(escapeButtonMode);

    if (hasCustomContent) {
        model->setCustomContent(new QGraphicsWidget);
    } else {
        model->setCustomContent(0);
    }

    QCOMPARE(controller->property("isEmpty").toBool(), isEmptyExpectedValue);

    // clean up
    if (toolBar) {
        model->setToolBar(0);
        delete toolBar;
    }
}

void Ut_MNavigationBarView::testDockedToolBarChangingItsIsEmptyProperty()
{
    MToolBar *toolBar = new MToolBar;
    MNavigationBarStyle *style = (MNavigationBarStyle *)subject->style().operator->();

    style->setHasTitle(false);
    style->setHasCloseButton(false);
    subject->applyStyle();

    model->setToolBar(toolBar);
    model->setEscapeButtonMode(MNavigationBarModel::EscapeButtonClose);

    // Since navigation bar is populated only by its docked toolbar, its isEmpty
    // property will follow toolbar's one.

    toolBar->setProperty("isEmpty", true);
    QCOMPARE(controller->property("isEmpty").toBool(), true);

    toolBar->setProperty("isEmpty", false);
    QCOMPARE(controller->property("isEmpty").toBool(), false);

    toolBar->setProperty("isEmpty", true);
    QCOMPARE(controller->property("isEmpty").toBool(), true);

    model->setToolBar(0);
    toolBar->setProperty("isEmpty", false);

    // tool bar is no longer docked, thus navigation bar should no longer follow it
    QCOMPARE(controller->property("isEmpty").toBool(), true);

    // clean up
    delete toolBar;
}

void Ut_MNavigationBarView::testToolBarViewTypeChange()
{
    MToolBar *toolBar = new MToolBar;

    model->setToolBar(toolBar);
    model->setEscapeButtonVisible(false);
    model->setArrowIconVisible(false);

    QCOMPARE(toolBar->property("widgetAlignment").toInt(), (int)Qt::AlignJustify);
    QCOMPARE(subject->d_func()->layout->policy(), subject->d_func()->toolbarPolicy);

    toolBar->setViewType(MToolBar::tabType);

    QCOMPARE(toolBar->property("widgetAlignment").toInt(), (int)Qt::AlignJustify);
    QCOMPARE(subject->d_func()->layout->policy(), subject->d_func()->toolbarPolicy);

    model->setToolBar(0);
    QCOMPARE(toolBar->property("widgetAlignment").isValid(), false);
    QCOMPARE(subject->d_func()->layout->policy(), subject->d_func()->toolbarPolicy);

    delete toolBar;
}

QTEST_APPLESS_MAIN(Ut_MNavigationBarView)
