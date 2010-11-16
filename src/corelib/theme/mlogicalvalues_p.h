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

#ifndef MLOGICALVALUES_P_H
#define MLOGICALVALUES_P_H

#include <QHash>
#include <QList>

class QFileInfo;
class QByteArray;

typedef QHash<QByteArray, QByteArray> Values;
typedef QHash<QByteArray, Values> Groups;

class MLogicalValuesPrivate
{
public:
    bool parse(const QFileInfo &fileInfo, Groups &groups);
    bool loadFromBinaryCache(const QFileInfo &fileInfo, Groups &groups);
    bool saveToBinaryCache(const QFileInfo &fileInfo, const Groups &groups) const;
    QString createBinaryFilename(const QFileInfo &fileInfo) const;
    void mergeGroups(const Groups &groups);
    Groups data;
    QList<uint> timestamps;
};

#endif
