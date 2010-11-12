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

#ifndef PT_MLAYOUT_H
#define PT_MLAYOUT_H

#include <QtTest/QtTest>
#include <QObject>

class QGraphicsWidget;
class QGraphicsScene;

class Pt_MLayout : public QObject
{
    Q_OBJECT

public:
    Pt_MLayout():m_form(NULL), m_scene(NULL) {};

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void linearLayoutPerformance();
    void linearLayoutPerformance_data();
    void gridLayoutPerformance();
    void gridLayoutPerformance_data();
    void heightForWidthPerformance_data();
    void heightForWidthPerformance();
private:
    QGraphicsWidget *m_form;
    QGraphicsScene *m_scene;
};

#endif
