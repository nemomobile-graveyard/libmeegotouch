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

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include <msettings.h>


#include "ut_msettings.h"

void Ut_MSettings::init()
{
    QString doesNotExist("/root/createdByUt_MSettings");
    QString exists("/dev/null");

    m_subject = new MSettings(doesNotExist);
    delete m_subject;
    m_subject = new MSettings(exists);
}

void Ut_MSettings::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettings::initTestCase()
{
}

void Ut_MSettings::globalSettings()
{
    m_subject->globalValue(MSettings::ThemeNameSetting);
    m_subject->globalValue(MSettings::FrameworkSvgSetting);
    m_subject->globalValue(MSettings::FrameworkCssSetting);
}

void Ut_MSettings::cleanupTestCase()
{
}

QTEST_MAIN(Ut_MSettings)
