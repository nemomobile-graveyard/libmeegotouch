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

#ifndef UT_MPOPUPLIST_H
#define UT_MPOPUPLIST_H

#include <QtTest/QtTest>
#include <QObject>
#include <QStringListModel>
#include <QStringList>

#include <MPopupList>


Q_DECLARE_METATYPE(MPopupList *);
Q_DECLARE_METATYPE(QModelIndex);

class Ut_MPopupList : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void setContent();
    void testCurrentIndex();
    void testSetItemIconID();
    void testSetItemPixmap();
    void testQStandardItemWithIcon();
    void testScrollTo();
    void testModelScrollTo();

private:
    MPopupList *m_popuplist;
    QStringListModel m_model;
    QStringList m_stringList;
};

#endif
