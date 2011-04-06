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

#include <QMetaType>
#include <MApplication>
#include <MApplicationWindow>

#include <MWidget>

#include <MButton>
#include <MDialog>
#include <MSceneWindow>
#include <MLayout>
#include <MPannableWidget>
#include <MDismissEvent>

#include <mbuttonmodel.h>
#include <mdialog_p.h>

#include "ut_mdialog.h"

MWindow *appWin;
void Ut_MDialog::initTestCase()
{
    static int argc = 1;
    static char *argv[] = { (char*)"./ut_mdialog" };
    app = new MApplication(argc, argv);
    appWin = new MWindow;
}

void Ut_MDialog::cleanupTestCase()
{
    delete app;
}

void Ut_MDialog::init()
{
    dialog = new MDialog();

    // For some reason libmeegotouch wants the controller to be notified about the existence of a
    // new model (via setupModel()) only when the event processing loop is reached.
    // Therefore, let's make sure setupModel() is called.
    // Otherwise the controller won't know about the new model and will therefore
    // lose all model::modified() signals.
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}

void Ut_MDialog::cleanup()
{
    delete dialog;
}

void Ut_MDialog::constructorWithParameters()
{
    QString title("Title");
    MDialog *dlg = new MDialog(title,
                                   M::OkButton | M::CancelButton | M::YesToAllButton |
                                   M::NoButton | M::ResetButton);

    QCOMPARE(dlg->title(), title);
    QVERIFY(dlg->button(M::OkButton));
    QVERIFY(dlg->button(M::CancelButton));
    QVERIFY(dlg->button(M::YesToAllButton));
    QVERIFY(dlg->button(M::NoButton));
    QVERIFY(dlg->button(M::ResetButton));

    QVERIFY(!dlg->button(M::SaveButton));
    QVERIFY(!dlg->button(M::SaveAllButton));
    QVERIFY(!dlg->button(M::OpenButton));
    QVERIFY(!dlg->button(M::YesButton));
    QVERIFY(!dlg->button(M::NoToAllButton));
    QVERIFY(!dlg->button(M::AbortButton));
    QVERIFY(!dlg->button(M::RetryButton));
    QVERIFY(!dlg->button(M::IgnoreButton));
    QVERIFY(!dlg->button(M::CloseButton));
    QVERIFY(!dlg->button(M::DiscardButton));
    QVERIFY(!dlg->button(M::HelpButton));
    QVERIFY(!dlg->button(M::ApplyButton));
    QVERIFY(!dlg->button(M::RestoreDefaultsButton));
}

