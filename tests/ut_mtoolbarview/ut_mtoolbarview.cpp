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

#include <MApplication>
#include <MApplicationWindow>
#include <MAction>
#include <MButton>
#include <MSlider>
#include <MToolBar>
#include <MToolBarView>
#include <MToolBarTabView>
#include <MTextEdit>
#include <MWidgetAction>
#include <MScene>
#include <QGraphicsLinearLayout>
#include <QPointer>

#include "ut_mtoolbarview.h"

MApplication *app;
MApplicationWindow *appWin;

void Ut_MToolBarView::initTestCase()

{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MToolBarView" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;
}

void Ut_MToolBarView::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MToolBarView::init()
{
    appWin->setOrientationAngle(M::Angle0);
    m_toolbar = new MToolBar();
    QVERIFY(m_toolbar != 0);
    appWin->scene()->addItem(m_toolbar);

    m_toolbarview = new MToolBarView(m_toolbar);
    QVERIFY(m_toolbarview != 0);

    m_toolbar->setView(m_toolbarview);
    //m_toolbarview->updateStyle();
}

void Ut_MToolBarView::cleanup()
{
    delete m_toolbar;
    m_toolbar = 0;
}

void Ut_MToolBarView::testDeleteAddAction()
{
    m_toolbar->clearActions();

    MWidget *parentWidget = new MWidget();
    m_toolbar->addAction(createTextEditAction(parentWidget));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete parentWidget;

    // toolbar should not crash as action has been deleted
    m_toolbar->addAction(new MAction("TEXTONLY", m_toolbar));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(m_toolbar->actions().count() == 1);
}

void Ut_MToolBarView::testTabView()
{
    m_toolbar->clearActions();

    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);

    m_toolbar->setView(tabView);
}

void Ut_MToolBarView::testTabActionExclusivity()
{
    m_toolbar->clearActions();
    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);
    m_toolbar->setView(tabView);

    MAction *action0 = new MAction("action0", m_toolbar);
    action0->setLocation(MAction::ToolBarLandscapeLocation);
    action0->setCheckable(true);
    action0->setChecked(true);
    m_toolbar->addAction(action0);
    MAction* action = qobject_cast<MAction *>(m_toolbar->actions().at(0));
    QVERIFY(action);
    QVERIFY(action == action0);
    MButton* button0 = dynamic_cast<MButton *>(tabView->getWidget(action));
    QVERIFY(button0);
    QVERIFY(button0->isChecked());

    QVERIFY(action0->isChecked() == true);

    MAction *action1 = new MAction("action1", m_toolbar);
    action1->setLocation(MAction::ToolBarPortraitLocation);
    action1->setCheckable(true);
    m_toolbar->addAction(action1);
    action = qobject_cast<MAction *>(m_toolbar->actions().at(1));
    QVERIFY(action);
    QVERIFY(action == action1);

    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);

    MButton* button1 = dynamic_cast<MButton *>(tabView->getWidget(action));
    QVERIFY(button1);
    QVERIFY(button0->isChecked());
    QVERIFY(!button1->isChecked());

    MAction *action2 = new MAction("action2", m_toolbar);
    action2->setLocation(MAction::ToolBarLocation);
    action2->setCheckable(true);
    m_toolbar->addAction(action2);
    action = qobject_cast<MAction *>(m_toolbar->actions().at(2));
    QVERIFY(action);
    QVERIFY(action == action2);

    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);
    QVERIFY(action2->isChecked() == false);

    MButton* button2 = dynamic_cast<MButton *>(tabView->getWidget(action));
    QVERIFY(button2);
    QVERIFY(button0->isChecked());
    QVERIFY(!button1->isChecked());
    QVERIFY(!button2->isChecked());

    action2->setChecked(true);
    QVERIFY(action0->isChecked() == false);
    QVERIFY(action1->isChecked() == false);
    QVERIFY(action2->isChecked() == true);
    QVERIFY(!button0->isChecked());
    QVERIFY(!button1->isChecked());
    QVERIFY(button2->isChecked());
}

MWidgetAction *Ut_MToolBarView::createTextEditAction(MWidget *parentWidget)
{
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(parentWidget);
    actionTextEdit->setWidget(textEntry);
    return actionTextEdit;
}

void Ut_MToolBarView::testAddingRemoveActions()
{
    QPointer<MAction> action1 = new MAction("action", m_toolbar);
    action1->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action1);

    QPointer<MAction> action2 = new MAction("action", m_toolbar);
    action2->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action2);

    m_toolbar->removeAction(action1);
    m_toolbar->removeAction(action2);
    QVERIFY(!action1.isNull());
    QVERIFY(!action2.isNull());

}

