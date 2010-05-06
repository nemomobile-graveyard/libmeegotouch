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

#ifndef KEYPRESSWAITER_H
#define KEYPRESSWAITER_H

#include <QApplication>
#include <QThread>
#include <QTextStream>
#include <QFile>
#include <QtDebug>

class KeyPressWaiter : public QThread
{
    Q_OBJECT
public:

    void run() {
        qDebug() << "Hit enter to exit..";
        QTextStream qin(stdin, QFile::ReadOnly);
        qin.readLine();
    }
};

#endif
