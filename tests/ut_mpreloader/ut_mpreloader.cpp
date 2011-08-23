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

#include "ut_mpreloader.h"
#include <mpreloader.h>
#include <msystemdirectories.h>

#include <qfontmetrics.h>

QStringList fontStrings;

namespace {
    static QString g_CurrentAppDir;
}

//stub
QString MSystemDirectories::cacheDirectory()
{
    return ::g_CurrentAppDir;
}

class QFontPrivate {
public:
    QAtomicInt ref;
};

QFontMetrics::QFontMetrics(const QFont &font)
    : d(font.d.data())
{
    fontStrings.removeOne(font.toString());
}

void Ut_MPreloader::init()
{
    fontStrings.append("Helvetica,-1,4,5,25,0,0,0,0,0");
    fontStrings.append("Helvetica,-1,4,5,75,0,0,0,0,0");
    fontStrings.append("Helvetica,-1,8,5,25,0,0,0,0,0");
    fontStrings.append("Helvetica,-1,8,5,75,0,0,0,0,0");
    fontStrings.append("Helvetica,-1,20,5,25,0,0,0,0,0");
    fontStrings.append("Helvetica,-1,20,5,75,0,0,0,0,0");
    fontStrings.append("Courier New,-1,6,5,50,1,0,0,0,0");
    fontStrings.append("Courier New,-1,12,5,50,1,0,0,0,0");
    fontStrings.append("Font that is not installed in system,-1,1,5,25,0,0,0,0,0");
    fontStrings.append("Font that is not installed in system,-1,1,5,50,0,0,0,0,0");
    fontStrings.append("Font that is not installed in system,-1,2,5,25,0,0,0,0,0");
    fontStrings.append("Font that is not installed in system,-1,2,5,50,0,0,0,0,0");
    fontStrings.append("Font that is not installed in system,-1,3,5,25,0,0,0,0,0");
    fontStrings.append("Font that is not installed in system,-1,3,5,50,0,0,0,0,0");
}

void Ut_MPreloader::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MPreloader::testLoadingFontsFromConfigFile()
{
    m_subject = new MPreloader();

    QCOMPARE(fontStrings.count(), 0);
}

int main(int argc, char *argv[])
{
    if (argc > 0) {
        ::g_CurrentAppDir = QFileInfo(argv[0]).absolutePath() + QDir::separator();
        qDebug() << g_CurrentAppDir;
    }
    Ut_MPreloader tc;
    return QTest::qExec(&tc, argc, argv);
}