void Ut_MToolBarView::testSizeHint_data()
{
    QTest::addColumn<bool>("portrait"); //Whether to add the action to portrait location
    QTest::addColumn<bool>("landscape"); //Whether to add the action to landscape location
    QTest::addColumn<bool>("rotate");  //Whether to rotate to portrait mode

    for(int i = 0; i < 2; i++) {
        QString rotationDescription(i?" (rotated)":"");
        QTest::newRow(("Adding to Portrait only" + rotationDescription).toLatin1()) << true << false << !!i;
        QTest::newRow(("Adding to Landscape only" + rotationDescription).toLatin1()) << false << true << !!i;
        QTest::newRow(("Adding to both Portrait and Landscape" + rotationDescription).toLatin1()) << true << true << !!i;
        QTest::newRow(("Adding to nowhere!" + rotationDescription).toLatin1()) << false << false << !!i;
    }

}
void Ut_MToolBarView::testSizeHint()
{
    QFETCH(bool, portrait);
    QFETCH(bool, landscape);
    QFETCH(bool, rotate);
    QList< QPointer<MAction> > actions;
    QList<QSizeF> minimumSizes;
    QList<QSizeF> maximumSizes;
    QList<QSizeF> preferredSizes;

    if(rotate) {
        appWin->setOrientationAngle(M::Angle90);
        qApp->processEvents();
    }
    for(int i = 0; i < 5; i++) {

        minimumSizes << m_toolbar->sizeHint(Qt::MinimumSize);
        maximumSizes << m_toolbar->sizeHint(Qt::MaximumSize);
        preferredSizes << m_toolbar->sizeHint(Qt::PreferredSize);

        QVERIFY(minimumSizes.at(i).isValid());
        QVERIFY(maximumSizes.at(i).isValid());
        QVERIFY(preferredSizes.at(i).isValid());

        MAction *action = new MAction("action", m_toolbar);
        if(landscape && portrait)
            action->setLocation(MAction::ToolBarLocation);
        else if(landscape)
            action->setLocation(MAction::ToolBarLandscapeLocation);
        else if(portrait)
            action->setLocation(MAction::ToolBarPortraitLocation);
        else
            action->setLocation(MAction::ApplicationMenuLocation);
        action->setCheckable(true);
        action->setChecked(true);
        actions << action;

        m_toolbar->addAction(action);

        if((landscape && !rotate) || (portrait && rotate)) {
            //After adding an action the size shouldn't be smaller
            QVERIFY(minimumSizes[i].width() <= m_toolbar->sizeHint(Qt::MinimumSize).width());
            QVERIFY(maximumSizes[i].width() <= m_toolbar->sizeHint(Qt::MaximumSize).width());
            QVERIFY(preferredSizes[i].width() <= m_toolbar->sizeHint(Qt::PreferredSize).width());
        } else {
            //After adding an non-landscape action the sizes shouldn't have changed
            QVERIFY(minimumSizes[i].width() == m_toolbar->sizeHint(Qt::MinimumSize).width());
            QVERIFY(maximumSizes[i].width() == m_toolbar->sizeHint(Qt::MaximumSize).width());
            QVERIFY(preferredSizes[i].width() == m_toolbar->sizeHint(Qt::PreferredSize).width());
        }
    }
    //A maximum of 4 items is allowed, so the sizehints for 4 actions should be the same as the sizehints for 5 actions
    QCOMPARE(minimumSizes[4], m_toolbar->sizeHint(Qt::MinimumSize));
    QCOMPARE(maximumSizes[4], m_toolbar->sizeHint(Qt::MaximumSize));
    QCOMPARE(preferredSizes[4], m_toolbar->sizeHint(Qt::PreferredSize));

    //Throw in rotation and rotation back again, just to be sure
    if(rotate) {
        appWin->setOrientationAngle(M::Angle0);
        appWin->setOrientationAngle(M::Angle90);
    } else {
        appWin->setOrientationAngle(M::Angle90);
        appWin->setOrientationAngle(M::Angle0);
    }

    for(int i = 4; i >= 0; i--) {
        m_toolbar->removeAction(actions[4-i]);
        QVERIFY(!actions[4-i].isNull());
        QCOMPARE(minimumSizes[i], m_toolbar->sizeHint(Qt::MinimumSize));
        QCOMPARE(maximumSizes[i], m_toolbar->sizeHint(Qt::MaximumSize));
        QCOMPARE(preferredSizes[i], m_toolbar->sizeHint(Qt::PreferredSize));
    }
}

