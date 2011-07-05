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

#include "accessmanagerimpl.h"
#include "loginsheet.h"
#include <QEventLoop>

AccessManagerImpl::AccessManagerImpl(QObject *parent) :
    QObject(parent)
{
}

AccessManagerImpl::~AccessManagerImpl()
{
}

bool AccessManagerImpl::login(bool fullscreen, bool autofocus, int orientation)
{
    LoginSheet* sheet = new LoginSheet();
    sheet->setStatusBarVisibleInSystemwide(!fullscreen);
    sheet->setAutoFocusOnFirstTextEditEnabled(autofocus);
    sheet->setSystemwideModeOrientation(static_cast<MSheet::SystemwideModeOrientation>(orientation));

    sheet->appearSystemwide(MSceneWindow::DestroyWhenDone);

    QEventLoop eventLoop;
    eventLoop.connect(sheet, SIGNAL(disappeared()), SLOT(quit()));
    eventLoop.exec();

    return sheet->isLoginSuccessful();
}
