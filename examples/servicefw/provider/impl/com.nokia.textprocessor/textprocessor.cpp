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

#include "textprocessor.h"
#include <QtDebug>
#include <QString>

TextProcessor::TextProcessor()
    : QObject(0)
{
    qDebug() << "TextProcessor is created!";
}

TextProcessor::~TextProcessor()
{
}

QString TextProcessor::reverse(const QString &message)
{
    qDebug() << message << "reverse to:";

    QString mirror;
    for (int i = message.size() - 1; i >= 0; --i) {
        mirror.append(message[i]);
    }

    qDebug() << mirror;
    return mirror;
}
