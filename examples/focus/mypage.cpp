/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QGraphicsLinearLayout>
#include <MTextEdit>
#include <MButton>
#include <MDebug>
#include <MAction>

#include "mypage.h"
#include "mydialog.h"

MyPage::MyPage()
{
    setTitle("Focus example");

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(centralWidget());

    textEdit = new MTextEdit;
    textEdit->setPrompt("Click to focus");
    layout->addItem(textEdit);

    MButton *button = new MButton("Open dialog");
    layout->addItem(button);

    MyDialog *dialog = new MyDialog();

    QObject::connect(button, SIGNAL(clicked()), dialog, SLOT(appear()));

    MAction *action = new MAction("Hello!", this);
    action->setLocation(MAction::ApplicationMenuLocation);
    addAction(action);
}

bool MyPage::event(QEvent *e)
{
    if (e->type() == QEvent::WindowBlocked) {
        mDebug("MyPage") << "Help! Help! I'm Being Oppressed!";
    }
    if (e->type() == QEvent::WindowUnblocked) {
        textEdit->setFocus();
    }

    return MApplicationPage::event(e);
}
