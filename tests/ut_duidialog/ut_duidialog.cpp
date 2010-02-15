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

#include <QMetaType>
#include <DuiApplication>
#include <DuiApplicationWindow>

#include <DuiWidget>

#include <DuiButton>
#include <DuiDialog>
#include <DuiSceneWindow>

#include "ut_duidialog.h"

void Ut_DuiDialog::initTestCase()
{
    app = buildApp(1, "./ut_duidialog");
}

void Ut_DuiDialog::cleanupTestCase()
{
    delete app;
}

void Ut_DuiDialog::init()
{
    dialog = new DuiDialog();

    // For some reason libdui wants the controller to be notified about the existence of a
    // new model (via setupModel()) only when the event processing loop is reached.
    // Therefore, let's make sure setupModel() is called.
    // Otherwise the controller won't know about the new model and will therefore
    // lose all model::modified() signals.
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}

void Ut_DuiDialog::cleanup()
{
    delete dialog;
}

void Ut_DuiDialog::constructorWithParameters()
{
    QString title("Title");
    DuiDialog *dlg = new DuiDialog(title,
                                   Dui::OkButton | Dui::CancelButton | Dui::YesToAllButton |
                                   Dui::NoButton | Dui::ResetButton);

    QCOMPARE(dlg->title(), title);
    QVERIFY(dlg->button(Dui::OkButton));
    QVERIFY(dlg->button(Dui::CancelButton));
    QVERIFY(dlg->button(Dui::YesToAllButton));
    QVERIFY(dlg->button(Dui::NoButton));
    QVERIFY(dlg->button(Dui::ResetButton));

    QVERIFY(!dlg->button(Dui::SaveButton));
    QVERIFY(!dlg->button(Dui::SaveAllButton));
    QVERIFY(!dlg->button(Dui::OpenButton));
    QVERIFY(!dlg->button(Dui::YesButton));
    QVERIFY(!dlg->button(Dui::NoToAllButton));
    QVERIFY(!dlg->button(Dui::AbortButton));
    QVERIFY(!dlg->button(Dui::RetryButton));
    QVERIFY(!dlg->button(Dui::IgnoreButton));
    QVERIFY(!dlg->button(Dui::CloseButton));
    QVERIFY(!dlg->button(Dui::DiscardButton));
    QVERIFY(!dlg->button(Dui::HelpButton));
    QVERIFY(!dlg->button(Dui::ApplyButton));
    QVERIFY(!dlg->button(Dui::RestoreDefaultsButton));
}

void Ut_DuiDialog::initVals()
{
    QCOMPARE(dialog->clickedButton(), (DuiButtonModel *)NULL);
}

void Ut_DuiDialog::settersAndGetters()
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
        dialog->setWindowModal(val);
        QCOMPARE(dialog->isWindowModal(), val);
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
        DuiButton *button1 = new DuiButton();
        DuiButton *button2 = new DuiButton();

        dialog->setCentralWidget(button1);
        QCOMPARE(dialog->centralWidget(), button1);
        dialog->setCentralWidget(button2);
        QCOMPARE(dialog->centralWidget(), button2);
    }
}

void Ut_DuiDialog::addStandardButtons()
{
    DuiButtonModel *b1 = dialog->addButton(Dui::AbortButton);
    DuiButtonModel *b2 = dialog->addButton(Dui::OkButton);
    DuiButtonModel *b4 = dialog->addButton(Dui::NoButton);

    QCOMPARE(dialog->button(Dui::AbortButton), b1);
    QCOMPARE(dialog->button(Dui::OkButton), b2);
    QCOMPARE(dialog->button(Dui::NoButton), b4);
    QVERIFY(dialog->button(Dui::YesToAllButton) == 0);
}

void Ut_DuiDialog::addInvalidStandardButton()
{
    DuiButtonModel *buttonModel = dialog->addButton(Dui::NoStandardButton);
    QVERIFY(buttonModel == NULL);
}

void Ut_DuiDialog::addExistingStandardButton()
{
    DuiButtonModel *b1 = dialog->addButton(Dui::OkButton);
    DuiButtonModel *b2 = dialog->addButton(Dui::OkButton);

    QCOMPARE(dialog->button(Dui::OkButton), b1);
    QCOMPARE(dialog->button(Dui::OkButton), b2);
    QVERIFY(b1 == b2);
}

void Ut_DuiDialog::acceptDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    DuiButtonModel *b1 = dialog->addButton("test button 1");
    dialog->addButton("test button 2");
    b1->click();

    QCOMPARE(spyChanged1.count(), 1);
    QCOMPARE(spyChanged2.count(), 0);
    QCOMPARE(spyChanged3.count(), 1);
}

void Ut_DuiDialog::removeButton()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));

    DuiButtonModel *b1 = dialog->addButton("test button 1");
    b1->increaseReferenceCount(); // otherwise it will get deleted on removeButon() call.
    dialog->addButton("test button 2");

    dialog->removeButton(b1);
    b1->click();
    delete b1;
    b1 = 0;

    QCOMPARE(spyChanged1.count(), 0);
}

void Ut_DuiDialog::rejectDialog()
{
    QSignalSpy spyChanged1(dialog, SIGNAL(accepted()));
    QSignalSpy spyChanged2(dialog, SIGNAL(rejected()));
    QSignalSpy spyChanged3(dialog, SIGNAL(finished(int)));

    dialog->addButton("test button 1");
    DuiButtonModel *b2 = dialog->addButton(Dui::CancelButton);
    b2->click();

    QCOMPARE(spyChanged1.count(), 0);
    QCOMPARE(spyChanged2.count(), 1);
    QCOMPARE(spyChanged3.count(), 1);
}

DuiApplication *Ut_DuiDialog::buildApp(int count, const QString &params)
{
    QChar sep(' ');
    char *argv[MAX_PARAMS];
    int x = 0;

    QStringList list = params.split(sep);
    QStringListIterator it(list);
    while (it.hasNext() && x < MAX_PARAMS)  {
        argv[x++] = strdup(it.next().toLocal8Bit().constData());
    }
    return new DuiApplication(count, argv);
}

void Ut_DuiDialog::dismissDialog()
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

QTEST_APPLESS_MAIN(Ut_DuiDialog);
