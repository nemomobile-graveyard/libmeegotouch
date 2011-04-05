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

    createCentralWidget();
    createHeaderWidget();

    connect(this, SIGNAL(appeared()), SLOT(setfocusOnUsernameTextEdit()));
}

void LoginSheet::createCentralWidget()
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical,
                                                                  centralWidget());
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    //% "Connect to Service"
    MLabel *label = new MLabel(qtTrId("xx_wg_sheets_connect_service"));
    label->setStyleName("CommonTitle");
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addItem(label);

    mainLayout->addItem(createSpacer());

    //% "Username:"
    label = new MLabel(qtTrId("xx_wg_sheets_username"));
    label->setStyleName("CommonFieldLabel");

    mainLayout->addItem(label);

    userNameTextEdit = new MTextEdit;
    userNameTextEdit->setStyleName("CommonSingleInputFieldLabeled");
    mainLayout->addItem(userNameTextEdit);

    mainLayout->addItem(createSpacer());

    //% "Password:"
    label = new MLabel(qtTrId("xx_wg_sheets_password"));
    label->setStyleName("CommonFieldLabel");
    mainLayout->addItem(label);

    MTextEdit *textEdit = new MTextEdit;
    textEdit->setStyleName("CommonSingleInputFieldLabeled");
    textEdit->setEchoMode(MTextEditModel::Password);
    mainLayout->addItem(textEdit);

    mainLayout->addStretch();
}

void LoginSheet::createHeaderWidget()
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(this);

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
    dismiss();
}

void LoginSheet::showLoginSuccessfulAndDismiss()
{
    MBasicSheetHeader *header = static_cast<MBasicSheetHeader*>(headerWidget());

    header->setProgressIndicatorVisible(false);

    MMessageBox *messageBox = new MMessageBox;

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
    spacer->setFlag(QGraphicsItem::ItemHasNoContents);
    spacer->setPreferredHeight(24.0);
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    return spacer;
}
