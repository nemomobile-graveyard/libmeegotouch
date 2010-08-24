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

#ifndef ECHODIALOG_H
#define ECHODIALOG_H

#include <QWidget>
#include <QStatusBar>
#include <QObject>

#include <qmobilityglobal.h>
#include <QServiceManager>

QT_BEGIN_NAMESPACE
class QString;
class QLineEdit;
class QLabel;
class QPushButton;
class QGridLayout;
QT_END_NAMESPACE

QTM_BEGIN_NAMESPACE
class QServiceManager;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

/*
 * Example class to demonstrate use of service framework
 */
class EchoWindow : public QWidget
{
    Q_OBJECT

public:
    EchoWindow();
    virtual ~EchoWindow();

private slots:
    void processPickedContacts( const QVariantList &contacts, const QString &clientID );
    void sendEcho();
    void enableButton(const QString &service, QService::Scope scope);
    void disableButton(const QString &service, QService::Scope scope);

private:
    void createGUI();

    QServiceManager *manager;

    QObject *interface;
    QLabel *label;
    QPushButton *button;
    QStatusBar *statusBar;
    QGridLayout *layout;
};

#endif