void Ut_MDialog::testStandardButtonsInConstructor()
{
    QString title("Title");
    MDialog *dlg = new MDialog(title,
                                   M::SaveButton | M::SaveAllButton | M::OpenButton |
                                   M::YesButton | M::NoToAllButton| M::HelpButton |
                                   M::AbortButton | M::RetryButton | M::IgnoreButton |
                                   M::CloseButton | M::DiscardButton | M::ApplyButton |
                                   M::RestoreDefaultsButton |M::OkButton | M::CancelButton |
                                   M::YesToAllButton | M::NoButton | M::ResetButton |
                                   M::DoneButton);

    QVERIFY(dlg->button(M::SaveButton));
    QCOMPARE(dlg->button(M::SaveButton)->role(), M::AcceptRole);
    QVERIFY(dlg->button(M::SaveAllButton));
    QCOMPARE(dlg->button(M::SaveAllButton)->role(), M::AcceptRole);
    QVERIFY(dlg->button(M::OpenButton));
    QCOMPARE(dlg->button(M::OpenButton)->role(), M::AcceptRole);
    QVERIFY(dlg->button(M::YesButton));
    QCOMPARE(dlg->button(M::YesButton)->role(), M::YesRole);
    QVERIFY(dlg->button(M::NoToAllButton));
    QCOMPARE(dlg->button(M::NoToAllButton)->role(), M::NoRole);
    QVERIFY(dlg->button(M::HelpButton));
    QCOMPARE(dlg->button(M::HelpButton)->role(), M::HelpRole);
    QVERIFY(dlg->button(M::OkButton));
    QCOMPARE(dlg->button(M::OkButton)->role(), M::AcceptRole);
    QVERIFY(dlg->button(M::CancelButton));
    QCOMPARE(dlg->button(M::CancelButton)->role(), M::RejectRole);
    QVERIFY(dlg->button(M::YesToAllButton));
    QCOMPARE(dlg->button(M::YesToAllButton)->role(), M::YesRole);
    QVERIFY(dlg->button(M::NoButton));
    QCOMPARE(dlg->button(M::NoButton)->role(), M::NoRole);
    QVERIFY(dlg->button(M::ResetButton));
    QCOMPARE(dlg->button(M::ResetButton)->role(), M::ResetRole);
    QVERIFY(dlg->button(M::AbortButton));
    QCOMPARE(dlg->button(M::AbortButton)->role(), M::RejectRole);
    QVERIFY(dlg->button(M::RetryButton));
    QCOMPARE(dlg->button(M::RetryButton)->role(), M::AcceptRole);
    QVERIFY(dlg->button(M::IgnoreButton));
    QCOMPARE(dlg->button(M::IgnoreButton)->role(), M::AcceptRole);
    QVERIFY(dlg->button(M::CloseButton));
    QCOMPARE(dlg->button(M::CloseButton)->role(), M::RejectRole);
    QVERIFY(dlg->button(M::DiscardButton));
    QCOMPARE(dlg->button(M::DiscardButton)->role(), M::DestructiveRole);
    QVERIFY(dlg->button(M::ApplyButton));
    QCOMPARE(dlg->button(M::ApplyButton)->role(), M::ApplyRole);
    QVERIFY(dlg->button(M::RestoreDefaultsButton));
    QCOMPARE(dlg->button(M::RestoreDefaultsButton)->role(), M::ResetRole);
    QVERIFY(dlg->button(M::DoneButton));
    QCOMPARE(dlg->button(M::DoneButton)->role(), M::AcceptRole);
}

void Ut_MDialog::initVals()
{
    QCOMPARE(dialog->clickedButton(), (MButtonModel *)NULL);
    QCOMPARE(dialog->isModal(), true);
    QCOMPARE(dialog->isSystem(), false);
    QCOMPARE(dialog->contentsVerticalPanningPolicy(), MPannableWidget::PanningAsNeeded);
}

