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

#include <mimagedirectory.h>
#include <mapplication.h>

#include "ut_mimagedirectory.h"

QString MThemeDaemon::systemThemeCacheDirectory()
{
    QString dir("/usr/share/theme/");
    return dir;
}

static MThemeResourceManager g_themeResourceManager;

MThemeResourceManager& MThemeResourceManager::instance()
{
    return g_themeResourceManager;
}

QSvgRenderer* MThemeResourceManager::svgRenderer(const QString& absoluteFilePath)
{
    Q_UNUSED( absoluteFilePath );
    return 0;
}

MApplication *app(NULL);

Ut_MImageDirectory::Ut_MImageDirectory() :
        m_subject(0)
{
}

void Ut_MImageDirectory::initTestCase()
{
    static char *app_name[1] = { (char *) "./ut_mimagedirectory" };
    static int argc = 0;
    app = new MApplication(argc, app_name);
    m_subject = new MThemeImagesDirectory(QApplication::applicationDirPath() + "/ut_mimagedirectory-samples");
    QVERIFY(m_subject != 0);
}

void Ut_MImageDirectory::cleanupTestCase()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MImageDirectory::testReloadLocalizedResources_data()
{
    QTest::addColumn<QString>("setLocale");
    QTest::addColumn<QString>("gotLocale");

    QTest::newRow("sr_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic")
      << "sr_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic"
      << "sr_Cyrl_RS_REVISED@collation=phonebook";

    QTest::newRow("sr_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic")
      << "sy_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic"
      << "sy";

    QTest::newRow("de_DE@collation=phonebook")
      << "de_DE@collation=phonebook"
      << "de_DE";

    QTest::newRow("de_DE_lala_foo@collation=phonebook")
      << "de_DE_lala_foo@collation=phonebook"
      << "de_DE_lala";

    QTest::newRow("de_CH@collation=phonebook")
      << "de_CH@collation=phonebook"
      << "de";
}

void Ut_MImageDirectory::testReloadLocalizedResources()
{
    QFETCH(QString, setLocale);
    QFETCH(QString, gotLocale);

    m_subject->reloadLocalizedResources( setLocale );

    QString myLocale = m_subject->locale();

    QCOMPARE( myLocale, gotLocale );
}

QTEST_APPLESS_MAIN(Ut_MImageDirectory);
