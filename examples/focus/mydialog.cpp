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
#include "mydialog.h"

#include <QGraphicsLinearLayout>
#include <MTextEdit>
#include <MLabel>

MyDialog::MyDialog()
    : MDialog("Dialogs are neat!", 0)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, centralWidget());
    MLabel *label = new MLabel("Don't you agree?");
    textEdit =  new MTextEdit(MTextEditModel::SingleLine);
    layout->addItem(label);
    layout->addItem(textEdit);

    addButton("Undoubtedly!");
    addButton("Meh...");

    connect(this, SIGNAL(appeared()), this, SLOT(focusTextEntry()));
}

void MyDialog::focusTextEntry()
{
    textEdit->setFocus();
}
