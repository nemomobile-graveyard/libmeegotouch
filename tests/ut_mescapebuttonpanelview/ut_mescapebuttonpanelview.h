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

#ifndef UT_MESCAPEBUTTONPANELVIEW_H
#define UT_MESCAPEBUTTONPANELVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class MEscapeButtonPanel;
class MEscapeButtonPanelView;

class Ut_MEscapeButtonPanelView : public QObject
{
    Q_OBJECT
public:

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testHiddenPanelWithCloseButton();
    void testHiddenPanelWithoutCloseButton();
    void testVisiblePanelWithCloseButton();
    void testVisiblePanelWithoutCloseButton();

private:
    MEscapeButtonPanel *m_buttonPanel;
    MEscapeButtonPanelView *m_subject;
};

#endif

