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

#ifndef PT_MCOMPONENTDATA2_H
#define PT_MCOMPONENTDATA2_H

#include <QtTest/QtTest>
#include <QObject>

/**
  * This performance test emulates the different things executed
  * when MComponentData is initialized.
  * It would be nicer to not duplicate the source code but linking
  * against MComponentData to pull in the private symbols pulls
  * in way to much dependencies.
  */
class Pt_MComponentData2 : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testabilityPlugin();
    void installMessageHandler();
    void themeId();
    void theme();
    void locale();
    void feedbackPlayer();
    void dbusConnection();
    void isConnected();
    void registerDefaultService();
    void showCursor();
    void inputContext();
};

#endif
