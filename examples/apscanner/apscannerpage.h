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

#ifndef APSCANNERPAGE_H
#define APSCANNERPAGE_H

#include <MApplicationPage>
#include "apscanner.h"

class MList;
class APItemModel;

class APScannerPage: public MApplicationPage
{
    Q_OBJECT

public:
    APScannerPage();
    virtual ~APScannerPage();
    virtual void createContent();

public Q_SLOTS:
    void accessPointUpdate(const APScanner::AccessPointInfo &accessPoint);

private:
    MList *list;
    APScanner *scanner;
    APItemModel *model;
};

#endif
