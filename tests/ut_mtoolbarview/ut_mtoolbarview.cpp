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
    } else {
        m_toolbar->removeAction(action3);
        m_toolbar->removeAction(action1);
        m_toolbar->removeAction(action2);
        QVERIFY(!action1.isNull());
        QVERIFY(!action2.isNull());
        QVERIFY(!action3.isNull());
    }
    QVERIFY(button1.isNull());
    QVERIFY(button2.isNull());
    QVERIFY(button3.isNull());
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
    QVERIFY(button.isNull()); //The toolbar owns the button, so it should have been deleted
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

QTEST_APPLESS_MAIN(Ut_MToolBarView)
