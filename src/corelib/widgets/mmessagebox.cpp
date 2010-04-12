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

#include "mmessagebox.h"
#include "mmessagebox_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MMessageBox)

MMessageBoxPrivate::MMessageBoxPrivate()
{
}

MMessageBox::MMessageBox(const QString &text, M::StandardButtons buttons)
    : MDialog(new MMessageBoxPrivate, buttons, new MMessageBoxModel, MSceneWindow::MessageBox)
{
    model()->setText(text);

    setCentralWidget(0);
    setTitleBarVisible(false);
}

MMessageBox::MMessageBox(const QString &title, const QString &text, M::StandardButtons buttons)
    : MDialog(new MMessageBoxPrivate, buttons, new MMessageBoxModel, MSceneWindow::MessageBox)
{
    setTitle(title);
    model()->setText(text);

    setCentralWidget(0);
}

MMessageBox::~MMessageBox()
{
}

QString MMessageBox::text() const
{
    return model()->text();
}

void MMessageBox::setText(const QString &text)
{
    model()->setText(text);
}
