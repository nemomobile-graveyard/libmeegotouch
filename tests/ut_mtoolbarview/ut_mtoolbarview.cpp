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
#include "mtoolbarview_p.h"

MApplication *app;
MApplicationWindow *appWin;

#define WAIT_VERIFY(x) { int i = 0; while(!(x) && i++ < 100) { QTest::qWait(50); } QVERIFY(x); }

void Ut_MToolBarView::initTestCase()

{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MToolBarView" };
    app = new MApplication(argc, app_name);

    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mtoolbarview.css");

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
    QPointer<MWidgetAction> action = createTextEditAction(parentWidget);
    m_toolbar->addAction(action);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete parentWidget;

    // toolbar should not crash as action has been deleted
    QVERIFY(m_toolbar->actions().count() == 0);
    QVERIFY(action.isNull());

    // Add another action just to sure
    m_toolbar->addAction(new MAction("TEXTONLY", m_toolbar));

    QVERIFY(m_toolbar->actions().count() == 1);
}
void Ut_MToolBarView::testDeleteAction()
{
    m_toolbar->clearActions();

    MWidget *parentWidget = new MWidget();
    QPointer<MWidgetAction> action = createTextEditAction(parentWidget);
    m_toolbar->addAction(action);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete action;

    // toolbar should not crash as action has been deleted
    QVERIFY(m_toolbar->actions().count() == 0);
    QVERIFY(action.isNull());
}
void Ut_MToolBarView::testDeleteAndAddingTextWidget()
{
    //This is testing that we properly delete a text edit action
    MWidget *parentWidget = new MWidget();
    QPointer<MWidgetAction> action = createTextEditAction(parentWidget);
    m_toolbar->addAction(action);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete action;

    //We should now be completely empty, and thus we can add a text edit again, and 4 more actions..
    MWidget *parentWidget2 = new MWidget();
    QPointer<MWidgetAction> action2 = createTextEditAction(parentWidget2);
    m_toolbar->addAction(action2);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(m_toolbar->actions().count() == 1);
    QVERIFY(action2->widget()->isVisible()); //Should be visible

    //Try adding a second text edit.  Should not show
    MWidget *parentWidget3 = new MWidget();
    QPointer<MWidgetAction> action3 = createTextEditAction(parentWidget3);
    m_toolbar->addAction(action3);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(m_toolbar->actions().count() == 2);
    QVERIFY(action2->widget()->isVisible()); //Should still be visible
    QVERIFY(!action3->widget()->isVisible()); //New one should not be visible

    //Now add four normal widgets
    MAction *action4 = new MAction("action4", m_toolbar);
    action4->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action4);

    MAction *action5 = new MAction("action4", m_toolbar);
    action5->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action5);

    MAction *action6 = new MAction("action4", m_toolbar);
    action6->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action6);

    MAction *action7 = new MAction("action4", m_toolbar);
    action7->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action7);

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    //Two of the normal actions should be visible, and two not
    MButton* button4 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action4));
    MButton* button5 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action5));
    MButton* button6 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action6));
    MButton* button7 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action7));
    QVERIFY(button4);
    QVERIFY(button4->isVisible());
    QVERIFY(button5);
    QVERIFY(button5->isVisible());
    QVERIFY(!button6 || !button6->isVisible());
    QVERIFY(!button7 || !button7->isVisible());

    //Now delete the showing text edit.  The second should become visible
    delete parentWidget2;
    QVERIFY(action2.isNull());

    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    QVERIFY(action3->widget()->isVisible()); //New one should now be visible

    button4 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action4));
    button5 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action5));
    button6 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action6));
    button7 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action7));
    QVERIFY(button4);
    QVERIFY(button4->isVisible());
    QVERIFY(button5);
    QVERIFY(button5->isVisible());
    QVERIFY(!button6 || !button6->isVisible());
    QVERIFY(!button7 || !button7->isVisible());

    //Now remove the third text edit
    m_toolbar->removeAction(action3);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    //All the buttons should now be visible again
    QVERIFY(!action3->widget()->isVisible());

    button4 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action4));
    button5 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action5));
    button6 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action6));
    button7 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action7));
    QVERIFY(button4);
    QVERIFY(button4->isVisible());
    QVERIFY(button5);
    QVERIFY(button5->isVisible());
    QVERIFY(button6);
    QVERIFY(button6->isVisible());
    QVERIFY(button7);
    QVERIFY(button7->isVisible());

    //Try inserting the toolbar again before the 4th button visible.
    //This should fail since there isn't room for it
    m_toolbar->insertAction(action7, action3);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(!action3->widget()->isVisible());
    button4 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action4));
    button5 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action5));
    button6 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action6));
    button7 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action7));
    QVERIFY(button4);
    QVERIFY(button4->isVisible());
    QVERIFY(button5);
    QVERIFY(button5->isVisible());
    QVERIFY(button6);
    QVERIFY(button6->isVisible());
    QVERIFY(button7);
    QVERIFY(button7->isVisible());

    //Try inserting the toolbar before the 3rd button visible.
    //This should now work, pushing off button6 and button7
    m_toolbar->insertAction(action6, action3);
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(action3->widget()->isVisible());
    button4 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action4));
    button5 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action5));
    button6 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action6));
    button7 = dynamic_cast<MButton *>(m_toolbarview->getWidget(action7));
    QVERIFY(button4);
    QVERIFY(button4->isVisible());
    QVERIFY(button5);
    QVERIFY(button5->isVisible());
    QVERIFY(!button6 || !button6->isVisible());
    QVERIFY(!button7 || !button7->isVisible());

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

    QPointer<MButton> button2 = dynamic_cast<MButton *>(tabView->getWidget(action2));
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

    action2->setVisible(false);
    // validation of checked button is deferred to a separate event
    QCoreApplication::processEvents();

    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);
    QVERIFY(action2->isChecked() == false);

    WAIT_VERIFY(!button2 || !button2->isVisible());
    QVERIFY(button0->isChecked());
    QVERIFY(!button1->isChecked());
    QVERIFY(!button2 || !button2->isChecked());

    action2->setVisible(true);

    button2 = dynamic_cast<MButton *>(tabView->getWidget(action2));
    QVERIFY(action0->isChecked() == true);
    QVERIFY(action1->isChecked() == false);
    QVERIFY(action2->isChecked() == false);

    WAIT_VERIFY(button2);
    WAIT_VERIFY(button2->isVisible());
    QVERIFY(button0->isChecked());
    QVERIFY(!button1->isChecked());
    QVERIFY(!button2->isChecked());
}

