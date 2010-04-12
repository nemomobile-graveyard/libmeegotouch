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

#include "memorybenchmark.h"
#include "memorybenchmark.inl"
#include "mt_mdialog.h"
#include <QPainter>
#include <QPixmap>
#include <MApplication>
#include <MTheme>
#include <MDialog>
#include <MQueryDialog>
#include <MMessageBox>

MApplication *app;

void Mt_MDialog::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./widgetsgallery";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
    MTheme::instance()->changeTheme("common");

    // We need to ensure that QTs subsystems are up and running
    // and do not affect to our benchmark
    QPixmap pixmap(100, 30);
    QPainter painter(&pixmap);
    MDialog *dialog = new MDialog();
    dialog->appearNow();
    dialog->paint(&painter, NULL);
    delete dialog;
}

void Mt_MDialog::cleanupTestCase()
{
    delete app;
}

void Mt_MDialog::testPlainDialog_data()
{
}

void Mt_MDialog::testPlainDialog()
{
    /*
    We want to measure how much memory will be used by dialog
    when it's painted on a screen (buffer). We don't take into
    account pixmap and painter. Also since graphics resource
    loading is asynchronous we need to make sure that MTheme
    daemon loaded all pictures.
    */
    QPixmap pixmap(MDeviceProfile::instance()->resolution());
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    MDialog *dialog = new MDialog;
    dialog->appearNow();

    dialog->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release
    delete dialog;

    QCoreApplication::processEvents();

    endMemoryBenchmark();
}

void Mt_MDialog::testQueryDialog_data()
{
    QTest::addColumn<int>("buttonCount");

    QTest::newRow("1 buttons")  << 1;
    QTest::newRow("2 button")   << 2;
    QTest::newRow("5 buttons")  << 5;
    QTest::newRow("10 buttons") << 10;
    QTest::newRow("20 buttons") << 20;
    QTest::newRow("50 buttons") << 50;
    QTest::newRow("100 buttons") << 100;

}
void Mt_MDialog::testQueryDialog()
{
    QFETCH(int, buttonCount);

    QPixmap pixmap(MDeviceProfile::instance()->resolution());
    QPainter painter(&pixmap);

    MButton **buttons = new MButton*[buttonCount];

    beginMemoryBenchmark();

    MQueryDialog *dialog = new MQueryDialog("Test");

    for (int i = 0; i < buttonCount; ++i) {
        buttons[i] = dialog->addButton(QString("Button ") + QString::number(i));
    }
    dialog->appearNow();

    dialog->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release
    delete dialog;

    QCoreApplication::processEvents();

    endMemoryBenchmark();

    delete[] buttons;
}


void Mt_MDialog::testMessagebox_data()
{
    QTest::addColumn<int>("buttons");

    QTest::newRow("no buttons")  << (int)(MMessageBoxModel::NoButton);
    QTest::newRow("ok") << (int)(MMessageBoxModel::Ok);
    QTest::newRow("ok & cancel") << (int)(MMessageBoxModel::Ok | MMessageBoxModel::Cancel);
    QTest::newRow("ok & cancel & save") << (int)(MMessageBoxModel::Ok | MMessageBoxModel::Cancel | MMessageBoxModel::Save);
    QTest::newRow("ok & cancel & save & discard") << (int)(MMessageBoxModel::Ok | MMessageBoxModel::Cancel | MMessageBoxModel::Save | MMessageBoxModel::Discard);
}

void Mt_MDialog::testMessagebox()
{
    QFETCH(int, buttons);
    /*
    We want to measure how much memory will be used by dialog
    when it's painted on a screen (buffer). We don't take into
    account pixmap and painter. Also since graphics resource
    loading is asynchronous we need to make sure that MTheme
    daemon loaded all pictures.
    */
    QPixmap pixmap(MDeviceProfile::instance()->resolution());
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    MMessageBox *mb = new MMessageBox("Test messageBox", (MMessageBoxModel::Button) buttons);
    mb->appearNow();

    mb->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release
    delete mb;

    QCoreApplication::processEvents();

    endMemoryBenchmark();
}

QTEST_APPLESS_MAIN(Mt_MDialog)
