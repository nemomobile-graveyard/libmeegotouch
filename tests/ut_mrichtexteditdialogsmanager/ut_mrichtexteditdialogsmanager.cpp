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

#include <MApplicationWindow>
#include <QColor>
#include <QSignalSpy>
#include <mapplication.h>
#include <MComboBox>
#include <mcolorcombobox.h>
#include <MDialog>
#include <mrichtexteditdialogsmanager_p.h>

Ut_MRichTextEditDialogsManager_callbacks::Ut_MRichTextEditDialogsManager_callbacks(
                                                                Ut_MRichTextEditDialogsManager *ut,
                                                                bool accept)
: m_ut(ut)
, accept(accept)
{ }

Ut_MRichTextEditDialogsManager_callbacks::~Ut_MRichTextEditDialogsManager_callbacks()
{ }

void Ut_MRichTextEditDialogsManager::init()
{
    m_subject = new MRichTextEditDialogsManager;
    m_app->processEvents();
}

void Ut_MRichTextEditDialogsManager::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MRichTextEditDialogsManager::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mrichtexteditdialogsmanager" };
    m_app = new MApplication(argc, app_name);
    m_appWin = new MApplicationWindow;
}

void Ut_MRichTextEditDialogsManager::cleanupTestCase()
{
    delete m_appWin;
    m_appWin = 0;
    delete m_app;
    m_app = 0;
}

void Ut_MRichTextEditDialogsManager_callbacks::testSettingTextStyle_callback()
{
    m_ut->m_subject->fontColorCombo->colorPicked(QColor(Qt::red));

    m_ut->m_subject->fontFamilyCombo->activated("Family1");

    QItemSelectionModel *sizeSelection = m_ut->m_subject->fontSizeCombo->selectionModel();
    sizeSelection->setCurrentIndex(sizeSelection->currentIndex().sibling(20, 0), QItemSelectionModel::Current);
    m_ut->m_subject->fontSizeCombo->activated("");

    if (accept) {
        m_ut->m_subject->dialogs.textStyles.first->accept();
    } else {
        m_ut->m_subject->dialogs.textStyles.first->reject();
    }
}

void Ut_MRichTextEditDialogsManager::testSettingTextStyle_data()
{
    QTest::addColumn<bool>("accept");
    QTest::addColumn<bool>("changed");

    QTest::newRow("rejected") << false << false;
    QTest::newRow("accepted") << true << true;
}

// styles from dialog must be applied only after "Done" button is pressed
// (NB#290512 - Closing the style settings dialog has effect of the style on the text)
// this is collective effect, which is to be tested by UI tests
// here we check that font*Selected() signals emitted if and only if the font dialog is accepted
void Ut_MRichTextEditDialogsManager::testSettingTextStyle()
{
    QFETCH(bool, accept);
    QFETCH(bool, changed);

    QSignalSpy font(m_subject, SIGNAL(fontFamilySelected(const QString&)));
    QSignalSpy size(m_subject, SIGNAL(fontSizeSelected(int)));
    QSignalSpy color(m_subject, SIGNAL(fontColorSelected(const QColor&)));

    Ut_MRichTextEditDialogsManager_callbacks cb(this, accept);
    QTimer::singleShot(1000, &cb, SLOT(testSettingTextStyle_callback()));
    m_subject->showTextStylingDialog("Family0", 18, QColor(Qt::black));

    if (changed) {
        QCOMPARE(font.size(), 1);
        QCOMPARE(font.first().first().toString(), QString("Family1"));
        QCOMPARE(size.size(), 1);
        QCOMPARE(size.first().first().toInt(), 28);
        QCOMPARE(color.size(), 1);
        QCOMPARE(color.first().first().value<QColor>(), QColor(Qt::red));
    } else {
        QCOMPARE(font.size(), 0);
        QCOMPARE(size.size(), 0);
        QCOMPARE(color.size(), 0);
    }
}

QTEST_APPLESS_MAIN(Ut_MRichTextEditDialogsManager)
