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

#include "ut_mrichtexteditdialogsmanager.h"
#include <QColor>
#include <QFontDatabase>
#include <QStringList>
#include <QSignalSpy>
#include <mapplication.h>
#include <MComboBox>
#include "mcolorcombobox.h"
#include <mrichtexteditdialogsmanager_p.h>

#include <QDebug>

void Ut_MRichTextEditDialogsManager::init()
{
    m_subject = new MRichTextEditDialogsManager;
    m_subject->initTextStylingDialog();
}

void Ut_MRichTextEditDialogsManager::cleanup()
{
    delete m_subject;
    m_subject = 0;
}
MApplication *app;

void Ut_MRichTextEditDialogsManager::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mrichtexteditdialogsmanager" };
    app = new MApplication(argc, app_name);
}

void Ut_MRichTextEditDialogsManager::cleanupTestCase()
{
    delete app;
}

void Ut_MRichTextEditDialogsManager::testSettingTextStyle()
{
    QCOMPARE(m_subject->fontFamilyCombo->currentText(), QString(""));

    QString firstFont = m_subject->fontFamilyCombo->itemText(0);
    int fontSize = 12;
    QColor blue("blue");
    m_subject->setTextStyleValues(firstFont, fontSize, blue);

    QSignalSpy familySpy(m_subject, SIGNAL(fontFamilySelected(QString)));
    QSignalSpy sizeSpy(m_subject, SIGNAL(fontSizeSelected(int)));
    QSignalSpy colorSpy(m_subject, SIGNAL(fontColorSelected(QColor)));

    m_subject->fontFamilyCombo->activated(firstFont);
    QCOMPARE(familySpy.count(), 1);
    QList<QVariant> arguments = familySpy.takeFirst();
    QCOMPARE(arguments[0].toString(), firstFont);

    m_subject->setFontSize();
    QCOMPARE(sizeSpy.count(), 1);
    arguments = sizeSpy.takeFirst();
    QCOMPARE(arguments[0].toInt(), fontSize);

    m_subject->fontColorCombo->colorPicked(blue);
    QCOMPARE(colorSpy.count(), 1);
    arguments = colorSpy.takeFirst();
    QCOMPARE(arguments[0].value<QColor>(), blue);
}

QTEST_APPLESS_MAIN(Ut_MRichTextEditDialogsManager)
