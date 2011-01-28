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

#ifndef UT_MTEXTEDITVIEW_H
#define UT_MTEXTEDITVIEW_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>


class MApplication;
class MApplicationWindow;
class MTextEdit;
class MTextEditView;

class Ut_MTextEditView : public QObject
{
    Q_OBJECT
public:
    static QHash<QString, QPixmap *> themePixmap;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testPaint();
    void testBoundingRect();
    void testStyleUpdated();
    void testResizeEvent();
    void testGrowing();
    void testMultilineGrowsOnWordWrap();
    void testInputMethodQuery();
    void testMaskedCharacters();
    void testUpdateScrollWhenTextChanged();
    void testSizeHint();
    void testSendsScenePositionChangesOnMagnifier();

private:
    MTextEdit *m_controller;
    MTextEditView *m_subject;
    MApplication *m_app;
    MApplicationWindow *m_appWindow;
};

#endif

