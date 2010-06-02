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

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include <mstatusbar.h>

#include "ut_mstatusbarview.h"

Ut_MStatusBarView::Ut_MStatusBarView():
        m_subject(0),
        m_statusbar(0)
{
}

void Ut_MStatusBarView::initTestCase()
{
    m_statusbar = new MStatusBar();
    m_subject= new MStatusBarView(m_statusbar);
}

void Ut_MStatusBarView::cleanupTestCase()
{
    delete m_statusbar;
    m_statusbar = 0;
}
#ifdef Q_WS_X11
void Ut_MStatusBarView::testXDamageWhileTogglingVisibility()
{
    m_subject->sharedPixmap = QPixmap(50,50);
    m_subject->enablePixmapUpdates();
    QVERIFY(m_subject->pixmapDamage != 0);
    m_subject->disablePixmapUpdates();
    QVERIFY(m_subject->pixmapDamage == 0);
}
#endif

QTEST_MAIN(Ut_MStatusBarView)