MWidgetAction *Ut_MToolBarView::createTextEditAction(MWidget *parentWidget)
{
    MTextEdit *textEntry = new MTextEdit();
    MWidgetAction *actionTextEdit = new MWidgetAction(parentWidget);
    actionTextEdit->setWidget(textEntry);
    return actionTextEdit;
}

void Ut_MToolBarView::testInsertingActions()
{
    QPointer<MAction> action1 = new MAction("action", m_toolbar);
    action1->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action1);
    QPointer<MButton> button1 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action1));

    QPointer<MAction> action2 = new MAction("action", m_toolbar);
    action2->setLocation(MAction::ToolBarLocation);
    m_toolbar->insertAction(action1, action2);
    QPointer<MButton> button2 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action2));

    qApp->processEvents();
    QCOMPARE(m_toolbar->actions().count(), 2);
    QVERIFY(m_toolbar->actions()[0] == action2);
    QVERIFY(m_toolbar->actions()[1] == action1);
    QVERIFY(button2->geometry().left() < button1->geometry().left());

    action1->setVisible(false);
    button1 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action1));

    //Check we can insert before a hidden action
    QPointer<MAction> action3 = new MAction("action", m_toolbar);
    action3->setLocation(MAction::ToolBarLocation);
    m_toolbar->insertAction(action1, action3);
    QPointer<MButton> button3 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action3));

    qApp->processEvents();
    QCOMPARE(m_toolbar->actions().count(), 3);
    QVERIFY(m_toolbar->actions()[0] == action2);
    QVERIFY(m_toolbar->actions()[1] == action3);
    QVERIFY(m_toolbar->actions()[2] == action1);
    QVERIFY(button2->geometry().left() < button3->geometry().left());
    QCOMPARE(button1 && button1->isVisible(), false);

    //Check if we insert a hidden action then make it visible, it's in the right place
    QPointer<MAction> action4 = new MAction("action", m_toolbar);
    action4->setLocation(MAction::ToolBarLocation);
    action4->setVisible(false);
    m_toolbar->insertAction(action2, action4);
    action4->setVisible(true);
    QPointer<MButton> button4 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action4));

    qApp->processEvents();
    QCOMPARE(m_toolbar->actions().count(), 4);
    QVERIFY(m_toolbar->actions()[0] == action4);
    QVERIFY(m_toolbar->actions()[1] == action2);
    QVERIFY(m_toolbar->actions()[2] == action3);
    QVERIFY(m_toolbar->actions()[3] == action1);
    QVERIFY(button4->geometry().left() < button2->geometry().left());
    QVERIFY(button2->geometry().left() < button3->geometry().left());
    QCOMPARE(button1 && button1->isVisible(), false);
    QCOMPARE(button4->isVisible(), true);

    //Add an action at the end, so we now have 4 visible buttons and 1 hidden button
    QPointer<MAction> action5 = new MAction("action", m_toolbar);
    action5->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action5);
    QPointer<MButton> button5 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action5));

    qApp->processEvents();
    QCOMPARE(m_toolbar->actions().count(), 5);
    QVERIFY(m_toolbar->actions()[0] == action4);
    QVERIFY(m_toolbar->actions()[1] == action2);
    QVERIFY(m_toolbar->actions()[2] == action3);
    QVERIFY(m_toolbar->actions()[3] == action1);
    QVERIFY(m_toolbar->actions()[4] == action5);

    QVERIFY(button4->geometry().left() < button2->geometry().left());
    QVERIFY(button2->geometry().left() < button3->geometry().left());
    QVERIFY(button3->geometry().left() < button5->geometry().left());
    QCOMPARE(button1 && button1->isVisible(), false);

    //Make action1 visible again.  This should push action5 off so that it's hidden
    action1->setVisible(true);
    button1 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action1));
    button5 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action5));
    QVERIFY(button1);
    qApp->processEvents();

    QCOMPARE(button1->isVisible(), true);
    QCOMPARE(button5 && button5->isVisible(), false);

    QCOMPARE(m_toolbar->actions().count(), 5);
    QVERIFY(m_toolbar->actions()[0] == action4);
    QVERIFY(m_toolbar->actions()[1] == action2);
    QVERIFY(m_toolbar->actions()[2] == action3);
    QVERIFY(m_toolbar->actions()[3] == action1);
    QVERIFY(m_toolbar->actions()[4] == action5);
    QVERIFY(button4->geometry().left() < button2->geometry().left());
    QVERIFY(button2->geometry().left() < button3->geometry().left());
    QVERIFY(button3->geometry().left() < button1->geometry().left());
}
void Ut_MToolBarView::testAddingRemoveActions_data()
{
    QTest::addColumn<bool>("deleteAction");
    QTest::newRow("Adding and removing action") << false;
    QTest::newRow("Adding and deleting action") << true;
}
void Ut_MToolBarView::testAddingRemoveActions()
{
    QFETCH(bool, deleteAction);
    QPointer<MAction> action1 = new MAction("action", m_toolbar);
    action1->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action1);
    QPointer<MButton> button1 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action1));

    QPointer<MAction> action2 = new MAction("action", m_toolbar);
    action2->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action2);
    QPointer<MButton> button2 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action2));
    action2->setVisible(false);

    QPointer<MAction> action3 = new MAction("action", m_toolbar);
    action3->setLocation(MAction::ToolBarLocation);
    action3->setVisible(false);
    m_toolbar->addAction(action3);
    QPointer<MButton> button3 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action3));

    QVERIFY(!button1.isNull());
    QVERIFY(!button2.isNull());
    QVERIFY(button3.isNull());

    if(deleteAction) {
        delete action3;
        delete action1;
        delete action2;
        // After deleting the actions, one can no longer check for them in
        // the QCache but the 2 buttons will be there
        QVERIFY(m_toolbarview->d_ptr->removedActionsButtons.count() == 2);
    } else {
        m_toolbar->removeAction(action3);
        m_toolbar->removeAction(action1);
        m_toolbar->removeAction(action2);
        QVERIFY(!action1.isNull());
        QVERIFY(!action2.isNull());
        QVERIFY(!action3.isNull());

        QPointer<MButton> removedButton = 0;
        removedButton = m_toolbarview->d_ptr->removedActionsButtons.take(action1);
        QVERIFY(button1 == removedButton);
        removedButton = m_toolbarview->d_ptr->removedActionsButtons.take(action2);
        QVERIFY(button2 == removedButton);
        removedButton = m_toolbarview->d_ptr->removedActionsButtons.take(action3);
        QVERIFY(button3 == removedButton);
    }

    if(!deleteAction) {
        //Delete now, after they've been removed, just to clean up
        delete action1;
        delete action2;
        delete action3;
    }
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

        MAction *action = new MAction("icon-l-search", "action", m_toolbar);
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

    //This is an implementation specific check.  It's nice
    //if the button is not deleted so that it's available
    //straight away if we need it again
    WAIT_VERIFY(!button.isNull());
    WAIT_VERIFY(!button->isVisible());

    /* Set the button back to how it was, but disabled */
    action->setEnabled(false);
    action->setChecked(true);
    action->setText("Hello");
    /* And now show the button again */
    action->setVisible(true);

    /* Another implementation specific check. */
    QVERIFY(button == dynamic_cast<MButton*>(m_toolbarview->getWidget(action)));

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
    // When an action is removed, the associated button is added to a cache of
    // buttons to avoid having to recreate it if the action is added again
    MButton *removedButton = m_toolbarview->d_ptr->removedActionsButtons.take(action);
    QVERIFY(button == removedButton);
}

