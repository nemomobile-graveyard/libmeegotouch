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

#include <mapplication.h>
#include <msystemdirectories.h>

#include "ut_msystemdirectories.h"

void Ut_MSystemDirectories::init()
{
}

void Ut_MSystemDirectories::cleanup()
{
}

MApplication *app;
void Ut_MSystemDirectories::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msystemdirectories" };
    app = new MApplication(argc, app_name);
}

void Ut_MSystemDirectories::cleanupTestCase()
{
    delete app;
}

void Ut_MSystemDirectories::testCacheDirectoryExists()
{

  QString cacheDir =  MSystemDirectories::cacheDirectory();
  QFileInfo cacheDirInfo(cacheDir);

  QVERIFY(cacheDirInfo.exists() == true);
}

void Ut_MSystemDirectories::testCacheDirectoryIsDir()
{

  QString cacheDir =  MSystemDirectories::cacheDirectory();
  QFileInfo cacheDirInfo(cacheDir);

  QVERIFY(cacheDirInfo.isDir() == true);
}

void Ut_MSystemDirectories::testCacheDirectoryIsWritable()
{

  QString cacheDir =  MSystemDirectories::cacheDirectory();
  QFileInfo cacheDirInfo(cacheDir);

  QVERIFY(cacheDirInfo.isWritable() == true);
}

void Ut_MSystemDirectories::testPrefixedCacheDirectoryExists()
{
  MSystemDirectories::setPrefix("test");
  QString cacheDir = MSystemDirectories::cacheDirectory();

  QVERIFY (cacheDir.startsWith("test") == true);

}

QTEST_APPLESS_MAIN(Ut_MSystemDirectories)
