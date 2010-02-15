/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duiactionprovider.h"
#include "duiserviceaction.h"

#include <DuiDebug>

/***** TODO: remove test action when not needed any more ******/
TestAction::TestAction(const QString &text) :
    DuiServiceAction(NULL),
    text(text)
{
}

TestAction::~TestAction()
{
}

void TestAction::executeService()
{
    duiDebug("TestAction") << text;
}
/***** TODO: test action ends here ******/


DuiAction *DuiActionProvider::getDefaultAction(const QUrl &uri)
{
    if (uri.isValid()) {
        if (uri.scheme() == "settings") {
            return new TestAction(QString().sprintf("Action for opening the settings page %s triggered", uri.host().toUtf8().constData()));
        } else if (uri.path().endsWith(QString(".png"), Qt::CaseInsensitive) || uri.path().endsWith(QString(".jpg"), Qt::CaseInsensitive)) {
            return new TestAction(QString("Default action for Image triggered"));
        }
    }

    return NULL;
}