void Ut_MToolBarView::testMWidgetAction_data()
{
    QTest::addColumn<QString>("widgetType"); //The type of widget to add - button, textedit etc

    QTest::newRow("Testing MWidgetAction with MButton") << "button";
    QTest::newRow("Testing MWidgetAction with MTextEdit") << "textedit";
//    QTest::newRow("Testing MWidgetAction with MSlider") << "image";
}
void Ut_MToolBarView::testMWidgetAction()
{
    QFETCH(QString, widgetType);
    QPointer<MWidgetAction> action = new MWidgetAction(m_toolbar);
    action->setLocation(MAction::ToolBarLocation);
    action->setEnabled(true);
    action->setVisible(true);

    MButton *button = NULL;
    QPointer<MWidget> widget;
    if(widgetType == "textedit")
        widget = new MTextEdit;
    else if(widgetType == "button") {
        button = new MButton(m_toolbar);
        button->setText("Hello");
        widget = button;
    } else
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

    if(button) {
        //MWidgetAction changes should not reflect to properties of the attached widget
        QCOMPARE(button->text(), QString("Hello"));
        action->setText(QString("Goodbye"));
        QCOMPARE(button->text(), QString("Hello"));
        QCOMPARE(button->isChecked(), false);
        action->setCheckable(true);
        action->setChecked(true);
        QCOMPARE(button->isChecked(), false);
    }
}

