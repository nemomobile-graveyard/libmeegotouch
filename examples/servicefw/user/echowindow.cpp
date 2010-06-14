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

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <textprocessorinterface.h>

#include "echowindow.h"

EchoWindow::EchoWindow()
{
    createGUI();
    setLayout(layout);
    setWindowTitle("TextProcessor Demo");

    // instantiate the interface
    textProcessorInterface = new TextProcessorInterface();

    // check the interface is valid
    if (!textProcessorInterface->isValid()) {
        disableTextProcessing(QString());
        statusBar->showMessage("Err: Service unavailable");
    }

    // example of how to get a list of all installed services
    if (textProcessorInterface->isValid()) {
        QStringList services = textProcessorInterface->serviceNames();
        QStringListIterator iterator(services);
        qDebug() << "All services:";
        while (iterator.hasNext())
            qDebug() << iterator.next().toLocal8Bit().constData();
    }

    // example of how to get a list of all services for a particular interface
    if (textProcessorInterface->isValid()) {
        QStringList services = textProcessorInterface->serviceNames("com.nokia.TextProcessorInterface");
        QStringListIterator iterator(services);
        qDebug() << "Services implementing com.nokia.TextProcessorInterface:";
        while (iterator.hasNext())
            qDebug() << iterator.next().toLocal8Bit().constData();
    }

    // connect the availability signals
    // you likely need to know when a service appears/disappears to
    // do things like add/remove an option on a menu
    connect(
        textProcessorInterface, SIGNAL(serviceAvailable(QString)),
        this, SLOT(enableTextProcessing(QString))
    );
    connect(
        textProcessorInterface, SIGNAL(serviceUnavailable(QString)),
        this, SLOT(disableTextProcessing(QString))
    );
    connect(
        textProcessorInterface, SIGNAL(serviceChanged(QString)),
        this, SLOT(enableTextProcessing(QString))
    );
}

EchoWindow::~EchoWindow()
{
    delete textProcessorInterface;
}

void EchoWindow::sendEcho()
{
    // call the interface method to reverse the string
    QString text = textProcessorInterface->reverse(lineEdit->text());

    // set the label text to the reversed string
    label->setText(text);
}

/*
 * This function is specific to this example.
 * Your own code will likely do something different
 */
void EchoWindow::createGUI()
{
    lineEdit = new QLineEdit;
    label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    button = new QPushButton(tr("Send Message"));
    statusBar = new QStatusBar( this );

    connect(lineEdit, SIGNAL(editingFinished()),
            this, SLOT(sendEcho()));
    connect(button, SIGNAL(clicked()),
            this, SLOT(sendEcho()));

    layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Message:")), 0, 0);
    layout->addWidget(lineEdit, 0, 1);
    layout->addWidget(new QLabel(tr("Answer:")), 1, 0);
    layout->addWidget(label, 1, 1);
    layout->addWidget(button, 2, 1, Qt::AlignRight);
    layout->addWidget(statusBar, 3, 0, 1, 2);
    layout->setSizeConstraint(QLayout::SetFixedSize);
}

/*
 * This function is specific to this example.
 * Your own code will likely do something different
 */
void EchoWindow::enableTextProcessing(const QString &service)
{
    statusBar->showMessage(service + " is available");
    lineEdit->setEnabled(true);
    label->setEnabled(true);
    button->setEnabled(true);
}

/*
 * This function is specific to this example.
 * Your own code will likely do something different
 */
void EchoWindow::disableTextProcessing(const QString &service)
{
    statusBar->showMessage("Err: " + service + " is unavailable");
    lineEdit->setEnabled(false);
    label->setEnabled(false);
    button->setEnabled(false);
}
