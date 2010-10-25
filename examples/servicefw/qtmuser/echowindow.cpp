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
#include <QGridLayout>

#include <QServiceManager>
#include <QServiceFilter>

#include "echowindow.h"

EchoWindow::EchoWindow():
    manager( new QServiceManager( this ) )

{
    createGUI();
    setLayout(layout);
    setWindowTitle("Contacts Interface Demo");

    // instantiate the interface
    interface = manager->loadInterface( "com.nokia.maemo.meegotouch.ContactsInterface" );

    // check the interface is valid
    if (!interface) {
        disableButton(QString(), QService::UserScope);
        statusBar->showMessage("Err: Service unavailable");
    }

    // example of how to get a list of all installed services
    {
        QStringList services;

        {
            QList<QServiceInterfaceDescriptor> list = manager->findInterfaces();
            for ( int i=0; i<list.size(); ++i ) {
                QServiceInterfaceDescriptor thisItem = list[i];
                services << thisItem.serviceName();
            }
        }

        QStringListIterator iterator(services);
        qDebug() << "All services:";
        while (iterator.hasNext())
            qDebug() << iterator.next().toLocal8Bit().constData();
    }

    // example of how to get a list of all services for a particular interface
    {
        QStringList services;

        {
            QServiceFilter filter;
            filter.setInterface( "com.nokia.maemo.meegotouch.ContactsInterface" );
            QList<QServiceInterfaceDescriptor> list = manager->findInterfaces( filter );
            for ( int i=0; i<list.size(); ++i ) {
                services << list[i].serviceName();
            }
        }

        QStringListIterator iterator(services);
        qDebug() << "All services for com.nokia.maemo.meegotouch.ContactsInterface:";
        while (iterator.hasNext())
            qDebug() << iterator.next().toLocal8Bit().constData();
    }

    // connect the availability signals
    // you likely need to know when a service appears/disappears to
    // do things like add/remove an option on a menu
    connect(
        manager, SIGNAL(serviceAdded(const QString&, QService::Scope)),
        this, SLOT(enableButton(QString, QService::Scope))
    );
    connect(
        manager, SIGNAL(serviceRemoved(const QString&, QService::Scope)),
        this, SLOT(disableButton(QString, QService::Scope))
    );
    /* ug - no equivalent signal in QtmSFW
    connect(
        interface, SIGNAL(serviceChanged(QString)),
        this, SLOT(enableTextProcessing(QString))
    );
    */

    if ( interface ) {
        qDebug() << "connecting contactsPicked() with processPickedContacts()";

        connect(
                interface, SIGNAL( contactsPicked( const QVariantList&, const QString& ) ),
                this, SLOT( processPickedContacts( const QVariantList&, const QString& ) )
                );
    } else {
        qDebug() << "interface invalid, so not connecting";
    }

}

EchoWindow::~EchoWindow()
{
    delete interface;
}

void EchoWindow::processPickedContacts( const QVariantList &contacts, const QString &clientID )
{
    QString textToDisplay = clientID+":";

    if ( contacts.count() != 2 ) {
        textToDisplay += "contacts not 2 - expected 2";
    } else {
        uint    contactData0 = contacts.at(0).toInt();
        QString contactData1 = contacts.at(1).toString();

        textToDisplay += QString::number(contactData0)+":"+contactData1;
    }

    label->setText( textToDisplay );
}

void EchoWindow::sendEcho()
{
    // call the interface method to reverse the string
    QString text;
    bool success = QMetaObject::invokeMethod( interface,
            "openContactPicker",
            Q_ARG( bool, true ), // singleSelection
            Q_ARG( int, 0 ),    // filter
            Q_ARG( int, 0 ),    // sorting
            Q_ARG( const uint, effectiveWinId() ), // window id
            Q_ARG( const QString, "WINDOW NAME" ) // window name
            );

    if ( ! success ) {
        text = "invokeMethod failed";
    } else {
        text = "Called openContactPicker method";
    }

    // set the label text to the reversed string
    label->setText( text );
}

/*
 * This function is specific to this example.
 * Your own code will likely do something different
 */
void EchoWindow::createGUI()
{
    label = new QLabel(this);
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    button = new QPushButton(tr("Open Contact List"));
    statusBar = new QStatusBar(this);

    connect(button, SIGNAL(clicked()),
            this, SLOT(sendEcho()));

    layout = new QGridLayout(this);
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
void EchoWindow::enableButton(const QString &service, QService::Scope scope)
{
    Q_UNUSED( scope );
    statusBar->showMessage(service + " is available");
    button->setEnabled(true);
}

/*
 * This function is specific to this example.
 * Your own code will likely do something different
 */
void EchoWindow::disableButton(const QString &service, QService::Scope scope)
{
    Q_UNUSED( scope );
    statusBar->showMessage("Err: " + service + " is unavailable");
    label->setEnabled(false);
    button->setEnabled(false);
}