void Ut_MDialog::settersAndGetters()
{
    for (int i = 0; i < 5; i++) {
        int val = i * 345; // Arbitrary
        dialog->setResult(val);
        QCOMPARE(dialog->result(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setButtonBoxVisible(val);
        QCOMPARE(dialog->isButtonBoxVisible(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setTitleBarVisible(val);
        QCOMPARE(dialog->isTitleBarVisible(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setProgressIndicatorVisible(val);
        QCOMPARE(dialog->isProgressIndicatorVisible(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setCloseButtonVisible(val);
        QCOMPARE(dialog->isCloseButtonVisible(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setSystem(val);
        QCOMPARE(dialog->isSystem(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setModal(val);
        QCOMPARE(dialog->isModal(), val);
    }

    for (int i = 0; i < 4; i++) {
        bool val = i % 2 ? false : true;
        dialog->setSystemModal(val);
        QCOMPARE(dialog->isSystemModal(), val);
    }

    {
        QString val;
        val = QString("Test string 1");
        dialog->setTitle(val);
        QCOMPARE(dialog->title(), val);

        val = QString("Something else");
        dialog->setTitle(val);
        QCOMPARE(dialog->title(), val);
    }

    {
        QString val;
        val = QString("Test string 1");
        dialog->setTitleBarIconId(val);
        QCOMPARE(dialog->titleBarIconId(), val);

        val = QString("Something else");
        dialog->setTitleBarIconId(val);
        QCOMPARE(dialog->titleBarIconId(), val);
    }

    {
        MButton *button1 = new MButton();
        MButton *button2 = new MButton();

        dialog->setCentralWidget(button1);
        QCOMPARE(dialog->centralWidget(), button1);
        dialog->setCentralWidget(button2);
        QCOMPARE(dialog->centralWidget(), button2);
    }

    {
        dialog->setContentsVerticalPanningPolicy(MPannableWidget::PanningAlwaysOn);
        QCOMPARE(dialog->contentsVerticalPanningPolicy(), MPannableWidget::PanningAlwaysOn);
        dialog->setContentsVerticalPanningPolicy(MPannableWidget::PanningAlwaysOff);
        QCOMPARE(dialog->contentsVerticalPanningPolicy(), MPannableWidget::PanningAlwaysOff);
        dialog->setContentsVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
        QCOMPARE(dialog->contentsVerticalPanningPolicy(), MPannableWidget::PanningAsNeeded);
    }
}

void Ut_MDialog::checkStandardButtons()
{
    MButtonModel *b1 = dialog->addButton(M::OkButton);
    MButtonModel *b2 = dialog->addButton(M::CancelButton);
    MButtonModel *b3 = new MButtonModel;
    MButtonModel *b4 = NULL;

    QCOMPARE(dialog->standardButton(b1), M::OkButton);
    QCOMPARE(dialog->standardButton(b2), M::CancelButton);
    QCOMPARE(dialog->standardButton(b3), M::NoStandardButton);
    QCOMPARE(dialog->standardButton(b4), M::NoStandardButton);
}

void Ut_MDialog::addStandardButtons()
{
    MButtonModel *b1 = dialog->addButton(M::AbortButton);
    MButtonModel *b2 = dialog->addButton(M::OkButton);
    MButtonModel *b4 = dialog->addButton(M::NoButton);

    QCOMPARE(dialog->button(M::AbortButton), b1);
    QCOMPARE(dialog->button(M::AbortButton)->role(), M::RejectRole);
    QCOMPARE(dialog->button(M::OkButton), b2);
    QCOMPARE(dialog->button(M::OkButton)->role(), M::AcceptRole);
    QCOMPARE(dialog->button(M::NoButton), b4);
    QCOMPARE(dialog->button(M::NoButton)->role(), M::NoRole);
    QVERIFY(dialog->button(M::YesToAllButton) == 0);
}

void Ut_MDialog::addNonStandardButtonModel()
{
    const QIcon icon("icon-m-home");
    MButtonModel *mbm1 = new MButtonModel();
    MButtonModel *mbm2 = new MButtonModel();
    mbm1->setIcon( icon );
    dialog->addButton(mbm1);

    QCOMPARE(dialog->model()->buttons().at(0)->icon(), icon);
    QVERIFY(dialog->model()->buttons().contains(mbm1));
    QVERIFY(!dialog->model()->buttons().contains(mbm2));

    dialog->addButton(mbm2);

    QVERIFY(dialog->model()->buttons().contains(mbm1));
    QVERIFY(dialog->model()->buttons().contains(mbm2));
}

void Ut_MDialog::addNonStandardButtonsWithRole()
{
    MButtonModel *b1 = dialog->addButton("Custom Action Button", M::AcceptRole);
    MButtonModel *b2 = dialog->addButton("Custom Reject Button", M::RejectRole);

    QCOMPARE(b1->role(), M::AcceptRole);
    QCOMPARE(b2->role(), M::RejectRole);
}

void Ut_MDialog::addInvalidStandardButton()
{
    MButtonModel *buttonModel = dialog->addButton(M::NoStandardButton);
    QVERIFY(buttonModel == NULL);
}

void Ut_MDialog::addExistingStandardButton()
{
    MButtonModel *b1 = dialog->addButton(M::OkButton);
    MButtonModel *b2 = dialog->addButton(M::OkButton);

    QCOMPARE(dialog->button(M::OkButton), b1);
    QCOMPARE(dialog->button(M::OkButton), b2);
    QVERIFY(b1 == b2);
}

void Ut_MDialog::checkLayouts()
{
    MLayout *l1 = new MLayout;
    MLayout *l2 = new MLayout;
    dialog->setLayout(l1);

    QCOMPARE(dialog->layout(), l1);

    dialog->setLayout(l2);

    QCOMPARE(dialog->layout(), l2);
}

void Ut_MDialog::acceptDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    MButtonModel *b1 = dialog->addButton("test button 1");
    dialog->addButton("test button 2");
    b1->click();

    QCOMPARE(spyChanged1.count(), 1);
    QCOMPARE(spyChanged2.count(), 0);
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_MDialog::acceptDialogDirectly()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    dialog->accept();

    QCOMPARE(spyChanged1.count(), 1);
    QCOMPARE(spyChanged2.count(), 0);
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_MDialog::removeButton()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));

    MButtonModel *b1 = dialog->addButton("test button 1");
    b1->increaseReferenceCount(); // otherwise it will get deleted on removeButon() call.
    dialog->addButton("test button 2");

    dialog->removeButton(b1);
    b1->click();
    delete b1;
    b1 = 0;

    QCOMPARE(spyChanged1.count(), 0);
}

void Ut_MDialog::rejectDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    dialog->addButton("test button 1");
    MButtonModel *b2 = dialog->addButton(M::CancelButton);
    b2->click();

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 1);
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_MDialog::rejectDialogDirectly()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    dialog->reject();

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 1);
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_MDialog::dismissDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    dialog->close();

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 1);
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_MDialog::testRotation()
{
    dialog->appear(appWin);
    appWin->setOrientationAngle(M::Angle0);
    QSizeF preferredSizeLandscape = dialog->preferredSize();
    appWin->setOrientationAngle(M::Angle90);
    QSizeF preferredSizePortrait = dialog->preferredSize();
    appWin->setOrientationAngle(M::Angle0);
    QCOMPARE(dialog->preferredSize(), preferredSizeLandscape);
    appWin->setOrientationAngle(M::Angle90);
    QCOMPARE(dialog->preferredSize(), preferredSizePortrait);
}

void Ut_MDialog::testSystemDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged2(dialog, SIGNAL(finished(int)));

    dialog->setSystem(true);
    dialog->setModal(false);
    dialog->appear(MSceneWindow::KeepWhenDone);
    QVERIFY(dialog->d_func()->standAloneWindow);
    dialog->dismissEvent(new MDismissEvent());

    QCOMPARE(spyChanged1.count(), 1);
    QCOMPARE(spyChanged2.count(), 1);
}

void Ut_MDialog::testSystemModalDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged2(dialog, SIGNAL(finished(int)));
    QSignalSpy spyChanged3(dialog, SIGNAL(accepted()));

    dialog->setSystem(true);
    dialog->setModal(true);
    dialog->appear();
    QVERIFY(dialog->d_func()->standAloneWindow);
    dialog->dismissEvent(new MDismissEvent());

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 0);
    QCOMPARE(spyChanged3.count(), 0);
    dialog->accept();
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_MDialog::testCentralWidgetSurviveDoubleDelete()
{
    QGraphicsWidget *newCentralWidget = new QGraphicsWidget(dialog);
    QGraphicsWidget *anotherCentralWidget = new QGraphicsWidget(dialog);
    dialog->setCentralWidget(newCentralWidget);
    delete newCentralWidget;
    dialog->setCentralWidget(anotherCentralWidget);
    QVERIFY(true);
}

QTEST_APPLESS_MAIN(Ut_MDialog);
