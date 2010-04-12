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

#ifndef PT_MComponentData_H
#define PT_MComponentData_H

#include <QtTest/QtTest>
#include <QObject>

class MComponentData;
class MWidgetView;

/**
  * MApplication constructor spends most of its runtime creating a
  * MComponentData object. This test benchmarks the MComponentData
  * creation.
  */
class Pt_MComponentData : public QObject
{
    Q_OBJECT

private slots:
    void constructor();
};

#endif
