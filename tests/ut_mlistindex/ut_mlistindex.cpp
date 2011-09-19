/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_mlistindex.h"
#include "mlistindex.h"

#include <MApplication>

#include <QSignalSpy>

MApplication *app = NULL;

void Ut_MListIndex::init()
{
    m_subject = new MListIndex();
}

void Ut_MListIndex::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MListIndex::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mlistindex" };
    app = new MApplication(argc, app_name);
}

void Ut_MListIndex::cleanupTestCase()
{
    delete app;
}

void Ut_MListIndex::testListConnections()
{
    QSignalSpy groupTitlesChangedSpy(m_subject, SIGNAL(groupTitlesChanged()));
    m_subject->setList(NULL);
    m_subject->updateGroupTitles();

    QCOMPARE(groupTitlesChangedSpy.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_MListIndex)
