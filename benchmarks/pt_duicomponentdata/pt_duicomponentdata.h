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

#ifndef PT_DuiComponentData_H
#define PT_DuiComponentData_H

#include <QtTest/QtTest>
#include <QObject>

class DuiComponentData;
class DuiWidgetView;

/**
  * DuiApplication constructor spends most of its runtime creating a
  * DuiComponentData object. This test benchmarks the DuiComponentData
  * creation.
  */
class Pt_DuiComponentData : public QObject
{
    Q_OBJECT

private slots:
    void constructor();
};

#endif
