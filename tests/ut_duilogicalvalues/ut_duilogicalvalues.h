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

#ifndef UT_DUILOGICALVALUES_H
#define UT_DUILOGICALVALUES_H

#include <QObject>
#include "../../src/corelib/theme/duilogicalvalues.h"

class Ut_DuiLogicalValues : public QObject
{
    Q_OBJECT
public:
    Ut_DuiLogicalValues();

private slots:
    void init();
    void cleanup();

    void testFontsAndColors();

private:

    DuiLogicalValues      *m_subject;
};

#endif