void Ut_MToolBarView::testSetWidgetAfterWidgetActionIsAdded()
{
    QPointer<MWidgetAction> action = new MWidgetAction(m_toolbar);
    action->setLocation(MAction::ToolBarLocation);

    m_toolbar->addAction(action);
    QVERIFY(m_toolbar->actions().count() == 1);

    QPointer<MTextEdit> widget = new MTextEdit;
    action->setWidget(widget);

    QPointer<MTextEdit> widgetInToolBar = dynamic_cast<MTextEdit*>(m_toolbarview->getWidget(action));
    QVERIFY(widgetInToolBar);

    qApp->processEvents();

    QVERIFY(widgetInToolBar->isVisible());
}

void Ut_MToolBarView::testChangeWidgetInWidgetAction()
{
    QPointer<MWidgetAction> action = new MWidgetAction(m_toolbar);
    action->setLocation(MAction::ToolBarLocation);
    QPointer<MTextEdit> widget = new MTextEdit;
    action->setWidget(widget);
    m_toolbar->addAction(action);

    QPointer<MTextEdit> editInToolBar = dynamic_cast<MTextEdit*>(m_toolbarview->getWidget(action));
    QVERIFY(editInToolBar);

    MWidget *widgetInAction = action->widget();
    QVERIFY(editInToolBar == widgetInAction);
    action->releaseWidget(editInToolBar);

    QPointer<MButton> button = new MButton;
    action->setWidget(button);

    QPointer<MButton> widgetInToolBar = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(widgetInToolBar);

    widgetInAction = action->widget();
    QVERIFY(widgetInToolBar == widgetInAction);

    qApp->processEvents();

    QVERIFY(widgetInToolBar->isVisible());
}

