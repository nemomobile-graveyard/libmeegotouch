/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "loginsheet.h"

#include <MApplication>
#include <MBasicSheetHeader>
#include <MLabel>
#include <MMessageBox>
#include <MTextEdit>

#include <QAction>
#include <QGraphicsLinearLayout>

LoginSheet::LoginSheet()
{
    fakeAuthenticationTimer.setSingleShot(true);
    fakeAuthenticationTimer.setInterval(2000);
    connect(&fakeAuthenticationTimer, SIGNAL(timeout()), SLOT(showLoginSuccessfulAndDismiss()));

    userNameTextEdit = 0;
    messageBox = 0;

    createCentralWidget();
    createHeaderWidget();

    autoFocusOnFirstTextEdit = false;
}

void LoginSheet::setAutoFocusOnFirstTextEditEnabled(bool enabled)
{
    if (enabled == autoFocusOnFirstTextEdit)
        return;

    autoFocusOnFirstTextEdit = enabled;

    bool ok;

    if (autoFocusOnFirstTextEdit) {
        ok = connect(this, SIGNAL(appeared()), SLOT(setfocusOnUsernameTextEdit()));
        if (!ok) qFatal("signal connection failed");
    } else {
        disconnect(SIGNAL(appeared()), this, SLOT(setfocusOnUsernameTextEdit()));
    }
}

void LoginSheet::createCentralWidget()
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical,
                                                                  centralWidget());
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    //% "Connect to Service"
    MLabel *label = new MLabel(qtTrId("xx_wg_sheets_connect_service"));
    label->setObjectName("connectLabel");
    label->setStyleName(inv("CommonTitle"));
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addItem(label);

    mainLayout->addItem(createSpacer());

    //% "Username:"
    label = new MLabel(qtTrId("xx_wg_sheets_username"));
    label->setObjectName("usernameLabel");
    label->setStyleName(inv("CommonFieldLabel"));

    mainLayout->addItem(label);

    userNameTextEdit = new MTextEdit;
    userNameTextEdit->setObjectName("userNameTextEdit");
    userNameTextEdit->setStyleName(inv("CommonSingleInputFieldLabeled"));
    mainLayout->addItem(userNameTextEdit);

    mainLayout->addItem(createSpacer());

    //% "Password:"
    label = new MLabel(qtTrId("xx_wg_sheets_password"));
    label->setObjectName("PasswordLabel");
    label->setStyleName(inv("CommonFieldLabel"));
    mainLayout->addItem(label);

    MTextEdit *textEdit = new MTextEdit;
    textEdit->setObjectName("passwordTextEdit");
    textEdit->setStyleName(inv("CommonSingleInputFieldLabeled"));
    textEdit->setEchoMode(MTextEditModel::Password);
    mainLayout->addItem(textEdit);

    mainLayout->addStretch();
}

void LoginSheet::createHeaderWidget()
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(this);
    basicHeader->setObjectName("basicSheetHeader");
    basicHeader->setStyleName(inv(""));

    //% "Cancel"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_cancel"), basicHeader));
    //% "Connect"
    basicHeader->setPositiveAction(new QAction(qtTrId("xx_wg_sheets_connect"), basicHeader));

    connect(basicHeader->negativeAction(), SIGNAL(triggered()), SLOT(cancel()));
    connect(basicHeader->positiveAction(), SIGNAL(triggered()), SLOT(login()));

    setHeaderWidget(basicHeader);
}

void LoginSheet::login()
{
    MBasicSheetHeader *header = static_cast<MBasicSheetHeader*>(headerWidget());

    header->setProgressIndicatorVisible(true);
    header->positiveAction()->setEnabled(false);
    centralWidget()->setEnabled(false);

    // fake that we're waiting for authentication via network
    Q_ASSERT(!fakeAuthenticationTimer.isActive());
    fakeAuthenticationTimer.start();
}

void LoginSheet::cancel()
{
    if (fakeAuthenticationTimer.isActive()) {
        fakeAuthenticationTimer.stop();
    }

    /* Dismiss the page only if the success message box hasn't been created yet. If the
       sheet is system wide, dismissing the page will destroy the messagebox, which may
       lead to a crash */
    if (!messageBox) {
        dismiss();
    }
}

void LoginSheet::showLoginSuccessfulAndDismiss()
{
    MBasicSheetHeader *header = static_cast<MBasicSheetHeader*>(headerWidget());

    header->setProgressIndicatorVisible(false);

    messageBox = new MMessageBox;
    messageBox->setObjectName("messageBox");

    //% "Access Granted!"
    messageBox->setTitle(qtTrId("xx_wg_sheets_connection_success"));
    //% "You are now able to use the requested services."
    messageBox->setText(qtTrId("xx_wg_sheets_connection_success_text"));

    connect(messageBox, SIGNAL(disappeared()), SLOT(dismiss()));
    messageBox->appear(MSceneWindow::DestroyWhenDone);
}

void LoginSheet::setfocusOnUsernameTextEdit()
{
    userNameTextEdit->setFocus();
}

QGraphicsWidget *LoginSheet::createSpacer() const
{
    QGraphicsWidget *spacer = new QGraphicsWidget;
    spacer->setObjectName("spacer");
    spacer->setFlag(QGraphicsItem::ItemHasNoContents);
    spacer->setPreferredHeight(24.0);
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    return spacer;
}

QString LoginSheet::inv(QString stylename)
{
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        return stylename;
    } else {
        return stylename.append("Inverted");
    }
}
