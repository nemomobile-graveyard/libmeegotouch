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

#include "MainWindow.h"

#include <QMenu>
#include <QFileDialog>

MainWindow::MainWindow() : QMainWindow(NULL)
{
    setupUi(this);
    QMenu *menu(menuBar()->addMenu("Long menu"));

    for (int i = 0; i < 200; ++i) {
        menu->addAction(QString("Entry %1").arg(i));
        comboBox->addItem(QString("%1 cm").arg(i * 2));
    }
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));

    statusBar()->showMessage("I am the status bar");
}

MainWindow::~MainWindow()
{

}

void MainWindow::fileOpen()
{
    QString s = QFileDialog::getOpenFileName();
}