void Ut_MToolBarView::testRemoveWidgetFromWidgetAction()
{
    QPointer<MWidgetAction> action = new MWidgetAction(m_toolbar);
    action->setLocation(MAction::ToolBarLocation);
    QPointer<MTextEdit> widget = new MTextEdit;
    action->setWidget(widget);
    m_toolbar->addAction(action);

    QPointer<MTextEdit> widgetInToolBar = dynamic_cast<MTextEdit*>(m_toolbarview->getWidget(action));
    QVERIFY(widgetInToolBar);

    MWidget *widgetInAction = action->widget();
    action->releaseWidget(widgetInAction);

    action->setWidget(0);

    QVERIFY(!m_toolbarview->getWidget(action));
    QVERIFY(m_toolbar->actions().count() == 1);
}

void Ut_MToolBarView::testAddToLandscapeWhenInPortrait()
{
    QVERIFY(m_toolbar->actions().isEmpty());

    appWin->setOrientationAngle(M::Angle90);
    qApp->processEvents();

    MAction *action = new MAction("Hello", m_toolbar);
    action->setLocation(MAction::ToolBarLandscapeLocation);
    m_toolbar->addAction(action);

    //Since we are in landscape mode we should not see this action
    QPointer<MButton> button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    WAIT_VERIFY(!button || !button->isVisible());
}

void Ut_MToolBarView::testChangingLocation()
{
    MAction *action = new MAction("Hello", m_toolbar);
    action->setLocation(MAction::ToolBarLocation);

    //It should get distributed to the toolbar
    appWin->addAction(action);

    MToolBar *toolbar = NULL;
    foreach(QGraphicsWidget *widget, action->associatedGraphicsWidgets()) {
        if( (toolbar = qobject_cast<MToolBar *>(widget)) )
            break;
    }
    //Check that the action was added to the toolbar
    QVERIFY(toolbar);
    QVERIFY(toolbar->actions().count() == 1);

    const MToolBarView *view = dynamic_cast<const MToolBarView *>(toolbar->view());
    QVERIFY(view);
    const MButton* button = dynamic_cast<MButton *>(view->getWidget(action));

    QVERIFY(button);
    WAIT_VERIFY(button->isVisible());

    action->setLocation(MAction::NoLocation);

    //Remove the action
    WAIT_VERIFY(!button || !button->isVisible());

    //Readd the action
    action->setLocation(MAction::EveryLocation);
    button = dynamic_cast<const MButton *>(view->getWidget(action));
    QVERIFY(button);
    WAIT_VERIFY(button->isVisible());
}


void Ut_MToolBarView::createDummyActions(int count)
{
    for (int i=0; i<count; i++)
        createAction();
}

MAction *Ut_MToolBarView::createAction()
{
    MAction *action = new MAction("icon-l-search", "action", m_toolbar);
    action->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(action);
    return action;
}

bool Ut_MToolBarView::isVisible(MButton *button)
{
    return (button && button->isVisible());
}