#define WAIT_VERIFY(x) { int i = 0; while(!(x) && i++ < 100) { QTest::qWait(50); } QVERIFY(x); }
void Ut_MToolBarView::testButtons()
{
    QVERIFY(m_toolbar->actions().isEmpty());
    MAction *action = new MAction("Hello", m_toolbar);
    action->setLocation(MAction::ToolBarLandscapeLocation);
    action->setCheckable(true);
    action->setChecked(true);

    m_toolbar->addAction(action);
    QPointer<MButton> button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(button);
    QVERIFY(button->isEnabled());
    QVERIFY(button->isCheckable());
    QVERIFY(button->isChecked());
    QCOMPARE(button->text(), QString("Hello"));
    WAIT_VERIFY(button->isVisible());

    action->setChecked(false);
    QVERIFY(!button->isChecked());
    QCOMPARE(button->text(), QString("Hello"));

    action->setText("Goodbye");
    QVERIFY(!button->isChecked());
    QCOMPARE(button->text(), QString("Goodbye"));

    action->setVisible(false);
    WAIT_VERIFY(button.isNull() || !button->isVisible());

    /* Set the button back to how it was, but disabled */
    action->setEnabled(false);
    action->setChecked(true);
    action->setText("Hello");
    /* And now show the button again */
    action->setVisible(true);
    button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(button);
    QVERIFY(!button->isEnabled());
    QVERIFY(button->isCheckable());
    QVERIFY(button->isChecked());
    QCOMPARE(button->text(), QString("Hello"));
    WAIT_VERIFY(button->isVisible());

    /* Now change the location to landscape.  Nothing should change */
    action->setLocation(MAction::ToolBarLandscapeLocation);
    QVERIFY(button);
    QVERIFY(!button->isEnabled());
    QVERIFY(button->isCheckable());
    QVERIFY(button->isChecked());
    QCOMPARE(button->text(), QString("Hello"));
    WAIT_VERIFY(button->isVisible());

    /* Now change the location to portrait.  It should now be invisble */
    action->setLocation(MAction::ToolBarPortraitLocation);
    QVERIFY(button);
    QVERIFY(!button->isEnabled());
    QVERIFY(button->isCheckable());
    QVERIFY(button->isChecked());
    QCOMPARE(button->text(), QString("Hello"));
    WAIT_VERIFY(!button->isVisible());

    /* Change the location back to landscape so that we now see it again */
    action->setLocation(MAction::ToolBarLandscapeLocation);
    QVERIFY(button);
    QVERIFY(!button->isEnabled());
    QVERIFY(button->isCheckable());
    QVERIFY(button->isChecked());
    QCOMPARE(button->text(), QString("Hello"));
    WAIT_VERIFY(button->isVisible());

    m_toolbar->removeAction(action);
    QVERIFY(button.isNull()); //The toolbar owns the button, so it should have been deleted
}

void Ut_MToolBarView::testMWidgetAction_data()
{
    QTest::addColumn<QString>("widgetType"); //The type of widget to add - button, textedit etc

    QTest::newRow("Testing MWidgetAction with MButton") << "button";
    QTest::newRow("Testing MWidgetAction with MTextEdit") << "textedit";
    QTest::newRow("Testing MWidgetAction with MSlider") << "image";
}
void Ut_MToolBarView::testMWidgetAction()
{
    QFETCH(QString, widgetType);
    QPointer<MWidgetAction> action = new MWidgetAction(m_toolbar);
    action->setLocation(MAction::ToolBarLocation);
    action->setEnabled(true);
    action->setVisible(true);

    QPointer<MWidget> widget;
    if(widgetType == "textedit")
        widget = new MTextEdit;
    else if(widgetType == "button")
        widget = new MButton(m_toolbar);
    else
        widget = new MSlider(m_toolbar);
    action->setWidget(widget);

    m_toolbar->addAction(action);

    QVERIFY(widget);
    QVERIFY(widget->isEnabled());
    WAIT_VERIFY(widget->isVisible());

    m_toolbar->removeAction(action);
    QVERIFY(!action.isNull());
    QVERIFY(!widget.isNull());  //We own the widget, so it shouldn't have been deleted
    WAIT_VERIFY(!widget->isVisible());

    m_toolbar->addAction(action);

    QVERIFY(widget);
    QVERIFY(widget->isEnabled());
    WAIT_VERIFY(widget->isVisible());
    
}

QTEST_APPLESS_MAIN(Ut_MToolBarView)
