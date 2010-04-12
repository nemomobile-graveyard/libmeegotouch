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

#include "ut_mdialog.h"

void Ut_MDialog::initTestCase()
{
    app = buildApp(1, "./ut_mdialog");
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

void Ut_MDialog::initVals()
{
    QCOMPARE(dialog->clickedButton(), (MButtonModel *)NULL);
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
        MButton *button1 = new MButton();
        MButton *button2 = new MButton();

        dialog->setCentralWidget(button1);
        QCOMPARE(dialog->centralWidget(), button1);
        dialog->setCentralWidget(button2);
        QCOMPARE(dialog->centralWidget(), button2);
    }
}

void Ut_MDialog::addStandardButtons()
{
    MButtonModel *b1 = dialog->addButton(M::AbortButton);
    MButtonModel *b2 = dialog->addButton(M::OkButton);
    MButtonModel *b4 = dialog->addButton(M::NoButton);

    QCOMPARE(dialog->button(M::AbortButton), b1);
    QCOMPARE(dialog->button(M::OkButton), b2);
    QCOMPARE(dialog->button(M::NoButton), b4);
    QVERIFY(dialog->button(M::YesToAllButton) == 0);
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

MApplication *Ut_MDialog::buildApp(int count, const QString &params)
{
    QChar sep(' ');
    char *argv[MAX_PARAMS];
    int x = 0;

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }
    return new MApplication(count, argv);
}

void Ut_MDialog::dismissDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    //ABI FREEZE: Release this
    //dialog->dismissNow();
    dialog->close();

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 1);
    QCOMPARE(spyChanged3.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_MDialog);