void Ut_MToolBarView::testCapacityLimitedTo3()
{
    appWin->setOrientationAngle(M::Angle90);

    createDummyActions(3);

    QPointer<MAction> action4 = createAction();

    qApp->processEvents();

    QPointer<MButton> button4 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action4));

    QCOMPARE(m_toolbar->actions().count(), 4);
    QVERIFY(isVisible(button4) == false);
}

void Ut_MToolBarView::testCapacityLimitedTo3AndHidingAndShowingFirstButton()
{
    appWin->setOrientationAngle(M::Angle90);

    QPointer<MAction> action1 = createAction();
    createDummyActions(2);
    QPointer<MAction> action4 = createAction();

    action1->setVisible(false);

    qApp->processEvents();

    QPointer<MButton> button1 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action1));
    QPointer<MButton> button4 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action4));

    QCOMPARE(m_toolbar->actions().count(), 4);
    QVERIFY(isVisible(button1) == false);
    QVERIFY(isVisible(button4));

    action1->setVisible(true);

    qApp->processEvents();

    QVERIFY(isVisible(button1));
    QVERIFY(isVisible(button4) == false);

}

void Ut_MToolBarView::testCapacityIncreaseTo4WhenRotatedToLandscape()
{
    appWin->setOrientationAngle(M::Angle90);

    createDummyActions(3);
    QPointer<MAction> action4 = createAction();

    QCOMPARE(m_toolbar->actions().count(), 4);

    appWin->setOrientationAngle(M::Angle0);

    qApp->processEvents();

    QPointer<MButton> button4 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action4));

    QVERIFY(isVisible(button4));
}

void Ut_MToolBarView::testCapacityDecreaseTo3WhenRotatedToPortrait()
{
    appWin->setOrientationAngle(M::Angle0);

    createDummyActions(3);
    QPointer<MAction> action4 = createAction();

    qApp->processEvents();

    QPointer<MButton> button4 = dynamic_cast<MButton*>(m_toolbarview->getWidget(action4));

    QCOMPARE(m_toolbar->actions().count(), 4);
    QVERIFY(isVisible(button4));

    appWin->setOrientationAngle(M::Angle90);

    qApp->processEvents();

    QCOMPARE(m_toolbar->actions().count(), 4);
    QVERIFY(isVisible(button4) == false);
}

void Ut_MToolBarView::testTabBarCapacityLimitedTo5()
{
    appWin->setOrientationAngle(M::Angle90);
    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);
    m_toolbar->setView(tabView);

    createDummyActions(4);
    QPointer<MAction> action5 = createAction();
    QPointer<MAction> action6 = createAction();

    qApp->processEvents();

    QPointer<MButton> button5 = dynamic_cast<MButton*>(tabView->getWidget(action5));
    QPointer<MButton> button6 = dynamic_cast<MButton*>(tabView->getWidget(action6));

    QCOMPARE(m_toolbar->actions().count(), 6);
    QVERIFY(isVisible(button5));
    QVERIFY(isVisible(button6) == false);
}

void Ut_MToolBarView::testIconIsVisibleOnLandscape()
{
    appWin->setOrientationAngle(M::Angle0);

    QPointer<MAction> action = createAction();

    qApp->processEvents();

    QPointer<MButton> button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(button);
    QVERIFY(button->isIconVisible());
}

void Ut_MToolBarView::testIconIsNotVisibleOnPortrait()
{
    appWin->setOrientationAngle(M::Angle90);

    QPointer<MAction> action = createAction();

    qApp->processEvents();

    QPointer<MButton> button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(button);
    QVERIFY(button->isIconVisible() == false);
}

void Ut_MToolBarView::testLabelIsNotVisibleOnLandscape()
{
    appWin->setOrientationAngle(M::Angle0);

    QPointer<MAction> action = createAction();

    qApp->processEvents();

    QPointer<MButton> button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(button);
    QVERIFY(button->isTextVisible() == false);
}

