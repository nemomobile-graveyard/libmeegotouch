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

#ifndef TESTS_UTILS_H
#define TESTS_UTILS_H

#include <QtTest>

namespace Ut_Utils
{
    /*! Generic signal waiter, because signals might not arrive instantly.
     *
     * How to use:
     *
     * QObject *obj = new QObject;
     * QSignalSpy spy(obj, SIGNAL(destroyed()));
     * delete obj;
     * Ut_Utils::waitForSignal(spy);
     */
    void waitForSignal(const QSignalSpy &spy, int waitLimit = 10)
    {
        int waitCount = 0;
        while (spy.count() < 1) {
            ++ waitCount;

            if (waitCount > waitLimit) {
                qCritical() << "Signal didn't arrive: "
                            << spy.signal();
                break;
            }

            QTest::qWait(100);
        }
    }
}

/*! An automated tester for common methods (and their expected behaviour),
 *  e.g., don't choke on self-assignment.
 *  TODO: add more common tests.
 *
 *  How to use:
 *
 *  MSetterTester<TesteeType, SetterParamType> tester(testee
 *                                                    &TesteeType::setParam,
 *                                                    &TesteeType::param);
 *  tester.set(new SetterParamType);
 *  tester.setReparents(new SetterParamType);
 *  teser.setSelfAssignment(new SetterParamType,
 *                          &SetterParamType::aParameterlessPredicate)
 *
 *
 *  TesteeType and SetterParamType must match the type where the function
 *  pointers were first declared, i.e., for non-virtual methods the most basic
 *  class that implements them.
 *  aParameterlessPredicate should have the signature
 *  "bool SetterParamType::somethingIsTrue() const".
 *
 *  See Ut_MTextEdit::testSetters for an example.
 */

template <class R, class T>
class MSetterTester
{
public:
    MSetterTester(R *newObj,
                  void (R::*newSetterFunction)(T *arg),
                  T *  (R::*newGetterFunction)())
        : setterFunction(newSetterFunction)
        , constGetterFunction(0)
        , getterFunction(newGetterFunction)
        , obj(newObj)
    {}

    MSetterTester(R *newObj,
                  void (R::*newSetterFunction)(T *arg),
                  T *  (R::*newGetterFunction)() const)
        : setterFunction(newSetterFunction)
        , constGetterFunction(newGetterFunction)
        , getterFunction(0)
        , obj(newObj)
    {}

    virtual void set(T *arg)
    {
        (*obj.*setterFunction)(arg);
        QCOMPARE(arg, get());
    }

    virtual void setSelfAssignment(T *arg, bool (T::*verify)())
    {
        setSelfAssignment(arg, (bool (T::*)() const)(verify));
    }

    virtual void setSelfAssignment(T *arg, bool (T::*verify)() const)
    {
        set(arg);

        // Setting arg again should not lead to its destruction, but some
        // getters cleanup whatever was previously set, and maybe forget
        // to guard against self-assignment:
        set(arg);

        // Using the verify method to check whether the object is still valid:
        (void) (*arg.*verify)();
    }

    virtual void setZero()
    {
        (*obj.*setterFunction)(0);
        QVERIFY(not get());
    }

    virtual void setReparents(T *arg, bool takesOwnership = true)
    {
        QObject *arg2q = dynamic_cast<QObject *>(arg);
        QObject *obj2q = dynamic_cast<QObject *>(obj);

        if (arg2q && obj2q) {
            QObject *lastParent = arg2q->parent();
            arg2q->setParent(0);

            (*obj.*setterFunction)(arg);
            QCOMPARE(arg2q->parent(), (takesOwnership ? obj2q : 0));

            arg2q->setParent(lastParent);
        } else {
            qWarning() << __PRETTY_FUNCTION__
                       << "Cannot test ownership transferal: no QObjects";
        }
    }

    virtual void setDoesNotReparent(T *arg)
    {
        setReparents(arg, false);
    }

    virtual T* get() const
    {
        return (constGetterFunction ? (*obj.*constGetterFunction)()
			            : (*obj.*getterFunction)());
    }

private:
    void (R::*setterFunction)(T *arg);
    T *  (R::*constGetterFunction)() const;
    T *  (R::*getterFunction)();
    R *obj;
};

#endif // TEST_UTILS_H
