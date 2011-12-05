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

#include <QObject>
#include <QSignalSpy>
#include "mescapebuttonpanel.h"
#include "ut_mescapebuttonpanel.h"
#include "mapplication.h"

MApplication *app;

void Ut_MEscapeButtonPanel::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mescapebuttonpanel" };
    app = new MApplication(argc, app_name);
}

void Ut_MEscapeButtonPanel::cleanupTestCase()
{
    delete app;
}

void Ut_MEscapeButtonPanel::init()
{
    m_subject = new MEscapeButtonPanel();
}

void Ut_MEscapeButtonPanel::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MEscapeButtonPanel::testEscapeMode()
{
    qRegisterMetaType<MEscapeButtonPanelModel::EscapeMode>("MEscapeButtonPanelModel::EscapeMode");
    QSignalSpy spy(m_subject, SIGNAL(escapeModeChanged(MEscapeButtonPanelModel::EscapeMode)));
    QList<QVariant> arguments;

    m_subject->setEscapeMode(MEscapeButtonPanelModel::BackMode);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).value<MEscapeButtonPanelModel::EscapeMode>() == MEscapeButtonPanelModel::BackMode); 
    QCOMPARE(m_subject->escapeMode(), MEscapeButtonPanelModel::BackMode);

    spy.clear();
    m_subject->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).value<MEscapeButtonPanelModel::EscapeMode>() == MEscapeButtonPanelModel::CloseMode);
    QCOMPARE(m_subject->escapeMode(), MEscapeButtonPanelModel::CloseMode);
}
QTEST_APPLESS_MAIN(Ut_MEscapeButtonPanel)
