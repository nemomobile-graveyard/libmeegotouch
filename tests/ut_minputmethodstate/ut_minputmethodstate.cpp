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

#include <QSignalSpy>

#include "ut_minputmethodstate.h"
#include "minputmethodstate.h"

void Ut_MInputMethodState::initTestCase()
{
    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_minputmethodstate" };
    MApplication::setLoadMInputContext(false);

    m_app.reset(new MApplication(argc, app_name));
}

void Ut_MInputMethodState::cleanupTestCase()
{
    m_app->quit();
}

void Ut_MInputMethodState::init()
{
}

void Ut_MInputMethodState::cleanup()
{
}

void Ut_MInputMethodState::testActiveWindowOrientationAngle()
{
    MInputMethodState *state = MInputMethodState::instance();
    QSignalSpy spy(state, SIGNAL(activeWindowOrientationAngleChanged(M::OrientationAngle)));

    // Initial state
    QCOMPARE(state->activeWindowOrientationAngle(), M::Angle0);


    // Set, get, get notified
    state->setActiveWindowOrientationAngle(M::Angle270);
    QCOMPARE(state->activeWindowOrientationAngle(), M::Angle270);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().value<M::OrientationAngle>(), M::Angle270);

    // Set the same orientation again, don't get notified
    state->setActiveWindowOrientationAngle(M::Angle270);
    QCOMPARE(spy.count(), 1);
}

void Ut_MInputMethodState::testInputMethodArea()
{
    MInputMethodState *state = MInputMethodState::instance();
    QSignalSpy spy(state, SIGNAL(inputMethodAreaChanged(QRect)));

    // Initial state
    QCOMPARE(state->inputMethodArea(), QRect());

    // Set, get, get notified
    const QRect rect(0, 0, 10, 10);
    state->setInputMethodArea(rect);
    QCOMPARE(state->inputMethodArea(), rect);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().value<QRect>(), rect);

    // Set the same orientation again, don't get notified
    state->setInputMethodArea(rect);
    QCOMPARE(spy.count(), 1);
}

// checks registering and modifying toolbars
void Ut_MInputMethodState::testToolbars()
{
    MInputMethodState *state = MInputMethodState::instance();

    QSignalSpy registerSpy(state, SIGNAL(toolbarRegistered(int, QString)));

    QSignalSpy changeSpy(state,
                         SIGNAL(toolbarItemAttributeChanged(int, QString, QString, QVariant)));

    // register toolbar
    int id = state->registerToolbar("filename");
    QCOMPARE(registerSpy.count(), 1);
    registerSpy.clear();

    // make a modification
    state->setToolbarItemAttribute(id, "itemName", "attributeName", 42);
    QCOMPARE(changeSpy.count(), 1);

    // check that setting value doesn't make signals if value does not change
    state->setToolbarItemAttribute(id, "itemName", "attributeName", 42);
    QCOMPARE(changeSpy.count(), 1);

    changeSpy.clear();

    // another toolbar with one item containing two attributes, and second containing one
    int id2 = state->registerToolbar("filename2");

    // check that registered toolbar ids are correct
    QList<int> idList = state->toolbarIds();
    QList<int> expectedIds;
    expectedIds << id << id2;
    QCOMPARE(idList, expectedIds);

    QMap<QString, QMap<QString, QVariant> > valuesToSet;
    QMap<QString, QVariant> item2Attributes;
    item2Attributes.insert("attributeName2", 43);
    item2Attributes.insert("attributeName3", 44);
    valuesToSet.insert("itemName2", item2Attributes);
    
    QMap<QString, QVariant> item3Attributes;
    item3Attributes.insert("attributeName4", 45);
    valuesToSet.insert("itemName3", item3Attributes);

    foreach (QString itemName, valuesToSet.keys()) {
        QMap<QString, QVariant> attributes = valuesToSet[itemName];

        foreach (QString attributeName, attributes.keys()) {
            QVariant attribute = attributes[attributeName];

            state->setToolbarItemAttribute(id2, itemName, attributeName, attribute);
        }
    }

    QCOMPARE(changeSpy.count(), 3);
    changeSpy.clear();

    // check the toolbar item attribute values are really the ones set
    QMap<QString, QMap<QString, QVariant> > itemAttributeValues = state->toolbarState(id2);
    QList<QString> names;

    QCOMPARE(valuesToSet, itemAttributeValues);

    // check that invalid toolbars are not accepted
    int invalidToolbar = 13131313;
    state->setToolbarItemAttribute(invalidToolbar, "itemName", "attributeName", 42);
    QCOMPARE(changeSpy.count(), 0);
}


QTEST_APPLESS_MAIN(Ut_MInputMethodState)
