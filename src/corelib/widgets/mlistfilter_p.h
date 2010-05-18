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

#ifndef MLISTFILTER_P_H
#define MLISTFILTER_P_H

#include <QObject>
#include <QPointF>
#include "mlistfilter.h"

class MPannableViewport;

class MListFilterPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MListFilter)
    
public:
    MListFilterPrivate();
    virtual ~MListFilterPrivate();
    void init();

public Q_SLOTS:
    void panningStarted();
    void viewportPositionChanged(const QPointF& pos);

private:
    MPannableViewport* pannableViewport();

private:
    QPointF viewportPos;
    QPointF panningStartPos;
    MPannableViewport *cachedPannableViewport;
    MListFilter *q_ptr;
};

#endif
