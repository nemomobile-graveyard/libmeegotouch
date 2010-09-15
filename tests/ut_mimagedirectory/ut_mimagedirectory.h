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

#ifndef UT_MIMAGEDIRECTORY_H
#define UT_MIMAGEDIRECTORY_H

#include <QtTest/QtTest>
#include <QObject>

class QSvgRenderer;

class MThemeDaemon
{
public:
    static QString systemThemeCacheDirectory();
};

class MThemeResourceManager
{
public:
    static MThemeResourceManager& instance();
    QSvgRenderer* svgRenderer(const QString& absoluteFilePath);
};

class MThemeImagesDirectory;

class Ut_MImageDirectory : public QObject
{
    Q_OBJECT
public:
    Ut_MImageDirectory();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testReloadLocalizedResources_data();
    void testReloadLocalizedResources();

private:
    MThemeImagesDirectory *m_subject;
};

#endif
