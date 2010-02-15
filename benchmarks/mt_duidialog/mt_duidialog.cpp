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

#include "memorybenchmark.h"
#include "memorybenchmark.inl"
#include "mt_duidialog.h"
#include <QPainter>
#include <QPixmap>
#include <DuiApplication>
#include <DuiTheme>
#include <DuiDialog>
#include <DuiQueryDialog>
#include <DuiMessageBox>

DuiApplication *app;

void Mt_DuiDialog::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./widgetsgallery";
    argv[0] = appName;
    app = new DuiApplication(argc, (char **)argv);
    DuiTheme::instance()->changeTheme("common");

    // We need to ensure that QTs subsystems are up and running
    // and do not affect to our benchmark
    QPixmap pixmap(100, 30);
    QPainter painter(&pixmap);
    DuiDialog *dialog = new DuiDialog();
    dialog->appearNow();
    dialog->paint(&painter, NULL);
    delete dialog;
}

void Mt_DuiDialog::cleanupTestCase()
{
    delete app;
}

void Mt_DuiDialog::testPlainDialog_data()
{
}

void Mt_DuiDialog::testPlainDialog()
{
    /*
    We want to measure how much memory will be used by dialog
    when it's painted on a screen (buffer). We don't take into
    account pixmap and painter. Also since graphics resource
    loading is asynchronous we need to make sure that DuiTheme
    daemon loaded all pictures.
    */
    QPixmap pixmap(DuiDeviceProfile::instance()->resolution());
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    DuiDialog *dialog = new DuiDialog;
    dialog->appearNow();

    dialog->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release
    delete dialog;

    QCoreApplication::processEvents();

    endMemoryBenchmark();
}

void Mt_DuiDialog::testQueryDialog_data()
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
void Mt_DuiDialog::testQueryDialog()
{
    QFETCH(int, buttonCount);

    QPixmap pixmap(DuiDeviceProfile::instance()->resolution());
    QPainter painter(&pixmap);

    DuiButton **buttons = new DuiButton*[buttonCount];

    beginMemoryBenchmark();

    DuiQueryDialog *dialog = new DuiQueryDialog("Test");

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


void Mt_DuiDialog::testMessagebox_data()
{
    QTest::addColumn<int>("buttons");

    QTest::newRow("no buttons")  << (int)(DuiMessageBoxModel::NoButton);
    QTest::newRow("ok") << (int)(DuiMessageBoxModel::Ok);
    QTest::newRow("ok & cancel") << (int)(DuiMessageBoxModel::Ok | DuiMessageBoxModel::Cancel);
    QTest::newRow("ok & cancel & save") << (int)(DuiMessageBoxModel::Ok | DuiMessageBoxModel::Cancel | DuiMessageBoxModel::Save);
    QTest::newRow("ok & cancel & save & discard") << (int)(DuiMessageBoxModel::Ok | DuiMessageBoxModel::Cancel | DuiMessageBoxModel::Save | DuiMessageBoxModel::Discard);
}

void Mt_DuiDialog::testMessagebox()
{
    QFETCH(int, buttons);
    /*
    We want to measure how much memory will be used by dialog
    when it's painted on a screen (buffer). We don't take into
    account pixmap and painter. Also since graphics resource
    loading is asynchronous we need to make sure that DuiTheme
    daemon loaded all pictures.
    */
    QPixmap pixmap(DuiDeviceProfile::instance()->resolution());
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    DuiMessageBox *mb = new DuiMessageBox("Test messageBox", (DuiMessageBoxModel::Button) buttons);
    mb->appearNow();

    mb->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    // Release
    delete mb;

    QCoreApplication::processEvents();

    endMemoryBenchmark();
}

QTEST_APPLESS_MAIN(Mt_DuiDialog)
