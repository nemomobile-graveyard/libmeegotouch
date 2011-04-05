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

#ifndef LOGINSHEET_H
#define LOGINSHEET_H

#include <MSheet>
#include <QTimer>

class MTextEdit;

class LoginSheet : public MSheet
{
    Q_OBJECT

public:
    LoginSheet();

private Q_SLOTS:
    void login();
    void cancel();
    void showLoginSuccessfulAndDismiss();
    void setfocusOnUsernameTextEdit();

private:
    void createCentralWidget();
    void createHeaderWidget();
    QGraphicsWidget *createSpacer() const;

    QTimer fakeAuthenticationTimer;
    MTextEdit *userNameTextEdit;
};

#endif //LOGINSHEET_H