void Ut_MToolBarView::testLabelIsVisibleOnPortrait()
{
    appWin->setOrientationAngle(M::Angle90);

    QPointer<MAction> action = createAction();

    qApp->processEvents();

    QPointer<MButton> button = dynamic_cast<MButton*>(m_toolbarview->getWidget(action));
    QVERIFY(button);
    QVERIFY(button->isTextVisible());
}

void Ut_MToolBarView::testIconAndLabelAreVisible()
{
    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);
    m_toolbar->setView(tabView);

    QPointer<MAction> action = createAction();

    qApp->processEvents();

    QPointer<MButton> button = dynamic_cast<MButton*>(tabView->getWidget(action));
    QVERIFY(button);
    QVERIFY(button->isIconVisible());
    QVERIFY(button->isTextVisible());
}

void Ut_MToolBarView::testLabelOnlyButtonsHaveDifferentStylename()
{
    MAction *actionIconAndLabel = new MAction("icon-l-search", "action", m_toolbar);
    actionIconAndLabel->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(actionIconAndLabel);

    MAction *actionLabelOnly = new MAction("action", m_toolbar);
    actionLabelOnly->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(actionLabelOnly);

    qApp->processEvents();

    QPointer<MButton> buttonIconAndLabel = dynamic_cast<MButton*>(m_toolbarview->getWidget(actionIconAndLabel));
    QPointer<MButton> buttonLabelOnly = dynamic_cast<MButton*>(m_toolbarview->getWidget(actionLabelOnly));
    QVERIFY(buttonIconAndLabel);
    QVERIFY(buttonLabelOnly);

    QString iconLabelStyleName = buttonIconAndLabel->styleName();
    QVERIFY(iconLabelStyleName.isEmpty() == false);
    QString labelOnlyStyleName = buttonLabelOnly->styleName();
    QVERIFY(labelOnlyStyleName.isEmpty() == false);
    QVERIFY(iconLabelStyleName != labelOnlyStyleName);
}

void Ut_MToolBarView::testLabelOnlyAreCommonButtonsInToolBar()
{
    MAction *actionIconAndLabel = new MAction("icon-l-search", "action", m_toolbar);
    actionIconAndLabel->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(actionIconAndLabel);

    MAction *actionLabelOnly = new MAction("action", m_toolbar);
    actionLabelOnly->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(actionLabelOnly);

    qApp->processEvents();

    QPointer<MButton> buttonIconAndLabel = dynamic_cast<MButton*>(m_toolbarview->getWidget(actionIconAndLabel));
    QPointer<MButton> buttonLabelOnly = dynamic_cast<MButton*>(m_toolbarview->getWidget(actionLabelOnly));
    QVERIFY(buttonIconAndLabel);
    QVERIFY(buttonLabelOnly);

    QVERIFY(buttonIconAndLabel->viewType() == "toolbar");
    QVERIFY(buttonLabelOnly->viewType() == "default");
}

void Ut_MToolBarView::testLabelOnlyAreNotCommonButtonsInTabBar()
{
    MToolbarTabView* tabView = new MToolbarTabView(m_toolbar);
    QVERIFY(tabView != 0);
    m_toolbar->setView(tabView);

    MAction *actionIconAndLabel = new MAction("icon-l-search", "action", m_toolbar);
    actionIconAndLabel->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(actionIconAndLabel);

    MAction *actionLabelOnly = new MAction("action", m_toolbar);
    actionLabelOnly->setLocation(MAction::ToolBarLocation);
    m_toolbar->addAction(actionLabelOnly);

    qApp->processEvents();

    QPointer<MButton> buttonIconAndLabel = dynamic_cast<MButton*>(tabView->getWidget(actionIconAndLabel));
    QPointer<MButton> buttonLabelOnly = dynamic_cast<MButton*>(tabView->getWidget(actionLabelOnly));
    QVERIFY(buttonIconAndLabel);
    QVERIFY(buttonLabelOnly);

    QVERIFY(buttonIconAndLabel->viewType() == "toolbartab");
    QVERIFY(buttonLabelOnly->viewType() == "toolbartab");
}

QTEST_APPLESS_MAIN(Ut_MToolBarView)
