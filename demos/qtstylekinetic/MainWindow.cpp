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

#include <QPushButton>
#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow() : QMainWindow(NULL)
{
    setupUi(this);

    QWidget* w = new QWidget();
    w->setLayout(new QVBoxLayout);
    for(int i = 0; i < 100; ++i)
        w->layout()->addWidget(new QPushButton(QString("Button %1").arg(i)));
    scrollArea->setWidget(w);

    for(int i = 0; i < 100; ++i)
        new QListWidgetItem(QString("Item %1").arg(i), listWidget);

    for(int row = 0; row < 100; ++row)
        for(int col = 0; col < 100; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(QString("%1").arg(row*col));
            tableWidget->setItem(row, col, item);
        }
}

MainWindow::~MainWindow()
{

}
