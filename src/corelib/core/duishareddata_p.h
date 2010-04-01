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

#ifndef DUISHARED_DATA_P_H
#define DUISHARED_DATA_P_H

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QSharedMemory>

class DuiSharedData;

class DuiSharedDataPrivate
{
    Q_DECLARE_PUBLIC(DuiSharedData)

public:
    QSharedMemory *shm;
    QByteArray dataByteArray;
    QBuffer dataBuffer;
    QDataStream dataStream;
    qint64 oldPos;

    QSharedMemory *allocateNewPage();
    QSharedMemory *allocateNewPage(int num);

    QSharedMemory *attachToPage(int num);

    void flushPage();
    bool pageOverflown();
    void nextPage();
    void savePos();

    void setKey(const QString &key);

    int currentPageOffset;
    int nextPageOffset;
    int currentPageNum;
    QSharedMemory *currentPage;

    int mode;

    int pageSize;
    QString key;
    QList<QSharedMemory *> ownedPages;

    QString errorString;

protected:
    DuiSharedData *q_ptr;
};

#endif

