/***************************************************************************
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

#include "emailservice.h"
#include <QtDebug>
#include <QString>

EmailService::EmailService()
    : QObject(0)
{
    qDebug() << "EmailService is created!";
}

EmailService::~EmailService()
{
}

QString EmailService::editSend(const QString &message)
{
    Q_UNUSED(message);

    // this is a dummy - real code is implemented in eternia
    QString mirror;
    return mirror;
}
