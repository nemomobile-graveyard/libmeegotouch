/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include <duisettings.h>


#include "ut_duisettings.h"

void Ut_DuiSettings::init()
{
    QString doesNotExist("/root/createdByUt_DuiSettings");
    QString exists("/dev/null");

    m_subject = new DuiSettings(doesNotExist);
    delete m_subject;
    m_subject = new DuiSettings(exists);
}

void Ut_DuiSettings::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettings::initTestCase()
{
}

void Ut_DuiSettings::globalSettings()
{
    m_subject->globalValue(DuiSettings::ThemeNameSetting);
    m_subject->globalValue(DuiSettings::FrameworkSvgSetting);
    m_subject->globalValue(DuiSettings::FrameworkCssSetting);
}

void Ut_DuiSettings::cleanupTestCase()
{
}

QTEST_MAIN(Ut_DuiSettings)
