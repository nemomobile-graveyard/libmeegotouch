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

#include "ut_mseparator.h"
#include <mapplication.h>
#include <mseparator.h>

#include <QDebug>

void Ut_MSeparator::initTestCase()
{
    m_separator = new MSeparator;
}

void Ut_MSeparator::cleanupTestCase()
{
    delete m_separator;
}

void Ut_MSeparator::separatorOrientation()
{
    m_separator->setOrientation(Qt::Vertical);
    QCOMPARE(m_separator->orientation(), Qt::Vertical);

    m_separator->setOrientation(Qt::Horizontal);
    QCOMPARE(m_separator->orientation(), Qt::Horizontal);
}

QTEST_APPLESS_MAIN(Ut_MSeparator)

