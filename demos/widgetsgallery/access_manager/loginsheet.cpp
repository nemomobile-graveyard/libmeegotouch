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
#include <MTextEdit>

#include <QAction>
#include <QGraphicsLinearLayout>

LoginSheet::LoginSheet()
{
    fakeAuthenticationTimer.setSingleShot(true);
    fakeAuthenticationTimer.setInterval(2000);
    connect(&fakeAuthenticationTimer, SIGNAL(timeout()), SLOT(dismiss()));

    userNameTextEdit = 0;
    successful = false;

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

    MLabel *label = new MLabel("Connect to Service");
    label->setObjectName("connectLabel");
    label->setStyleName("CommonTitle");
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addItem(label);

    mainLayout->addItem(createSpacer());

    label = new MLabel("Username:");
    label->setObjectName("usernameLabel");
    label->setStyleName("CommonFieldLabel");

    mainLayout->addItem(label);

    userNameTextEdit = new MTextEdit;
    userNameTextEdit->setObjectName("userNameTextEdit");
    userNameTextEdit->setStyleName("CommonSingleInputFieldLabeled");
    mainLayout->addItem(userNameTextEdit);

    mainLayout->addItem(createSpacer());

    label = new MLabel("Password:");
    label->setObjectName("PasswordLabel");
    label->setStyleName("CommonFieldLabel");
    mainLayout->addItem(label);

    MTextEdit *textEdit = new MTextEdit;
    textEdit->setObjectName("passwordTextEdit");
    textEdit->setStyleName("CommonSingleInputFieldLabeled");
    textEdit->setEchoMode(MTextEditModel::Password);
    mainLayout->addItem(textEdit);

    mainLayout->addStretch();
}

void LoginSheet::createHeaderWidget()
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(this);
    basicHeader->setObjectName("basicSheetHeader");

    basicHeader->setNegativeAction(new QAction("Cancel", basicHeader));
    basicHeader->setPositiveAction(new QAction("Connect", basicHeader));

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
    successful = true;
}

void LoginSheet::cancel()
{
    successful = false;

    if (fakeAuthenticationTimer.isActive()) {
        fakeAuthenticationTimer.stop();
    }

    dismiss();
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
