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

#include "ut_duilayout.h"

#include <duiapplication.h>
#include <duiscene.h>
#include <duilayout.h>
#include <duiabstractlayoutpolicy.h>
#include <duilinearlayoutpolicy.h>
#include <duigridlayoutpolicy.h>
#include <duibasiclayoutanimation.h>
#include <duilabel.h>
#include <duiflowlayoutpolicy.h>
#include <duifreestylelayoutpolicy.h>
#include <duibasiclayoutanimation.h>
#include <duitheme.h>
#include <duiwindow.h>

#include "../src/layout/duilayout_p.h"

#include <QtGui/QPushButton>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QSizePolicy>

class TestPolicy : public DuiAbstractLayoutPolicy
{
public:
    TestPolicy(DuiLayout *l) :
        DuiAbstractLayoutPolicy(l) {
        reset();
    }

    virtual ~TestPolicy() { }

    void relayout() {
        ++relayout_calls;
        items_in_last_relayout = count();
        for (int i = 0; i < items_in_last_relayout; i++)
            setItemGeometry(i, QRectF(0, 0, 0, 0));
    }

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const {
        Q_UNUSED(constraint);
        if (which == Qt::MaximumSize)
            return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        return QSizeF(0, 0);
    }
    virtual void addItem(QGraphicsLayoutItem *item) {
        DuiAbstractLayoutPolicy::addItem(item);
    }

    void reset() {
        relayout_calls = 0;
        items_in_last_relayout = 0;
        removed_items.clear();
    }

    int relayout_calls;
    int items_in_last_relayout;
    QList<QGraphicsLayoutItem *> removed_items;
};

class TestAnimation : public DuiBasicLayoutAnimation
{
public:
    TestAnimation(DuiLayout *layout) :
        DuiBasicLayoutAnimation(layout) {
        reset();
    }

    virtual ~TestAnimation()
    { }

    void reset() {
        layout_started = layout_to_animate = layout_done = false;
    }

    void animate(DuiItemState &i) {
        QVERIFY(!i.isAnimationDone() || (i.flags() != 0));
        layout_to_animate = true;
        if (i.geometryProgress() == 0) {
            i.setGeometryProgress(0.5);
            return;
        }
        i.item()->setGeometry(i.targetGeometry());
        i.animationDone();
    }

    void layoutAnimationStarted(DuiItemState *itemstate) {
        layout_started = true;
        DuiBasicLayoutAnimation::layoutAnimationStarted(itemstate);
    }

    void layoutAnimationFinished() {
        QVERIFY(layout_started);
        DuiBasicLayoutAnimation::layoutAnimationFinished();
        layout_done = true;
    }
    bool layout_started;
    bool layout_to_animate;
    bool layout_done;
};

class DuiLayoutTest : public DuiLayout
{
public:
    DuiLayoutTest(QGraphicsLayoutItem *parent = NULL) : DuiLayout(parent) {
        if (!animation())
            new DuiBasicLayoutAnimation(this);

    }
    virtual int addItem(QGraphicsLayoutItem *item) {
        return DuiLayout::addItem(item);
    }
};
//This class lets us check for layout requests
class GraphicsWidgetTest : public QGraphicsWidget
{
public:
    virtual void updateGeometry() {
        QGraphicsWidget::updateGeometry();
    }
};

DuiApplication *app;
DuiWindow *appWin;

Ut_DuiLayout::Ut_DuiLayout() :
    m_button(0),
    m_scene(0),
    m_proxy(0)
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_duilayout" };
    app = new DuiApplication(argc, argv);
    appWin = new DuiWindow;
    qRegisterMetaType<Dui::Orientation>("Dui::Orientation");
}

Ut_DuiLayout::~Ut_DuiLayout()
{
    Q_ASSERT(0 == m_button);
    Q_ASSERT(0 == m_scene);
    Q_ASSERT(0 == m_proxy);
}

void Ut_DuiLayout::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiLayout::init()
{
    Q_ASSERT(0 == m_button);
    Q_ASSERT(0 == m_scene);
    Q_ASSERT(0 == m_proxy);

    // setup a QGraphicsLayoutItem:
    m_scene = new DuiScene;
    appWin->setScene(m_scene);
    m_button = new QPushButton("Test");
    m_form = new QGraphicsWidget;
    m_proxy = m_scene->addWidget(m_button);
    m_scene->addItem(m_form);

    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle0, Dui::ImmediateOrientationChange);
    Q_ASSERT(0 != m_button);
    Q_ASSERT(0 != m_scene);
    Q_ASSERT(0 != m_proxy);
    DuiTheme::loadCSS(qApp->applicationDirPath() + "/ut_duilayout.css");
}

void Ut_DuiLayout::cleanup()
{
    delete m_scene; // Deletes the items in the scene, too!
    m_scene = 0;
    m_proxy = 0;
    m_button = 0;
    m_form = 0;
}

void Ut_DuiLayout::itemState()
{
    // ------------------------------------------------------------------
    // Construction:
    // ------------------------------------------------------------------

    // Null item state:
    DuiItemState null_state;
    QVERIFY(null_state.isNull());
    QVERIFY(null_state.item() == 0);
    QVERIFY(null_state.targetGeometry() == QRectF());
    QVERIFY(null_state.sourceGeometry() == QRectF());
    QVERIFY(null_state.flags() == DuiItemState::STATE_FLAG_NONE);

    QVERIFY(null_state.isAnimationDone());

    // Ignore setting stuff on null items:
    null_state.setTargetGeometry(QRectF(QPointF(1.0, 2.0), QSizeF(91.0, 92.0)));
    QVERIFY(null_state.targetGeometry() == QRectF());
    QVERIFY(null_state.sourceGeometry() == QRectF());
    QVERIFY(null_state.flags() == DuiItemState::STATE_FLAG_NONE);

    // item state:
    QRectF geometry(m_proxy->geometry());
    DuiItemState state1(m_proxy);
    QVERIFY(!state1.isNull());
    QVERIFY(state1.item() == m_proxy);
    QCOMPARE(state1.geometryProgress(), 1.0);
    QCOMPARE(state1.opacityProgress(), 1.0);
    QCOMPARE(state1.item()->geometry(), geometry);
    QVERIFY(state1.flags() == DuiItemState::STATE_FLAG_NONE);

    QVERIFY(state1.isAnimationDone());

    // Copy itemstate:
    DuiItemState state2(state1);
    QVERIFY(!state2.isNull());
    QVERIFY(state2.item() == m_proxy);
    QCOMPARE(state2.targetGeometry(), state1.targetGeometry());
    QCOMPARE(state2.item()->geometry(), state1.item()->geometry());
    QCOMPARE(state2.sourceGeometry(), state1.sourceGeometry());
    QCOMPARE(state2.flags(), state1.flags());
    QVERIFY(state2 == state1);

    QCOMPARE(state2.isAnimationDone(), state1.isAnimationDone());

    // Flag handling:
    QVERIFY(state1.isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED) == false);

    state1.setFlags(DuiItemState::STATE_FLAG_TO_BE_DELETED);
    QVERIFY(state1.isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED) == true);

    QVERIFY(state1.hasIdenticalLayout(state2) == false);
    QVERIFY((state1 == state2) == true);

    state1.removeFlags(DuiItemState::STATE_FLAG_TO_BE_DELETED);
    QCOMPARE(state1.isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED), false);

    // Move state1 and setTargetGeometry():
    QRectF new_target(state1.targetGeometry());
    new_target.moveTop(state1.targetGeometry().height() + 5.0);

    QRectF source(state1.item()->geometry());
    state1.setTargetGeometry(new_target);

    QCOMPARE(state1.targetGeometry(), new_target);
    QCOMPARE(state1.item()->geometry(), geometry);
    QCOMPARE(state1.sourceGeometry(), source);
    QCOMPARE(state1.flags(), DuiItemState::STATE_FLAG_TO_BE_SHOWN);

    // Comparisons:
    QCOMPARE(state1.hasIdenticalLayout(null_state), false);
    QCOMPARE(state1.hasIdenticalLayout(state1), true);
    QCOMPARE(state1.hasIdenticalLayout(state2), false);
    QCOMPARE((state1 == state2), true);
    QCOMPARE((state1 == null_state), false);

    // assignment:
    state2 = state1;
    QVERIFY(state1.hasIdenticalLayout(state2) == true);
    QVERIFY((state1 == state2) == true);
    state2.setTargetGeometry(source);
    QCOMPARE(state2.targetGeometry(), source);
    QCOMPARE(state1.item()->geometry(), geometry);
    QCOMPARE(state1.sourceGeometry(), source);
    QCOMPARE(state1.flags(), DuiItemState::STATE_FLAG_TO_BE_SHOWN);

    // animation:
    state1.setTargetGeometry(new_target);

    QRectF animation_target(source);
    animation_target.moveTop(source.height() + 2.5);
    state1.item()->setGeometry(animation_target);
    QCOMPARE(state1.targetGeometry(), new_target);
    QCOMPARE(state1.item()->geometry(), animation_target);
    QCOMPARE(state1.sourceGeometry(), source);
    QCOMPARE(state1.flags(), DuiItemState::STATE_FLAG_TO_BE_SHOWN);
}

void Ut_DuiLayout::testTakeAt()
{
    // Setup layout:
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);
    QVERIFY(layout->policy() == 0);
    QVERIFY(layout->registeredPolicies().isEmpty());

    //Use a qpointer to make sure that the objects are not deleted
    QPointer<QPushButton> button_pointer = m_button;
    QPointer<QGraphicsProxyWidget> proxy_pointer = m_proxy;
    QVERIFY(!button_pointer.isNull());
    QVERIFY(!proxy_pointer.isNull());

    // Create a policy:
    TestPolicy *policy1(new TestPolicy(layout));

    // Add graphics item:
    layout->addItem(m_proxy);
    QCOMPARE(layout->count(), 1);

    // Remove graphics item again, testing takeAt(0)
    QGraphicsLayoutItem *item = layout->takeAt(0);  // first position.
    QVERIFY(item == m_proxy);

    QVERIFY(!proxy_pointer.isNull());
    QVERIFY(!button_pointer.isNull());
    QCOMPARE(layout->count(), 0);

    layout->addItem(m_proxy);
    QCOMPARE(layout->count(), 1);

    layout->animatedDeleteAt(0);
    QGraphicsLayoutItem *deletedItem = layout->takeAt(0);  //This should cause it to be deleted

    QVERIFY(deletedItem == NULL);
    QCOMPARE(layout->count(), 0);
    QVERIFY(proxy_pointer.isNull());
    QVERIFY(button_pointer.isNull());

    // Delete policy1
    delete policy1;
    QVERIFY(layout->policy() == NULL);
    QVERIFY(layout->registeredPolicies().isEmpty());
    delete form;
}

void Ut_DuiLayout::testDeletingItem()
{
    // Setup layout:
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);

    //Use a qpointer to make sure that the objects are not deleted
    QPointer<QPushButton> button_pointer = m_button;
    QPointer<QGraphicsProxyWidget> proxy_pointer = m_proxy;

    // Create a policy:
    TestPolicy *policy1(new TestPolicy(layout));

    // Add graphics item:
    layout->addItem(m_proxy);
    QCOMPARE(layout->count(), 1);

    // Delete item directly, removing it from the layout
    delete layout->itemAt(0);

    QVERIFY(proxy_pointer.isNull());
    QVERIFY(button_pointer.isNull());
    QCOMPARE(layout->count(), 0);

    // Delete policy1
    delete policy1;
    QVERIFY(layout->policy() == NULL);
    QVERIFY(layout->registeredPolicies().isEmpty());
    delete form;
}

void Ut_DuiLayout::policyInteraction()
{
    // Setup layout:
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);
    layout->setAnimation(0); // turn of animation so that it does not delay
    // item creation/removal.
    QVERIFY(layout->policy() == 0);
    QVERIFY(layout->registeredPolicies().isEmpty());

    //Use a qpointer to make sure that the objects are not deleted
    QPointer<QPushButton> button_pointer = m_button;
    QPointer<QGraphicsProxyWidget> proxy_pointer = m_proxy;
    QVERIFY(!button_pointer.isNull());
    QVERIFY(!proxy_pointer.isNull());

    // Create a policy:
    TestPolicy *policy1(new TestPolicy(layout));
    QVERIFY(policy1->relayout_calls == 0);
    QVERIFY(layout->policy() == policy1);
    QVERIFY(layout->registeredPolicies().size() == 1);
    QVERIFY(layout->registeredPolicies().contains(policy1));

    // Set policy:
    layout->setPolicy(policy1);
    QVERIFY(policy1->relayout_calls == 0);
    QVERIFY(layout->policy() == policy1);
    QVERIFY(layout->registeredPolicies().size() == 1);
    QVERIFY(layout->registeredPolicies().contains(policy1));

    // Add graphics item:
    layout->addItem(m_proxy);
    QCOMPARE(layout->count(), 1);
    QCOMPARE(policy1->relayout_calls, 0);

    QVERIFY(layout->policy() == policy1);
    QVERIFY(layout->registeredPolicies().size() == 1);
    QVERIFY(layout->registeredPolicies().contains(policy1));

    policy1->reset();

    // Remove graphics item again:
    layout->removeAt(0); // first position.
    QVERIFY(!proxy_pointer.isNull());
    QVERIFY(!button_pointer.isNull());

    qApp->processEvents();
    QCOMPARE(layout->count(), 0);
    QCOMPARE(policy1->relayout_calls, 1);

    QVERIFY(layout->policy() == policy1);
    QVERIFY(layout->registeredPolicies().size() == 1);
    QVERIFY(layout->registeredPolicies().contains(policy1));

    policy1->reset();

    // Add second policy:
    TestPolicy *policy2(new TestPolicy(layout));

    QVERIFY(policy1 != policy2);
    QVERIFY(policy2->relayout_calls == 0);
    QVERIFY(layout->policy() == policy1);
    QVERIFY(layout->registeredPolicies().size() == 2);
    QVERIFY(layout->registeredPolicies().contains(policy1));
    QVERIFY(layout->registeredPolicies().contains(policy2));

    // Switch to second policy:
    layout->setPolicy(policy2);
    QVERIFY(layout->policy() == policy2);
    qApp->processEvents();

    QCOMPARE(policy1->relayout_calls, 0);

    QCOMPARE(policy2->relayout_calls, 1);

    QVERIFY(layout->registeredPolicies().size() == 2);
    QVERIFY(layout->registeredPolicies().contains(policy1));
    QVERIFY(layout->registeredPolicies().contains(policy2));

    policy1->reset();
    policy2->reset();
    // Add graphics item:
    layout->addItem(m_proxy);
    QCOMPARE(layout->count(), 1);
    QCOMPARE(policy1->relayout_calls, 0);

    QCOMPARE(policy2->relayout_calls, 0);

    QVERIFY(layout->policy() == policy2);
    QVERIFY(layout->registeredPolicies().size() == 2);
    QVERIFY(layout->registeredPolicies().contains(policy1));
    QVERIFY(layout->registeredPolicies().contains(policy2));

    // Remove graphics item again:
    layout->removeAt(0); // first position.
    QVERIFY(!proxy_pointer.isNull());
    QVERIFY(!button_pointer.isNull());
    delete m_proxy; //This deletes m_button too
    qApp->processEvents();
    QCOMPARE(layout->count(), 0);
    QCOMPARE(policy1->relayout_calls, 0);

    QVERIFY(layout->policy() == policy2);
    QCOMPARE(layout->registeredPolicies().size(), 2);
    QVERIFY(layout->registeredPolicies().contains(policy1));
    QVERIFY(layout->registeredPolicies().contains(policy2));

    // Delete policy2
    delete policy2;
    QVERIFY(layout->policy() == policy1);
    QCOMPARE(layout->registeredPolicies().size(), 1);
    QVERIFY(layout->registeredPolicies().contains(policy1));

    // Delete policy1
    delete policy1;
    QVERIFY(layout->policy() == NULL);
    QVERIFY(layout->registeredPolicies().isEmpty());
    delete form;
}

void Ut_DuiLayout::testReorderingItemsWithoutAnimation()
{
    DuiLayoutTest layout1;

    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    layout1.addItem(proxy_item1);
    layout1.addItem(proxy_item2);
    QCOMPARE(layout1.count(), 2);
    QVERIFY(layout1.itemAt(0) == proxy_item1);
    QVERIFY(layout1.itemAt(1) == proxy_item2);
    layout1.removeItem(proxy_item1);
    layout1.removeItem(proxy_item2);
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());
    QCOMPARE(layout1.count(), 0);
    layout1.addItem(proxy_item2);
    layout1.addItem(proxy_item1);
    QCOMPARE(layout1.count(), 2);
    QVERIFY(layout1.itemAt(0) == proxy_item2);
    QVERIFY(layout1.itemAt(1) == proxy_item1);
    layout1.removeItem(proxy_item1);
    layout1.removeItem(proxy_item2);
    delete proxy_item1;
    delete proxy_item2;
}

void Ut_DuiLayout::testDeletingLayoutDuringAnimation()
{
    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);

    //Test deleting layout during adding animation
    {
        DuiLayoutTest layout1;
        new TestAnimation(&layout1);
        DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(&layout1, Qt::Horizontal);
        policy->addItem(proxy_item1);
        policy->addItem(proxy_item2);
        QCOMPARE(layout1.count(), 2);
        static_cast<QGraphicsLayout *>(&layout1)->setGeometry(QRectF(0, 0, 100, 100)); // force a relayout now
    }
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());

    //Test deleting layout during deleting animation
    {
        DuiLayoutTest layout1;
        DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(&layout1, Qt::Horizontal);
        new TestAnimation(&layout1);
        policy->addItem(proxy_item1);
        policy->addItem(proxy_item2);
        QCOMPARE(layout1.count(), 2);
        static_cast<QGraphicsLayout *>(&layout1)->setGeometry(QRectF(0, 0, 100, 100)); // force a relayout now
        layout1.animatedDeleteAt(1);
        layout1.animatedDeleteAt(0);
    }
    QVERIFY(proxy_item1.isNull());
    QVERIFY(proxy_item2.isNull());
    //create the proxy items again
    proxy_item1 = m_scene->addWidget(new QPushButton);
    proxy_item2 = m_scene->addWidget(new QPushButton);
    //Test deleting items in layout with no policy
    {
        DuiLayoutTest layout1;
        TestAnimation *animation = new TestAnimation(&layout1);
        layout1.addItem(proxy_item1);
        layout1.addItem(proxy_item2);
        QCOMPARE(layout1.count(), 2);
        //The items should be hidden, since there's no policy to show them
        QCOMPARE(proxy_item1->isVisible(), false);

        layout1.animatedDeleteAt(1);
        layout1.animatedDeleteAt(0);

        QVERIFY(!animation->isAnimating());
        QVERIFY(proxy_item1.isNull());
        QVERIFY(proxy_item2.isNull());
    }
}

void Ut_DuiLayout::testDeletingThenHiding()
{
    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);

    DuiLayoutTest layout1;
    TestAnimation *animation = new TestAnimation(&layout1);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(&layout1, Qt::Horizontal);
    policy->addItem(proxy_item1);
    policy->addItem(proxy_item2);
    QCOMPARE(layout1.count(), 2);
    static_cast<QGraphicsLayout *>(&layout1)->setGeometry(layout1.geometry()); // force a relayout now
    layout1.animatedDeleteAt(1);
    layout1.animatedDeleteAt(0);
    //If they have not been deleted yet
    if (!proxy_item1.isNull())
        policy->removeItem(proxy_item1);
    if (!proxy_item2.isNull())
        policy->removeItem(proxy_item2);
    while (animation->isAnimating())
        QTest::qWait(50);
    QVERIFY(proxy_item1.isNull());
    QVERIFY(proxy_item2.isNull());
    qApp->processEvents();
    QVERIFY(!animation->isAnimating());
}

void Ut_DuiLayout::testDeletingLayoutDuringAnimationWithMultiplyPolicies()
{
    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    QPointer<TestAnimation> animation;
    //Test deleting layout during adding animation
    {
        DuiWidget *form = new DuiWidget;
        DuiLayoutTest *layout1 = new DuiLayoutTest;
        form->setLayout(layout1);
        TestPolicy *policy1(new TestPolicy(layout1));
        QVERIFY(layout1->policy() == policy1);

        layout1->addItem(proxy_item1);
        layout1->addItem(proxy_item2);
        animation = new TestAnimation(layout1);
        policy1->addItem(proxy_item1);
        policy1->addItem(proxy_item2);
        layout1->activate(); //for a relayout now so that the items become visible

        proxy_item1->setParentItem(NULL); //we don't want to delete the proxy_items yet
        proxy_item2->setParentItem(NULL);
        delete form;
    }
    QVERIFY(animation.isNull());
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());

    //Test deleting layout during deleting animation
    {
        DuiWidget *form = new DuiWidget;
        DuiLayoutTest *layout1 = new DuiLayoutTest;
        form->setLayout(layout1);
        TestPolicy *policy1(new TestPolicy(layout1));
        QVERIFY(layout1->policy() == policy1);

        animation = new TestAnimation(layout1);
        policy1->addItem(proxy_item1);
        policy1->addItem(proxy_item2);
        layout1->activate(); //for a relayout now so that the items become visible
        proxy_item1->setParentItem(NULL); // the items should be deleted even if they weren't a parent of form, since we marked them as to be deleted
        proxy_item2->setParentItem(NULL);
        layout1->animatedDeleteAt(1);
        layout1->animatedDeleteAt(0);
        delete form;
    }
    QVERIFY(animation.isNull());
    QVERIFY(proxy_item1.isNull());
    QVERIFY(proxy_item2.isNull());
}

void Ut_DuiLayout::testReorderingItemsWithAnimationWithMultiplePolicies()
{
    DuiLayoutTest layout1;
    TestAnimation *animation = new TestAnimation(&layout1);

    TestPolicy *policy1(new TestPolicy(&layout1));
    TestPolicy *policy2(new TestPolicy(&layout1));
    QVERIFY(layout1.policy() == policy1);

    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    layout1.addItem(proxy_item1);
    layout1.addItem(proxy_item2);
    QCOMPARE(layout1.count(), 2);
    QVERIFY(layout1.itemAt(0) == proxy_item1);
    QVERIFY(layout1.itemAt(1) == proxy_item2);
    layout1.removeItem(proxy_item1);
    layout1.removeItem(proxy_item2);
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());
    QCOMPARE(layout1.count(), 0);
    layout1.addItem(proxy_item2);
    layout1.addItem(proxy_item1);
    QCOMPARE(layout1.count(), 2);
    QVERIFY(layout1.itemAt(0) == proxy_item2);
    QVERIFY(layout1.itemAt(1) == proxy_item1);
    //don't give the animation a chance to finish adding the items.  Delete now.
    layout1.animatedDeleteAt(0);
    while (animation->isAnimating())
        QTest::qWait(50);
    QVERIFY(!animation->isAnimating());
    QCOMPARE(layout1.count(), 1); // Item should be removed and deleted
    QVERIFY(proxy_item2.isNull());
    layout1.animatedDeleteAt(0);

    while (animation->isAnimating())
        QTest::qWait(50);
    QVERIFY(!animation->isAnimating());
    QCOMPARE(layout1.count(), 0); // Item should be removed and deleted
    QVERIFY(proxy_item1.isNull());

    delete policy1;
    delete policy2;
}

void Ut_DuiLayout::testReorderingItemsWithAnimation()
{
    DuiLayoutTest layout1;
    TestAnimation *animation = new TestAnimation(&layout1);

    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    layout1.addItem(proxy_item1);
    layout1.addItem(proxy_item2);
    QCOMPARE(layout1.count(), 2);
    QVERIFY(layout1.itemAt(0) == proxy_item1);
    QVERIFY(layout1.itemAt(1) == proxy_item2);
    layout1.removeItem(proxy_item1);
    layout1.removeItem(proxy_item2);
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());
    QCOMPARE(layout1.count(), 0);
    layout1.addItem(proxy_item2);
    layout1.addItem(proxy_item1);
    QCOMPARE(layout1.count(), 2);
    QVERIFY(layout1.itemAt(0) == proxy_item2);
    QVERIFY(layout1.itemAt(1) == proxy_item1);
    //don't give the animation a chance to finish adding the items.  Delete now.
    layout1.animatedDeleteAt(0);

    while (animation->isAnimating())
        QTest::qWait(50);
    QVERIFY(!animation->isAnimating());
    QCOMPARE(layout1.count(), 1); // Item should be removed and deleted
    QVERIFY(proxy_item2.isNull());
    layout1.animatedDeleteAt(0);
    while (animation->isAnimating())
        QTest::qWait(50);
    QVERIFY(!animation->isAnimating());
    QCOMPARE(layout1.count(), 0); // Item should be removed and deleted
    QVERIFY(proxy_item1.isNull());

}

void Ut_DuiLayout::testRemoveItemFromLayout()
{
    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QGraphicsLinearLayout *layout_item = new QGraphicsLinearLayout;
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    layout_item->addItem(proxy_item2);

    DuiLayoutTest layout1;
    layout1.addItem(proxy_item1);
    layout1.addItem(layout_item);
    QCOMPARE(layout1.count(), 2);
    layout1.removeItem(proxy_item1);
    QCOMPARE(layout1.count(), 1);
    layout1.removeItem(proxy_item2); //This should fail since proxy_item2 isn't directly in the layout
    QCOMPARE(layout1.count(), 1);
    layout1.removeItem(layout_item);
    QCOMPARE(layout1.count(), 0);
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());
    //We can't directly check whether layout_item has been deleted
    //but since we only removed the item, it shouldn't be deleted
    //so proxy_item2 should still have it as a parent
    //If the below verify fails, DuiLayoutTest is deleting the item when it shouldn't
    QVERIFY(proxy_item2->parentLayoutItem() != NULL);

    //If the below verify fails, DuiLayoutTest isn't setting the parent to NULL
    QVERIFY(proxy_item1->parentLayoutItem() == NULL);
}

void Ut_DuiLayout::testDeletingLayout()
{
    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QGraphicsLinearLayout *layout_item = new QGraphicsLinearLayout;
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    layout_item->addItem(proxy_item2);

    QCOMPARE(proxy_item1->ownedByLayout(), false);
    QCOMPARE(proxy_item2->ownedByLayout(), false);
    QCOMPARE(layout_item->ownedByLayout(), true);
    {
        DuiLayoutTest layout1;
        layout1.addItem(proxy_item1);
        layout1.addItem(layout_item);
    }
    //Layout is now deleted.  It should only delete the layout
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());

    //We can't directly check whether layout_item has been deleted
    //but if its deleted, proxy_item2 shouldn't have a layout parent
    //If the below verify fails, DuiLayoutTest isn't deleting items that are ownerByLayout
    QVERIFY(proxy_item2->parentLayoutItem() == NULL);
    //If the below verify fails, DuiLayoutTest isn't setting the parent to NULL
    QVERIFY(proxy_item1->parentLayoutItem() == NULL);
}

void Ut_DuiLayout::testDeletingItemWithAnimation()
{
    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QGraphicsLinearLayout *layout_item = new QGraphicsLinearLayout;
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);
    layout_item->addItem(proxy_item2);

    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout1 = new DuiLayoutTest();
    form->setLayout(layout1);
    form->setGeometry(0, 0, 300, 300);

    //If this test fails, then it means that the layout.count() does not immediately
    //decrease after the item is removed.
    TestAnimation *animation = new TestAnimation(layout1);;
    TestPolicy *policy1(new TestPolicy(layout1));
    new TestPolicy(layout1); //Create a dummy policy - just for testing
    QVERIFY(layout1->policy() == policy1);
    policy1->addItem(proxy_item1);
    policy1->addItem(layout_item);
    QCOMPARE(layout1->count(), 2);
    qApp->processEvents(); //required for the items to be shown

    //Note that we do not wait for the animation to finish adding the item before removing it
    layout1->animatedDeleteAt(1);
    layout1->animatedDeleteAt(0);
    if (!animation->isAnimating())
        QCOMPARE(layout1->count(), 0);
    while (animation->isAnimating())
        QTest::qWait(50);
    QVERIFY(!animation->isAnimating());
    QCOMPARE(layout1->count(), 0); // Item should be removed and deleted

    QVERIFY(proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());  // proxy_item2 is inside a layout that was deleted, so although the layout was deleted, proxy_item2 should not
    QVERIFY(proxy_item2->parentLayoutItem() == NULL);
    delete form;
}
void Ut_DuiLayout::testRemovingItemWithAnimation()
{
    //If this test fails, then it means that the layout.count() does not immediately
    //decrease after the item is removed.
    DuiLayoutTest layout1;
    new TestAnimation(&layout1);

    TestPolicy *policy1(new TestPolicy(&layout1));
    new TestPolicy(&layout1);
    QVERIFY(layout1.policy() == policy1);

    layout1.addItem(m_proxy);
    QCOMPARE(layout1.count(), 1);
    //Note that we do not wait for the animation to finish adding the item before removing it
    layout1.removeItem(m_proxy);
    QCOMPARE(layout1.count(), 0);
}
void Ut_DuiLayout::animationInteraction()
{
    DuiLayoutTest layout1;
    // Setup layouts:
    QVERIFY(layout1.animation() != 0);
    DuiLayoutTest layout2;
    QVERIFY(layout2.animation() != 0);

    //Use a qpointer to make sure that the objects are not deleted
    QPointer<QPushButton> button_pointer = m_button;
    QPointer<QGraphicsProxyWidget> proxy_pointer = m_proxy;
    QVERIFY(!button_pointer.isNull());
    QVERIFY(!proxy_pointer.isNull());

    // Set animation:
    TestAnimation *animation1 = new TestAnimation(&layout1);
    QVERIFY(!animation1->isAnimating());
    QVERIFY(layout1.animation() == animation1);

    TestAnimation *animation2 = new TestAnimation(&layout2);
    QVERIFY(layout2.animation() == animation2);
    QVERIFY(!animation2->isAnimating());

    // Setup Policy:
    TestPolicy *policy2(new TestPolicy(&layout2));
    TestPolicy *policy1(new TestPolicy(&layout1));
    QVERIFY(layout1.policy() == policy1);
    QVERIFY(layout2.policy() == policy2);

    // Add item to layout1:
    QCOMPARE(layout1.count(), 0);
    layout1.addItem(m_proxy);
    QCOMPARE(layout1.count(), 1);
    QVERIFY(!animation1->layout_started);
    QVERIFY(!animation1->layout_done);
    QVERIFY(!animation1->isAnimating());
    QVERIFY(m_proxy->parentLayoutItem() == &layout1);

    // Remove item from layout1
    layout1.removeItem(m_proxy);
    QVERIFY(!button_pointer.isNull());
    QVERIFY(!proxy_pointer.isNull());
    m_proxy->setParentLayoutItem(NULL);
    QVERIFY(m_proxy->parentLayoutItem() != &layout1);

    //Make sure that the count decreases immediately,
    //i.e. not waiting for it to be animatedly hidden
    QCOMPARE(layout1.count(), 0);

    // Set animation back to 0:
    layout1.setAnimation(0);
    QVERIFY(layout1.animation() == 0);
    layout2.setAnimation(0);
    QVERIFY(layout2.animation() == 0);

    delete policy1;
    delete policy2;
}

void Ut_DuiLayout::testEmptyAnimation()
{
    DuiLayoutTest layout;
    layout.setAnimation(0);
    QVERIFY(layout.animation() == 0);

    QPointer<QGraphicsProxyWidget> proxy_item1 = m_scene->addWidget(new QPushButton);
    QPointer<QGraphicsProxyWidget> proxy_item2 = m_scene->addWidget(new QPushButton);

    QCOMPARE(layout.count(), 0);
    layout.addItem(proxy_item1);
    QCOMPARE(layout.count(), 1);
    layout.addItem(proxy_item2);
    QCOMPARE(layout.count(), 2);

    layout.removeItem(proxy_item2);
    QCOMPARE(layout.count(), 1);
    layout.removeItem(proxy_item1);
    QCOMPARE(layout.count(), 0);
    QVERIFY(!proxy_item1.isNull());
    QVERIFY(!proxy_item2.isNull());
    delete proxy_item1;
    delete proxy_item2;
}

void Ut_DuiLayout::testDuiLabelOnLayout()
{
    //This is to test specifically for BUG 117346 - DuiLayout scales oddly. This was
    //due to two separate issues:
    //
    //1) DuiLabelView had a messed up implementation of sizeHint. It should be using
    //the width constraint to determine the height.  Instead it invalidates the
    //geometry (and thus invalidates the layout) every time the size changes.  This
    //means that when the layout changes size, the animation resizes the widget which
    //in turn invalidates the layout, meaning that the layout has to start all over
    //again.
    //
    //2) When the layout is invalidated, DuiBasicLayoutAnimation counted the item as
    //being added again, retriggering the addItem animation.
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayout *layout = new DuiLayoutTest();
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    form->setLayout(layout);
    DuiLabel *labelTitle = new DuiLabel("Label title");
    mainLayoutPolicy->addItem(labelTitle);
    form->resize(860, 480);
    m_scene->addItem(form);
    QSizeF size;
    do {
        QVERIFY(labelTitle->size().width() >= size.width());
        QVERIFY(labelTitle->size().height() >= size.height());
        size = labelTitle->size();
        QTest::qWait(50);
    } while (layout->animation() && layout->animation()->isAnimating());
}

void Ut_DuiLayout::testHidingShowingWidgets_data()
{
    QTest::addColumn<bool>("reverse"); //Whether to use rtl layout direction

    QTest::newRow("LTR") << false;
    QTest::newRow("RTL") << true;
}

void Ut_DuiLayout::testHidingShowingWidgets()
{
    QFETCH(bool, reverse); //Whether to use rtl layout
    qApp->setLayoutDirection(reverse ? Qt::RightToLeft : Qt::LeftToRight);

    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);

    int w = 212;

    // here we save the expected geometries in different geomXY QRectF
    // variables. the X gives the column index starting at one,
    // and Y gives the row index starting at one

    QRectF geom11;
    QRectF geom12;
    QRectF geom21;
    QRectF geom22;

    if (reverse) {
        geom11 = QRectF(w - 104,  4, 100, 100);
        geom12 = QRectF(w - 104, 108, 100, 100);
        geom21 = QRectF(w - 208,  4, 100, 100);
        geom22 = QRectF(w - 208, 108, 100, 100);
    } else {
        geom11 = QRectF(4,  4, 100, 100);
        geom12 = QRectF(4, 108, 100, 100);
        geom21 = QRectF(108,  4, 100, 100);
        geom22 = QRectF(108, 108, 100, 100);
    }

    form->setGeometry(QRectF(0, 0, w, 212));
    layout->setMinimumSize(QSizeF(w, 212));
    m_scene->addItem(form);
    DuiLinearLayoutPolicy *policy1 = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    DuiLinearLayoutPolicy *policy2 = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    policy1->setSpacing(4);
    policy2->setSpacing(4);
    layout->setContentsMargins(4, 4, 4, 4);
    policy1->setContentsMargins(-1, -1, -1, -1);
    policy2->setContentsMargins(-1, -1, -1, -1);

    QCOMPARE(form->geometry(), QRectF(0, 0, w, 212));
    QCOMPARE(layout->geometry(), QRectF(0, 0, w, 212));

    QGraphicsWidget *item1 = new QGraphicsWidget;
    QGraphicsWidget *item2 = new QGraphicsWidget;
    QGraphicsWidget *item3 = new QGraphicsWidget;

    item1->setPreferredSize(100, 100);
    item2->setPreferredSize(100, 100);
    item3->setPreferredSize(100, 100);
    item1->setMaximumSize(100, 100);
    item2->setMaximumSize(100, 100);
    item3->setMaximumSize(100, 100);

    //Items 1 & 2 only in the first policy
    policy1->addItem(item1);
    policy1->addItem(item2);

    //Items 2 & 3 only in the second policy
    policy2->addItem(item2);
    policy2->addItem(item3);
    qApp->processEvents();
    QCOMPARE(layout->d_ptr->states.at(2).isAnimationDone(), true);

    if (layout->d_ptr->states.at(0).isAnimationDone()) //probably wont be done, but if system really slow (e.g. running in valgrind) it can be finished
        QCOMPARE(layout->d_ptr->states.at(0).flags(), DuiItemState::STATE_FLAG_SHOWING);
    else {
        QVERIFY(layout->d_ptr->states.at(0).isSet(DuiItemState::STATE_FLAG_SHOWING) ||
                layout->d_ptr->states.at(0).isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN));
    }

    if (layout->d_ptr->states.at(1).isAnimationDone())
        QCOMPARE(layout->d_ptr->states.at(0).flags(), DuiItemState::STATE_FLAG_SHOWING);
    else {
        QVERIFY(layout->d_ptr->states.at(0).isSet(DuiItemState::STATE_FLAG_SHOWING) ||
                layout->d_ptr->states.at(0).isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN));
    }
    QCOMPARE(layout->d_ptr->states.at(2).flags(), DuiItemState::STATE_FLAG_NONE);

    QCOMPARE(layout->d_ptr->states.at(0).targetGeometry(), geom11);
    QCOMPARE(layout->d_ptr->states.at(1).targetGeometry(), geom12);

    QCOMPARE(item3->isVisible(), false);

    //After processing events, item1 and item2 won't be fully expanded but should have their center correct already
    QCOMPARE(item1->geometry().center(), geom11.center());
    QCOMPARE(item2->geometry().center(), geom12.center());

    while (layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(layout->d_ptr->states.at(0).flags(), DuiItemState::STATE_FLAG_SHOWING);
    QCOMPARE(layout->d_ptr->states.at(1).flags(), DuiItemState::STATE_FLAG_SHOWING);
    QCOMPARE(layout->d_ptr->states.at(2).flags(), DuiItemState::STATE_FLAG_NONE);

    layout->setPolicy(policy2);
    qApp->processEvents();

    QCOMPARE(form->geometry(), QRectF(0, 0, 212, 212));
    QCOMPARE(layout->geometry(), QRectF(0, 0, 212, 212));

    QCOMPARE(layout->d_ptr->states.at(1).targetGeometry(), geom11);
    QCOMPARE(layout->d_ptr->states.at(2).targetGeometry(), geom21);

    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(layout->d_ptr->states.at(0).flags(), DuiItemState::STATE_FLAG_NONE);
    QCOMPARE(layout->d_ptr->states.at(1).flags(), DuiItemState::STATE_FLAG_SHOWING);
    QCOMPARE(layout->d_ptr->states.at(2).flags(), DuiItemState::STATE_FLAG_SHOWING);

    QCOMPARE(item2->geometry(), geom11);
    QCOMPARE(item3->geometry(), geom21);

    QCOMPARE(item1->isVisible(), false);
}

void Ut_DuiLayout::testLayoutInsideLayoutOrientation()
{
    /* A simple test to check that a layout inside of a layout respects the current window orientation */
    DuiLayout *layout = new DuiLayout(m_form);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiLayout *innerLayout = new DuiLayout;
    DuiLinearLayoutPolicy *landscapePolicy = new DuiLinearLayoutPolicy(innerLayout, Qt::Horizontal);
    DuiLinearLayoutPolicy *portraitPolicy = new DuiLinearLayoutPolicy(innerLayout, Qt::Horizontal);

    innerLayout->setLandscapePolicy(landscapePolicy);
    innerLayout->setPortraitPolicy(portraitPolicy);

    policy->addItem(innerLayout);

    QCOMPARE(appWin->orientation(), Dui::Landscape); //Default
    QVERIFY(innerLayout->policy() == landscapePolicy);

}
/* We are testing a layout inside of a layout.  We test four combinations
 * to make sure that DuiLayout behaves exactly like QGraphicsLinearLayout
 */
void Ut_DuiLayout::testLayoutInsideLayout_data()
{
    QTest::addColumn<bool>("firstLayoutIsQ");  //Whether first Layout is QGraphicsLinearLayout
    QTest::addColumn<bool>("secondLayoutIsQ"); //Whether second (inner) Layout is QGraphicsLinearLayout
    QTest::addColumn<bool>("usewidget"); //Whether to put second layout inside a widget and put the widget in a layout, as opposed to putting the second layout directly into the first layout
    QTest::addColumn<bool>("scale"); //Whether to scale the form widget
    QTest::addColumn<bool>("reverse"); //Whether to use rtl layout direction
    //Test every combination
    for (int firstLayoutIsQ = 0; firstLayoutIsQ < 2; firstLayoutIsQ++)
        for (int secondLayoutIsQ = 0; secondLayoutIsQ < 2; secondLayoutIsQ++)
            for (int usewidget = 0; usewidget < 2; usewidget++)
                for (int scale = 0; scale < 2; scale++)
                    for (int reverse = 0; reverse < 2; reverse++) {
                        QString description = QString("%1 and %2 inside%3%4%5").arg(
                                                  firstLayoutIsQ ? "QGraphicsLinearLayout" : "DuiLayout",
                                                  secondLayoutIsQ ? "QGraphicsLinearLayout" : "DuiLayout",
                                                  usewidget ? " with widget" : "",
                                                  reverse ? ", reversed" : "",
                                                  scale ? " (scaled)" : "");
                        QTest::newRow(description.toLatin1()) << bool(firstLayoutIsQ) << bool(secondLayoutIsQ) << bool(usewidget) << bool(scale) << bool(reverse);
                    }
}

void Ut_DuiLayout::testLayoutInsideLayout()
{
    QFETCH(bool, firstLayoutIsQ);  //Whether first Layout is QGraphicsLinearLayout
    QFETCH(bool, secondLayoutIsQ); //Whether second Layout is QGraphicsLinearLayout
    QFETCH(bool, usewidget); //Whether to put second layout inside a widget and put the widget in a layout
    QFETCH(bool, scale); //Whether to scale the form widget
    QFETCH(bool, reverse); //Whether to use rtl layout
    qApp->setLayoutDirection(reverse ? Qt::RightToLeft : Qt::LeftToRight);

    QGraphicsWidget *form = new QGraphicsWidget;
    m_scene->addItem(form);

    qreal scalefactor = 1;
    if (scale) {
        scalefactor = 2;
        form->scale(scalefactor, scalefactor);
    }
    QGraphicsLayout *layout;
    DuiLinearLayoutPolicy *policy = NULL;
    if (firstLayoutIsQ) {
        layout = new QGraphicsLinearLayout;
        static_cast<QGraphicsLinearLayout *>(layout)->setSpacing(4);
    } else {
        layout = new DuiLayoutTest;
        policy = new DuiLinearLayoutPolicy(static_cast<DuiLayoutTest *>(layout), Qt::Horizontal);
        policy->setSpacing(4);
    }
    layout->setContentsMargins(4, 4, 4, 4);
    form->setLayout(layout);
    QGraphicsWidget *item = new QGraphicsWidget;
    item->setMinimumSize(100, 100);
    item->setMaximumSize(100, 100);
    if (firstLayoutIsQ)
        static_cast<QGraphicsLinearLayout *>(layout)->addItem(item);
    else
        policy->addItem(item);
    QPointF form_origin(40, 40);
    form->setGeometry(QRectF(form_origin.x(), form_origin.y(), 316, 108));

    //Create another layout and put it inside of this layout
    QGraphicsLayout *layout2;
    DuiLinearLayoutPolicy *policy2 = NULL;
    if (secondLayoutIsQ) {
        layout2 = new QGraphicsLinearLayout;
        static_cast<QGraphicsLinearLayout *>(layout2)->setSpacing(4);
    } else {
        layout2 = new DuiLayoutTest;
        policy2 = new DuiLinearLayoutPolicy(static_cast<DuiLayoutTest *>(layout2), Qt::Horizontal);
        policy2->setSpacing(4);
    }
    layout2->setContentsMargins(0, 0, 0, 0);
    //If "usewidget" has been set by the _data() function, then we put the second (inner) layout
    //inside of a widget, then put that widget into the outer layout
    QGraphicsWidget *inner_layout_widget = NULL;
    if (usewidget) {
        inner_layout_widget = new QGraphicsWidget;
        inner_layout_widget->setLayout(layout2);

        if (firstLayoutIsQ)
            static_cast<QGraphicsLinearLayout *>(layout)->addItem(inner_layout_widget);
        else
            policy->addItem(inner_layout_widget);
    } else {
        if (firstLayoutIsQ)
            static_cast<QGraphicsLinearLayout *>(layout)->addItem(layout2);
        else
            policy->addItem(layout2);
    }

    //Create 2 new items and put it inside the 'layout2' which is inside 'layout'
    QGraphicsWidget *itemInside1 = new QGraphicsWidget;
    itemInside1->setMinimumSize(100, 100);
    itemInside1->setPreferredSize(100, 100);
    itemInside1->setMaximumSize(100, 100);
    if (secondLayoutIsQ)
        static_cast<QGraphicsLinearLayout *>(layout2)->addItem(itemInside1);
    else
        policy2->addItem(itemInside1);
    QGraphicsWidget *itemInside2 = new QGraphicsWidget;
    itemInside2->setMinimumSize(100, 100);
    itemInside2->setPreferredSize(100, 100);
    itemInside2->setMaximumSize(100, 100);
    if (secondLayoutIsQ)
        static_cast<QGraphicsLinearLayout *>(layout2)->addItem(itemInside2);
    else
        policy2->addItem(itemInside2);

    //Set the contents margins to something predictable
    layout->setContentsMargins(4,4,4,4);
    layout2->setContentsMargins(0,0,0,0);

    qreal left, top, right, bottom;
    layout->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, 4.0); QCOMPARE(top, 4.0); QCOMPARE(right, 4.0); QCOMPARE(bottom, 4.0);
    layout2->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, 0.0); QCOMPARE(top, 0.0); QCOMPARE(right, 0.0); QCOMPARE(bottom, 0.0);

    //Check the relationships
    QVERIFY(itemInside1->parentLayoutItem() == layout2);
    QVERIFY(itemInside2->parentLayoutItem() == layout2);
    QVERIFY(item->parentLayoutItem() == layout);
    if (usewidget) {
        QVERIFY(layout2->parentLayoutItem() == inner_layout_widget);
        QVERIFY(inner_layout_widget->parentLayoutItem() == layout);
    } else
        QVERIFY(layout2->parentLayoutItem() == layout);
    QVERIFY(layout->parentLayoutItem() == form);

    //Check that the items are on the scene
    QVERIFY(form->scene() == m_scene);
    QVERIFY(item->scene() == m_scene);
    QVERIFY(itemInside1->scene() == m_scene);
    QVERIFY(itemInside2->scene() == m_scene);
    if (usewidget)
        QVERIFY(inner_layout_widget->scene() == m_scene);
    //Wait for all the animations to finish
    qApp->processEvents();
    while ((!firstLayoutIsQ && dynamic_cast<DuiLayout *>(layout)->animation()->isAnimating()) ||
            (!secondLayoutIsQ && dynamic_cast<DuiLayout *>(layout2)->animation()->isAnimating()))
        QTest::qWait(50);

    // we calculate x values for tests with this formula in a rtl-supporting way:
    // x = orig + ( directionFactor * oldXValue ) + ( widthFactor * widthOfElement )

    int orig = 0; // the origin x value for calculations, left side (zero) in ltr
    int wF = 0; // the width factor for the element to check, zero for ltr,
    int dF = 1; // the direction factor for the element, 1 for ltr

    int origI = 0; // origin of inner layout

    if (reverse) {
        orig = 316; // right side of the outer widget aka width of it
        wF = -1; // subtract width from orig here
        dF = -1; // calculate in rtl direction

        origI = 204;
    }

    //Check the positions of the items
    QCOMPARE(form->geometry(), QRectF(form_origin.x(), form_origin.y(), 316, 108));
    QCOMPARE(layout->geometry(), QRectF(0, 0, 316, 108));
    QCOMPARE(item->geometry(), QRectF(orig + (dF * 4) + (wF * 100), 4, 100, 100));
    QCOMPARE(layout2->effectiveSizeHint(Qt::MinimumSize), QSizeF(204, 100));
    QCOMPARE(form->mapToScene(0, 0), form_origin);
    QVERIFY(form->sceneTransform() == QTransform(scalefactor, 0, 0, 0, scalefactor, 0, form_origin.x(), form_origin.y(), 1));
    //Because geometry coordinates are relative to the parent QGraphicsItem, the coordinates
    //are different for the inner items depending on whether we put the inner layout inside of
    //QGraphicsWidget or not
    if (usewidget) {
        QCOMPARE(inner_layout_widget->geometry(), QRectF(orig + (dF * 108) + (wF * 204), 4, 204, 100));
        QCOMPARE(layout2->geometry(), QRectF(0, 0, 204, 100));
        QCOMPARE(itemInside1->geometry(), QRectF(origI + (wF * 100), 0, 100, 100));
        QCOMPARE(itemInside2->geometry(), QRectF(origI + (wF * 100) + (dF * 104), 0, 100, 100));
        QCOMPARE(form->mapRectToScene(inner_layout_widget->geometry()),
                 QRectF(QPointF(orig + (wF * 204) + (dF * 108), 4)*scalefactor + form_origin,
                        QSizeF(204, 100)*scalefactor));
        QCOMPARE(inner_layout_widget->mapRectToScene(itemInside1->geometry()),
                 QRectF(QPointF(orig + (wF * 100) + (dF * 108), 4)*scalefactor + form_origin,
                        QSizeF(100, 100)*scalefactor));
        QCOMPARE(inner_layout_widget->mapRectToScene(itemInside2->geometry()),
                 QRectF(QPointF(orig + (wF * 100) + (dF*(108 + 104)), 4)*scalefactor + form_origin,
                        QSizeF(100, 100)*scalefactor));
    } else {
        QCOMPARE(layout2->geometry(), QRectF(orig + (dF * 108) + (wF * 204), 4, 204, 100));
        QCOMPARE(itemInside1->geometry(), QRectF(orig + (dF * 108) + (wF * 100), 4, 100, 100));
        QCOMPARE(itemInside2->geometry(), QRectF(orig + (dF * 212) + (wF * 100), 4, 100, 100));
        QCOMPARE(form->mapRectToScene(layout2->geometry()),
                 QRectF(QPointF(orig + (dF * 108) + (wF * 204), 4)*scalefactor + form_origin,
                        QSizeF(204, 100)*scalefactor));
        QCOMPARE(form->mapRectToScene(itemInside1->geometry()),
                 QRectF(QPointF(orig + (dF * 108) + (wF * 100), 4)*scalefactor + form_origin,
                        QSizeF(100, 100)*scalefactor));
        QCOMPARE(form->mapRectToScene(itemInside2->geometry()),
                 QRectF(QPointF(orig + (dF*(108 + 104)) + (wF * 100), 4)*scalefactor + form_origin,
                        QSizeF(100, 100)*scalefactor));
    }
}

void Ut_DuiLayout::testReparenting()
{
    //create a widget with a layout inside a layout and an item in the layout
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);
    m_scene->addItem(form);

    DuiLayoutTest *layout2 = new DuiLayoutTest;

    QGraphicsWidget *item = new QGraphicsWidget;
    layout2->addItem(item);
    QVERIFY(!item->scene());
    //So we have a layout2 with an item inside, and now we add that to the outer layout
    layout->addItem(layout2);
    //This should have added all the items inside layout2 to the scene
    QVERIFY(item->scene());
}

//Test reparenting item but this time make item originally belong to the form.
void Ut_DuiLayout::testReparentingWithPreviousParent()
{
    //create a widget with a layout inside a layout and an item in the layout
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);
    m_scene->addItem(form);

    DuiLayoutTest *layout2 = new DuiLayoutTest;

    //Make the item a child of form
    QGraphicsWidget *item = new QGraphicsWidget(form);
    //Now when we add the item to the layout, it has to be removed as a parent of form first
    layout2->addItem(item);
    //So we have a layout2 with an item inside, and now we add that to the outer layout
    layout->addItem(layout2);
    //This should have added all the items inside layout2 to the scene
    QVERIFY(item->scene());
}

void Ut_DuiLayout::testLinearPolicyChangingOrientationBasic()
{
    GraphicsWidgetTest *form = new GraphicsWidgetTest;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);
    form->setGeometry(QRectF(0, 0, 204, 204));
    form->setMinimumWidth(204);//don't let it shrink when we have fixed sized items inside
    m_scene->addItem(form);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    QCOMPARE(policy->orientation(), Qt::Vertical);
    qreal vertical_spacing = 3;
    policy->setSpacing(3);
    layout->setContentsMargins(4, 4, 4, 4);

    const int num_items = 5;
    //Add items to policy, vertically
    for (int i = 0; i < num_items; i++) {
        QGraphicsWidget *item = new QGraphicsWidget;
        item->setMinimumSize(100, 100);
        policy->addItem(item);
        item->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    }

    qApp->processEvents();
    //After processing events, the animation should have started, but probably not finished
    QCOMPARE(layout->effectiveSizeHint(Qt::MinimumSize), QSizeF(100 + 4 + 4, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    QCOMPARE(form->effectiveSizeHint(Qt::MinimumSize), QSizeF(204, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    QCOMPARE(form->geometry(), QRectF(0, 0, 204, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    for (int i = 0; i < num_items; i++) {
        if (layout->d_ptr->states.at(i).isAnimationDone()) //probably wont be done, but if system really slow (e.g. running in valgrind) it can be finished
            QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
        else {
            QVERIFY(layout->d_ptr->states.at(i).isSet(DuiItemState::STATE_FLAG_SHOWING) ||
                    layout->d_ptr->states.at(i).isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN));
        }
        QRectF targetGeometry;
        //If the item can expand, its width will be expanded to the width of the layout - i.e. 204 pixels minus margins
        if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag))
            targetGeometry = QRectF(4, 4 + i * (100 + vertical_spacing), 204 - 4 - 4, 100);
        else
            targetGeometry = QRectF(4, 4 + i * (100 + vertical_spacing), 100, 100);

        QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), targetGeometry);
        QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(layout->d_ptr->states.at(i).item());
        QVERIFY(widget);
        QCOMPARE(widget->isVisible(), true);
        QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), targetGeometry.center());
    }
}
void Ut_DuiLayout::testLinearPolicyChangingOrientation_data()
{
    QTest::addColumn<bool>("waitForFirstAnimation");
    QTest::addColumn<bool>("useFixedSizePolicy");
    QTest::addColumn<bool>("reverse");

    QTest::newRow("LTR Wait for first animation to be completed") << true << false << false;
    QTest::newRow("LTR Wait for first animation to be completed and use fixed size policy") << true << true << false;
    QTest::newRow("LTR Use fixed size policy") << false << true << false;
    QTest::newRow("LTR Use expanding size policy") << false << true << false;

    QTest::newRow("RTL Wait for first animation to be completed") << true << false << true;
    QTest::newRow("RTL Wait for first animation to be completed and use fixed size policy") << true << true << true;
    QTest::newRow("RTL Use fixed size policy") << false << true << true;
    QTest::newRow("RTL Use expanding size policy") << false << true << true;
}

void Ut_DuiLayout::testLinearPolicyChangingOrientation()
{
    QFETCH(bool, waitForFirstAnimation);  //Whether to wait for the Qt::Vertical animation to complete before changing to Qt::Horizontal
    QFETCH(bool, useFixedSizePolicy); //Whether to set the items size policy to fixed
    QFETCH(bool, reverse); //Whether to use RTL layout direction
    qApp->setLayoutDirection(reverse ? Qt::RightToLeft : Qt::LeftToRight);

    GraphicsWidgetTest *form = new GraphicsWidgetTest;
    DuiLayoutTest *layout = new DuiLayoutTest;
    form->setLayout(layout);
    form->setGeometry(QRectF(0, 0, 204, 204));
    form->setMinimumWidth(204);//don't let it shrink when we have fixed sized items inside
    m_scene->addItem(form);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    QCOMPARE(policy->orientation(), Qt::Vertical);
    qreal vertical_spacing = 3;
    qreal horizontal_spacing = 3;
    policy->setSpacing(3);
    layout->setContentsMargins(4, 4, 4, 4);

    /* The policy contents margins should come from the CSS file which
     * specifies them to all be -1 */
    qreal left, top, right, bottom;
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, -1.0);
    QCOMPARE(top, -1.0);
    QCOMPARE(right, -1.0);
    QCOMPARE(bottom, -1.0);

    /* Since the policy contents margins are all -1, the layout contents margin should be
     * 4,4,4,4 that we just set it to */
    layout->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, 4.0);
    QCOMPARE(top, 4.0);
    QCOMPARE(right, 4.0);
    QCOMPARE(bottom, 4.0);

    const int num_items = 5;
    //Add items to policy, vertically
    for (int i = 0; i < num_items; i++) {
        QGraphicsWidget *item = new QGraphicsWidget;
        item->setMinimumSize(100, 100);
        policy->addItem(item);
        if (useFixedSizePolicy)
            item->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    }

    qApp->processEvents();
    //After processing events, the animation should have started, but probably not finished
    //Minimum size is the minimum width of an item plus its contents margins
    QCOMPARE(layout->effectiveSizeHint(Qt::MinimumSize), QSizeF(100 + 4 + 4, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    QCOMPARE(form->effectiveSizeHint(Qt::MinimumSize), QSizeF(204, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    QCOMPARE(form->geometry(), QRectF(0, 0, 204, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    for (int i = 0; i < num_items; i++) {
        if (layout->d_ptr->states.at(i).isAnimationDone()) //probably wont be done, but if system really slow (e.g. running in valgrind) it can be finished
            QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
        else {
            QVERIFY(layout->d_ptr->states.at(i).isSet(DuiItemState::STATE_FLAG_SHOWING) ||
                    layout->d_ptr->states.at(i).isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN));
        }
        QRectF targetGeometry;
        //If the item can expand, its width will be expanded to the width of the layout - i.e. 204 pixels minus margins
        if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag))
            targetGeometry = QRectF(4, 4 + i * (100 + vertical_spacing), 204 - 4 - 4, 100);
        else
            targetGeometry = QRectF(4, 4 + i * (100 + vertical_spacing), 100, 100);

        QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), targetGeometry);
        QCOMPARE(dynamic_cast<QGraphicsWidget *>(layout->d_ptr->states.at(i).item())->isVisible(), true);
        QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), targetGeometry.center());
    }
    if (waitForFirstAnimation) {
        while (layout->animation()->isAnimating()) {
            QTest::qWait(50);
        }
        //Animation is now done - the items should be in the correct place
        for (int i = 0; i < num_items; i++) {
            QCOMPARE(layout->d_ptr->states.at(i).geometryProgress(), 1.0);
            QCOMPARE(layout->d_ptr->states.at(i).opacityProgress(), 1.0);
            QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
            if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag)) {
                //If the item can expand, its width will be expanded to the width of the layout - i.e. 204 pixels minus margins
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4, 4 + i*(100 + vertical_spacing), 204 - 4 - 4, 100));
                QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), QRectF(4, 4 + i*(100 + vertical_spacing), 204 - 4 - 4, 100).center());
            } else {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4, 4 + i*(100 + vertical_spacing), 100, 100));
                QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), QRectF(4, 4 + i*(100 + vertical_spacing), 100, 100).center());
            }

        }
    }

    //Now we change the orientation, and make sure that we move all the items to the new position
    policy->setOrientation(Qt::Horizontal);
    QCOMPARE(policy->orientation(), Qt::Horizontal);

    qApp->processEvents();

    for (int i = 0; i < num_items; i++) {
        if (waitForFirstAnimation)
            QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
        if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag)) {
            //If the item can expand, its height will be expanded to the height of the layout which was made larger when in horizontal mode
            qreal height = 100 * num_items + vertical_spacing * (num_items - 1);
            if (! reverse) {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(),
                         QRectF(4 + i*(100 + horizontal_spacing), 4, 100, height));
            } else {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(),
                         QRectF(4 + (num_items - i - 1) *(100 + horizontal_spacing), 4, 100, height));
            }
        } else {
            if (! reverse) {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(),
                         QRectF(4 + i*(100 + horizontal_spacing), 4, 100, 100));
            } else {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(),
                         QRectF(4 + (num_items - 1 - i)*(100 + horizontal_spacing), 4, 100, 100));
            }
        }
    }

    //wait for the animation to finish, so that the items should now be in the right position
    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    for (int i = 0; i < num_items; i++) {
        QCOMPARE(layout->d_ptr->states.at(i).geometryProgress(), 1.0);
        QCOMPARE(layout->d_ptr->states.at(i).opacityProgress(), 1.0);
        QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
        if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag)) {
            //If the item can expand, its height will be expanded to the height of the layout which was made larger when in horizontal mode
            qreal height = 100 * num_items + vertical_spacing * (num_items - 1);
            if (! reverse) {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4 + i*(100 + horizontal_spacing), 4, 100, height));
                QCOMPARE(layout->d_ptr->states.at(i).item()->geometry(), QRectF(4 + i*(100 + horizontal_spacing), 4, 100, height));
            } else {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4 + (num_items - i - 1)*(100 + horizontal_spacing), 4, 100, height));
                QCOMPARE(layout->d_ptr->states.at(i).item()->geometry(), QRectF(4 + (num_items - i - 1)*(100 + horizontal_spacing), 4, 100, height));
            }
        } else {
            if (! reverse) {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4 + i*(100 + horizontal_spacing), 4, 100, 100));
                QCOMPARE(layout->d_ptr->states.at(i).item()->geometry(), QRectF(4 + i*(100 + horizontal_spacing), 4, 100, 100));
            } else {
                QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4 + (num_items - i - 1)*(100 + horizontal_spacing), 4, 100, 100));
                QCOMPARE(layout->d_ptr->states.at(i).item()->geometry(), QRectF(4 + (num_items - i - 1)*(100 + horizontal_spacing), 4, 100, 100));
            }
        }

    }
}

void Ut_DuiLayout::testGridLayoutShaking_data()
{
    QTest::addColumn<bool>("testMinimumSize"); //Whether to test the minimum size or preferred size
    QTest::addColumn<bool>("reverse"); //Whether to test RTL layout direction
    QTest::newRow("LTR testing layout grows to correct minimum size") << true << false;
    QTest::newRow("LTR testing layouting in preferred size") << false << false;
    QTest::newRow("RTL testing layout grows to correct minimum size") << true << true;
    QTest::newRow("RTL testing layouting in preferred size") << false << true;
}

void Ut_DuiLayout::testGridLayoutShaking()
{
    QFETCH(bool, testMinimumSize);  //Whether to test the minimum size or preferred size
    QFETCH(bool, reverse); //Whether to use RTL layout direction
    qApp->setLayoutDirection(reverse ? Qt::RightToLeft : Qt::LeftToRight);

    QGraphicsWidget form;
    QPixmap pixmap(10, 10);
    DuiLayoutTest *layout = new DuiLayoutTest();
    form.setLayout(layout);
    layout->setContentsMargins(1, 1, 1, 1);
    DuiGridLayoutPolicy *policy = new DuiGridLayoutPolicy(layout);
    policy->setSpacing(1);
    int num_rows = 6;

    int orig = 0; // the origin x value for calculations, left side (zero) in ltr
    int wF = 0; // the width factor for the element to check, zero for ltr,
    int dF = 1; // the direction factor for the element, 1 for ltr

    if (reverse) {
        orig = 203; // right side of the outer widget aka width of it
        wF = -1; // subtract width from orig here
        dF = -1; // calculate in rtl direction
    }

    /* Basically we are creating a grid with 2 columns and num_rows rows.
     * The items in the first row determine the minimum width of the column
     * and the items in the first column determine the minimum height of the rows
     */
    for (int y = 0; y < num_rows; y++) {
        QGraphicsWidget *widget = new DuiLabel;
        QGraphicsWidget *widget2 = new DuiLabel;
        widget->setMinimumSize(0, 0);
        widget2->setMinimumSize(0, 0);

        if (testMinimumSize) {
            if (y == 0) { //let the first item determine the size for all of them in the row
                widget->setMinimumSize(100, 10);
                widget2->setMinimumWidth(100);
            } else
                widget->setMinimumHeight(10);
        } else {
            widget->setPreferredSize(100, 10);
            widget2->setPreferredSize(100, 20);
        }

        policy->addItem(widget, y, 0);
        policy->addItem(widget2, y, 1);

        widget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
        widget2->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    }
    /* Test and set the size.  If testing the minimum size, the form will grow by itself
     * in the event loop.
     * For testing preferred size, we set the size explicitly */
    int itemHeight;
    if (testMinimumSize) {
        itemHeight = 10;
        QRectF expectedGeometry(0, 0, 1 + 100 + 1 + 100 + 1, 1 + num_rows * 10 + (num_rows - 1) * 1 + 1);
        QCOMPARE(layout->minimumSize(), expectedGeometry.size());
        QCOMPARE(layout->effectiveSizeHint(Qt::MinimumSize), expectedGeometry.size());
        qApp->processEvents();
        QCOMPARE(layout->geometry(), expectedGeometry);
        QCOMPARE(form.geometry(), expectedGeometry);
    } else {
        itemHeight = 20;
        QRectF preferredGeometry(0, 0, 1 + 100 + 1 + 100 + 1, 1 + num_rows * 20 + (num_rows - 1) * 1 + 1);
        QCOMPARE(layout->effectiveSizeHint(Qt::PreferredSize), preferredGeometry.size());
        QCOMPARE(form.effectiveSizeHint(Qt::PreferredSize), preferredGeometry.size());
        form.setGeometry(preferredGeometry);
        QCOMPARE(layout->geometry(), preferredGeometry);
    }

    /* Check that all the items were laid out correctly
     * The animation is still being done, so just check target geometries and centers
     */
    for (int y = 0; y < num_rows; y++) {
        QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(policy->itemAt(y, 0));
        DuiItemState itemState = layout->d_ptr->states.at(layout->indexOf((QGraphicsItem *)widget));
        QVERIFY(itemState.item() == widget);

        QCOMPARE(itemState.targetGeometry(), QRectF(orig + (wF * 100) + (dF * 1), 1 + (itemHeight + 1)*y, 100, itemHeight));
        QCOMPARE(widget->geometry().center(), QRectF(orig + (wF * 100) + (dF * 1), 1 + (itemHeight + 1)*y, 100, itemHeight).center());

        QGraphicsWidget *widget2 = dynamic_cast<QGraphicsWidget *>(policy->itemAt(y, 1));
        DuiItemState itemState2 = layout->d_ptr->states.at(layout->indexOf((QGraphicsItem *)widget2));
        QVERIFY(itemState2.item() == widget2);

        QCOMPARE(itemState2.targetGeometry(),  QRectF(orig + (wF * 100) + (dF*(1 + 100 + 1)), 1 + (itemHeight + 1)*y, 100, itemHeight));
        QCOMPARE(widget2->geometry().center(), QRectF(orig + (wF * 100) + (dF*(1 + 100 + 1)), 1 + (itemHeight + 1)*y, 100, itemHeight).center());
    }
    while (layout->animation()->isAnimating())
        QTest::qWait(50);
    qApp->processEvents();  //make sure that there really are no outstanding events
    Q_ASSERT(!layout->animation()->isAnimating());
    /* Check that all the items are laid are correctly, this time the animation is finished so their
     * actual geometry should be correct */
    for (int y = 0; y < num_rows; y++) {
        QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(policy->itemAt(y, 0));
        DuiItemState itemState = layout->d_ptr->states.at(layout->indexOf((QGraphicsItem *)widget));
        QVERIFY(itemState.item() == widget);
        QCOMPARE(itemState.geometryProgress(), 1.0);
        QCOMPARE(widget->geometry(), QRectF(orig + (wF * 100) + (dF * 1), 1 + (itemHeight + 1)*y, 100, itemHeight));

        QGraphicsWidget *widget2 = dynamic_cast<QGraphicsWidget *>(policy->itemAt(y, 1));
        DuiItemState itemState2 = layout->d_ptr->states.at(layout->indexOf((QGraphicsItem *)widget2));
        QVERIFY(itemState2.item() == widget2);
        QCOMPARE(itemState2.geometryProgress(), 1.0);
        QCOMPARE(widget2->geometry(), QRectF(orig + (wF * 100) + (dF*(1 + 100 + 1)), 1 + (itemHeight + 1)*y, 100, itemHeight));
    }
}

void Ut_DuiLayout::testBasicAnimationWithLayoutInsideLayout_data()
{
    QTest::addColumn<bool>("useFixedSizePolicy");
    QTest::newRow("Using preferred size policy") << false;
    QTest::newRow("Using fixed size policy") << true;
}
void Ut_DuiLayout::testBasicAnimationWithLayoutInsideLayout()
{
    QFETCH(bool, useFixedSizePolicy);
    GraphicsWidgetTest *form = new GraphicsWidgetTest;
    DuiLayoutTest *layout = new DuiLayoutTest;
    QVERIFY(layout->animation());
    QVERIFY(dynamic_cast<DuiBasicLayoutAnimation *>(layout->animation()));
    form->setLayout(layout);
    form->setGeometry(QRectF(0, 0, 204, 204));
    form->setMinimumWidth(204);//don't let it shrink when we have fixed sized items inside
    m_scene->addItem(form);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    qreal vertical_spacing = 3;
    //qreal horizontal_spacing = 3;
    policy->setSpacing(vertical_spacing);
    policy->setContentsMargins(4, 4, 4, 4);

    int num_items = 5;
    //Add items to policy, vertically
    for (int i = 0; i < num_items - 1; i++) {
        QGraphicsWidget *item = new QGraphicsWidget;
        item->setMinimumSize(100, 100);
        policy->addItem(item);
        if (useFixedSizePolicy)
            item->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    }

    //Add one more item that is inside a layout
    DuiLayoutTest *layout_inside_layout = new DuiLayoutTest;
    QVERIFY(layout_inside_layout->animation());
    DuiLinearLayoutPolicy *inside_policy = new DuiLinearLayoutPolicy(layout_inside_layout, Qt::Vertical);
    inside_policy->setContentsMargins(0, 0, 0, 0);

    QGraphicsWidget *inside_item = new QGraphicsWidget;
    inside_item->setMinimumSize(100, 100);
    inside_policy->addItem(inside_item);
    if (useFixedSizePolicy)
        inside_item->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    policy->addItem(layout_inside_layout);

    //We now have 5 items plus 1 item inside its own layout
    //We want to check that the item inside its own layout behaves the same as the others

    qApp->processEvents();
    //After processing events, the animation should have started, but probably not finished
    QCOMPARE(layout_inside_layout->effectiveSizeHint(Qt::MinimumSize), QSizeF(100, 100));
    QCOMPARE(policy->count(), num_items);
    QCOMPARE(layout->effectiveSizeHint(Qt::MinimumSize), QSizeF(100 + 4 + 4, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    QCOMPARE(form->effectiveSizeHint(Qt::MinimumSize), QSizeF(204, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    QCOMPARE(form->geometry(), QRectF(0, 0, 204, 4 + 4 + 100 * num_items + vertical_spacing*(num_items - 1)));
    for (int i = 0; i < num_items; i++) {
        if (layout->d_ptr->states.at(i).isAnimationDone()) //probably wont be done, but if system really slow (e.g. running in valgrind) it can be finished
            QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
        else {
            QVERIFY(layout->d_ptr->states.at(i).isSet(DuiItemState::STATE_FLAG_SHOWING) ||
                    layout->d_ptr->states.at(i).isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN));
        }
        if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag) && !useFixedSizePolicy) {
            //If the item can expand, its width will be expanded to the width of the layout - i.e. 204 pixels minus margins
            QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4, 4 + i*(100 + vertical_spacing), 204 - 4 - 4, 100));
            QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), QRectF(4, 4 + i*(100 + vertical_spacing), 204 - 4 - 4, 100).center());
        } else {
            QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4, 4 + i*(100 + vertical_spacing), 100, 100));
            QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), QRectF(4, 4 + i*(100 + vertical_spacing), 100, 100).center());
        }
    }
    while (layout->animation()->isAnimating()) {
        qApp->processEvents();
    }
    //Animation is now done - the items should be in the correct place
    for (int i = 0; i < num_items; i++) {
        QCOMPARE(layout->d_ptr->states.at(i).geometryProgress(), 1.0);
        QCOMPARE(layout->d_ptr->states.at(i).opacityProgress(), 1.0);
        QCOMPARE(layout->d_ptr->states.at(i).flags(), DuiItemState::STATE_FLAG_SHOWING);
        if (layout->d_ptr->states.at(i).item()->sizePolicy().horizontalPolicy() & (QSizePolicy::GrowFlag | QSizePolicy::IgnoreFlag) && !useFixedSizePolicy) {
            //If the item can expand, its width will be expanded to the width of the layout - i.e. 204 pixels minus margins
            QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4, 4 + i*(100 + vertical_spacing), 204 - 4 - 4, 100));
            QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), QRectF(4, 4 + i*(100 + vertical_spacing), 204 - 4 - 4, 100).center());
        } else {
            QCOMPARE(layout->d_ptr->states.at(i).targetGeometry(), QRectF(4, 4 + i*(100 + vertical_spacing), 100, 100));
            QCOMPARE(layout->d_ptr->states.at(i).item()->geometry().center(), QRectF(4, 4 + i*(100 + vertical_spacing), 100, 100).center());
        }

    }
}
void Ut_DuiLayout::testBasicAnimationWithLayoutInsideLayout2_data()
{
    QTest::addColumn<bool>("withAnimation");
    QTest::addColumn<bool>("withInnerAnimation");
    for (int i = 0; i < 4; i++) {
        bool withAnimation = i & 1;
        bool withInnerAnimation = i & (1 << 1);
        QString description = QString("outer ") + (withAnimation ? "with" : "without") + " animation, inner " + (withInnerAnimation ? "with" : "without") + " animation";
        QTest::newRow(description.toLatin1()) << withAnimation << withInnerAnimation;
    }
}
void Ut_DuiLayout::testBasicAnimationWithLayoutInsideLayout2()
{
    /* Test the case of a layout within a layout using DuiLayout with animations.
     * Note that the next unit test, testQGraphicsLinearLayoutWithLayoutInsideLayout2(),
     * does the same thing but with QGraphicsLinearLayout, to check that we do the same
     * as Qt. */
    QFETCH(bool, withAnimation);
    QFETCH(bool, withInnerAnimation);
    GraphicsWidgetTest *form = new GraphicsWidgetTest;
    DuiLayoutTest *layout = new DuiLayoutTest;
    if (withAnimation) {
        QVERIFY(layout->animation());
        QVERIFY(dynamic_cast<DuiBasicLayoutAnimation *>(layout->animation()));
    } else {
        layout->setAnimation(NULL);
    }
    form->setLayout(layout);
    form->setGeometry(QRectF(0, 0, 204, 204));
    form->setMinimumWidth(204);
    form->setMaximumWidth(204);
    m_scene->addItem(form);

    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);

    /* Setup inner layout with a label */
    DuiLayoutTest *labelLayout = new DuiLayoutTest;
    if (!withInnerAnimation)
        labelLayout->setAnimation(NULL);
    labelLayout->setContentsMargins(0, 0, 0, 0);
    DuiLinearLayoutPolicy *labelPolicy = new DuiLinearLayoutPolicy(labelLayout, Qt::Horizontal);
    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setMinimumSize(10, 10);
    labelLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    labelPolicy->addItem(widget);

    policy->addItem(labelLayout);

    QCOMPARE(layout->count(), 1);
    QCOMPARE(labelLayout->count(), 1);
    QVERIFY(labelLayout->parentLayoutItem() == layout);

    qApp->processEvents(); //Start laying out items
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50); //Wait for animation to finish if there is one

    QCOMPARE(form->geometry(), QRectF(0, 0, 204, 204));
    QCOMPARE(layout->geometry(), QRectF(0, 0, 204, 204));
    QCOMPARE(labelLayout->geometry(), QRectF(0, 0, 204, 204));

    qApp->processEvents(); //make sure the inner layout is laid out
    while (withInnerAnimation && labelLayout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widget->geometry().width(), 204.0);
}
void Ut_DuiLayout::testQGraphicsLinearLayoutWithLayoutInsideLayout2()
{
    //This test does the same thing as testBasicAnimationWithLayoutInsideLayout2 but
    //using QGraphicsLinearLayout instead of DuiLayout, to compare results
    GraphicsWidgetTest *form = new GraphicsWidgetTest;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);

    form->setLayout(layout);
    form->setGeometry(QRectF(0, 0, 204, 204));
    form->setMinimumWidth(204);
    form->setMaximumWidth(204);
    m_scene->addItem(form);

    layout->setContentsMargins(0, 0, 0, 0);

    /* Setup inner layout */
    QGraphicsLinearLayout *labelLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    labelLayout->setContentsMargins(0, 0, 0, 0);
    labelLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setMinimumSize(10, 10);
    labelLayout->addItem(widget);

    layout->addItem(labelLayout);

    QCOMPARE(layout->count(), 1);
    QCOMPARE(labelLayout->count(), 1);
    QVERIFY(labelLayout->parentLayoutItem() == layout);

    qApp->processEvents();

    QCOMPARE(form->geometry(), QRectF(0, 0, 204, 204));
    QCOMPARE(layout->geometry(), QRectF(0, 0, 204, 204));
    QCOMPARE(labelLayout->geometry(), QRectF(0, 0, 204, 204));
    QCOMPARE(widget->geometry().width(), 204.0);
}

void Ut_DuiLayout::testLayoutPolicyStylingSimple()
{
    //This does a simple test to check that the layout is being restyled upon rotation
    //The following test, testLayoutPolicyStyling does a much more indepth test, but this
    //test is easier to follow if it fails
    qreal left, top, right, bottom;
    DuiLayoutTest *layout = new DuiLayoutTest(m_form);
    DuiAbstractLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);

    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle0);
    // The numbers which follow here are taken from the ut_duilayout.css file
    // if you change the css file, you need to keep them in sync:
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, -1.0);
    QCOMPARE(top, -1.0);
    QCOMPARE(right, -1.0);
    QCOMPARE(bottom, -1.0);

    //Now rotate - the values should be read in from the CSS file
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, 4.0);
    QCOMPARE(top, 5.0);
    QCOMPARE(right, 2.0);
    QCOMPARE(bottom, 6.0);
}

void Ut_DuiLayout::testLayoutPolicyStylingSimpleWithSceneManager()
{
    //This does a simple test to check that the layout is being restyled upon rotation
    //when there is a scene manager.
    //The following test, testLayoutPolicyStyling does a much more indepth test, but this
    //test is easier to follow if it fails
    qreal left, top, right, bottom;
    DuiLayoutTest *layout = new DuiLayoutTest(m_form);
    DuiAbstractLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);

    (void)DuiApplication::activeWindow()->sceneManager(); //Force the creation of a scene manager

    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle0);
    // The numbers which follow here are taken from the ut_duilayout.css file
    // if you change the css file, you need to keep them in sync:
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, -1.0);
    QCOMPARE(top, -1.0);
    QCOMPARE(right, -1.0);
    QCOMPARE(bottom, -1.0);

    //Now rotate - the values should be read in from the CSS file
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, 4.0);
    QCOMPARE(top, 5.0);
    QCOMPARE(right, 2.0);
    QCOMPARE(bottom, 6.0);
}

void Ut_DuiLayout::testLayoutPolicyStyling_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType");
    QTest::addColumn<bool>("isCurrent");
    QTest::newRow("DuiLinearLayoutPolicy, Horizontal as current") << "linearHorizontal" << true;
    QTest::newRow("DuiLinearLayoutPolicy, Vertical as current") << "linearVertical" << true;
    QTest::newRow("DuiGridLayoutPolicy as current") << "grid" << true;
    QTest::newRow("DuiFlowLayoutPolicy as current") << "flow" << true;
    QTest::newRow("DuiLinearLayoutPolicy, Horizontal not current") << "linearHorizontal" << false;
    QTest::newRow("DuiLinearLayoutPolicy, Vertical not current") << "linearVertical" << false;
    QTest::newRow("DuiGridLayoutPolicy not current") << "grid" << false;
    QTest::newRow("DuiFlowLayoutPolicy not current") << "flow" << false;
}

void checkPolicies(DuiLayoutTest *layout, DuiAbstractLayoutPolicy *policy, bool isCurrent, const QString &policyType, qreal marginLeft, qreal marginTop, qreal marginRight, qreal marginBottom, qreal verticalSpacing, qreal horizontalSpacing)
{
    qreal left, top, right, bottom;
    // set the contentsMargins of the layout to find out whether the
    // policy inherits these. The values for the contents
    // margins of the DuiAbstractLayoutPolicy in css are negative
    // which means to use the margins of the DuiLayout.
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, marginLeft);
    QCOMPARE(top, marginTop);
    QCOMPARE(right, marginRight);
    QCOMPARE(bottom, marginBottom);
    //We may have some negative numbers meaning that layout should ignore them
    layout->setContentsMargins(3.0, 3.0, 3.0, 3.0);
    if (!isCurrent)
        layout->policy()->setContentsMargins(-1, -1, -1, -1);
    layout->getContentsMargins(&left, &top, &right, &bottom);
    if (isCurrent) {
        QCOMPARE(left, (marginLeft < 0) ? 3.0 : marginLeft);
        QCOMPARE(top, (marginTop < 0) ? 3.0 : marginTop);
        QCOMPARE(right, (marginRight < 0) ? 3.0 : marginRight);
        QCOMPARE(bottom, (marginBottom < 0) ? 3.0 : marginBottom);
    } else {
        QCOMPARE(left, 3.0);
        QCOMPARE(top, 3.0);
        QCOMPARE(right, 3.0); // The policy values override
        QCOMPARE(bottom, 3.0);
    }

    // now set the contentsMargins in code and see if the values from
    // the css can be overridden:
    policy->setContentsMargins(9.0, 9.0, 9.0, -9.0);
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, 9.0);
    QCOMPARE(top, 9.0);
    QCOMPARE(right, 9.0);
    QCOMPARE(bottom, -9.0);
    layout->getContentsMargins(&left, &top, &right, &bottom);
    if (isCurrent) {
        QCOMPARE(left, 9.0);
        QCOMPARE(top, 9.0);
        QCOMPARE(right, 9.0); // The policy values override
        QCOMPARE(bottom, 3.0);
    } else {
        QCOMPARE(left, 3.0);
        QCOMPARE(top, 3.0);
        QCOMPARE(right, 3.0); // The policy values override
        QCOMPARE(bottom, 3.0);
    }

    // check whether the spacings are read correctly from the css style file:
    if (policyType == "linearHorizontal") {
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policy)->spacing(), horizontalSpacing);
        QCOMPARE(policy->horizontalSpacing(), horizontalSpacing);
        QCOMPARE(policy->verticalSpacing(), verticalSpacing);
        dynamic_cast<DuiLinearLayoutPolicy *>(policy)->setOrientation(Qt::Vertical);
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policy)->spacing(), verticalSpacing);
        QCOMPARE(policy->horizontalSpacing(), horizontalSpacing);
        QCOMPARE(policy->verticalSpacing(), verticalSpacing);
        dynamic_cast<DuiLinearLayoutPolicy *>(policy)->setOrientation(Qt::Horizontal);
    } else if (policyType == "linearVertical") {
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policy)->spacing(), verticalSpacing);
        QCOMPARE(policy->horizontalSpacing(), horizontalSpacing);
        QCOMPARE(policy->verticalSpacing(), verticalSpacing);
        dynamic_cast<DuiLinearLayoutPolicy *>(policy)->setOrientation(Qt::Horizontal);
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policy)->spacing(), horizontalSpacing);
        QCOMPARE(policy->horizontalSpacing(), horizontalSpacing);
        QCOMPARE(policy->verticalSpacing(), verticalSpacing);
        dynamic_cast<DuiLinearLayoutPolicy *>(policy)->setOrientation(Qt::Vertical);
    }

    QCOMPARE(policy->horizontalSpacing(), horizontalSpacing);
    QCOMPARE(policy->verticalSpacing(), verticalSpacing);

    //Reset back to the original values
    policy->unsetContentsMargins();
    //Check that they were restored correctly
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, marginLeft);
    QCOMPARE(top, marginTop);
    QCOMPARE(right, marginRight);
    QCOMPARE(bottom, marginBottom);

}

void Ut_DuiLayout::testLayoutPolicyStyling()
{
    QFETCH(QString, policyType); // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QFETCH(bool, isCurrent);

    DuiLayoutTest *layout = new DuiLayoutTest(m_form);
    DuiAbstractLayoutPolicy *policy;
    if (!isCurrent) {
        // with out a setPolicy, the policy added to the layout first
        // is the current policy
        DuiLinearLayoutPolicy *otherPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
        otherPolicy->setContentsMargins(-1, -1, -1, -1);
    }
    if (policyType == "grid")
        policy = new DuiGridLayoutPolicy(layout);
    else if (policyType == "linearHorizontal")
        policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    else if (policyType == "linearVertical")
        policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    else if (policyType == "flow")
        policy = new DuiFlowLayoutPolicy(layout);
    else {
        QVERIFY(false);
        return;
    }

    QCOMPARE(isCurrent, policy->isActive());

    // The objectName of the policy should be empty be default, make sure it is:
    QVERIFY(policy->objectName().isEmpty());

    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle0);

    // The numbers which follow here are taken from the ut_duilayout.css file
    // if you change the css file, you need to keep them in sync:
    checkPolicies(layout, policy, isCurrent, policyType, -1.0, -1.0, -1.0, -1.0, 6.0, 7.0);
    policy->setObjectName("packed");
    QCOMPARE(policy->objectName(), QString("packed"));
    checkPolicies(layout, policy, isCurrent, policyType, -1, 0.0, 0.0, 0.0, 0.0, 0.0);
    policy->setObjectName("spacing");
    QCOMPARE(policy->objectName(), QString("spacing"));
    checkPolicies(layout, policy, isCurrent, policyType, -1, 0.0, 0.0, 0.0, 10.0, 11.0);
    policy->setObjectName("margins");
    QCOMPARE(policy->objectName(), QString("margins"));
    checkPolicies(layout, policy, isCurrent, policyType, 10.0, 10.0, -1.0, 10.0, 0.0, 0.0);
    policy->setObjectName("spacing+margins");
    QCOMPARE(policy->objectName(), QString("spacing+margins"));
    checkPolicies(layout, policy, isCurrent, policyType, 10.0, -1.0, 10.0, 10.0, 10.0, 11.0);
    policy->setObjectName(QString::null);
    QVERIFY(policy->objectName().isEmpty());
    checkPolicies(layout, policy, isCurrent, policyType, -1.0, -1.0, -1.0, -1.0, 6.0, 7.0);

    // Now rotate to portrait
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);
    checkPolicies(layout, policy, isCurrent, policyType, 4.0, 5.0, 2.0, 6.0, 2.0, 3.0);
    policy->setObjectName("packed");
    QCOMPARE(policy->objectName(), QString("packed"));
    checkPolicies(layout, policy, isCurrent, policyType, 1, 2.0, 3.0, 4.0, 5.0, 6.0);

    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle180, Dui::ImmediateOrientationChange);
    QCOMPARE(policy->objectName(), QString("packed"));
    checkPolicies(layout, policy, isCurrent, policyType, -1, 0.0, 0.0, 0.0, 0.0, 0.0);
    policy->setObjectName("spacing");
    QCOMPARE(policy->objectName(), QString("spacing"));
    checkPolicies(layout, policy, isCurrent, policyType, -1, 0.0, 0.0, 0.0, 10.0, 11.0);

    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle270, Dui::ImmediateOrientationChange);
    QCOMPARE(policy->objectName(), QString("spacing"));
    checkPolicies(layout, policy, isCurrent, policyType, -1, 0.0, 0.0, 0.0, 10.0, 11.0);

    policy->setObjectName("packed");
    QCOMPARE(policy->objectName(), QString("packed"));
    checkPolicies(layout, policy, isCurrent, policyType, 1, 2.0, 3.0, 4.0, 5.0, 6.0);

    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle0, Dui::ImmediateOrientationChange);
    policy->setObjectName(QString::null);
    QVERIFY(policy->objectName().isEmpty());
    checkPolicies(layout, policy, isCurrent, policyType, -1.0, -1.0, -1.0, -1.0, 6.0, 7.0);
}

void Ut_DuiLayout::testLayoutSwitchingWithOrientation()
{
    //Test rotating the screen to make sure that the landscape/portrait policy is always correctly set
    //This was written in response to NB#144347 but the test passes, without finding any problems
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle0);
    DuiLayoutTest *layout = new DuiLayoutTest(m_form);
    DuiAbstractLayoutPolicy *landscape_policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    QVERIFY(layout->policy() == landscape_policy); //Only policy, so it must be this one
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle90);
    DuiAbstractLayoutPolicy *portrait_policy = new DuiGridLayoutPolicy(layout);
    QVERIFY(layout->policy() == landscape_policy); //Still only policy, so it must be this one

    layout->setPortraitPolicy(portrait_policy);
    QVERIFY(layout->policy() == portrait_policy);

    layout->setLandscapePolicy(landscape_policy);
    QVERIFY(layout->policy() == portrait_policy);

    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange); //Set it to 90, again
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle90);
    QVERIFY(layout->policy() == portrait_policy);
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle180, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle180);
    QVERIFY(layout->policy() == landscape_policy);
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle270, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle270);
    QVERIFY(layout->policy() == portrait_policy);
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle90);
    QVERIFY(layout->policy() == portrait_policy);
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle0, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle0);
    QVERIFY(layout->policy() == landscape_policy);
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle0, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle0);
    QVERIFY(layout->policy() == landscape_policy);
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle180, Dui::ImmediateOrientationChange);
    QCOMPARE(DuiApplication::activeWindow()->orientationAngle(), Dui::Angle180);
    QVERIFY(layout->policy() == landscape_policy);

}
void Ut_DuiLayout::testLayoutBoundingBox_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType");
    QTest::newRow("DuiLinearLayoutPolicy, Horizontal") << "linearHorizontal";
    QTest::newRow("DuiLinearLayoutPolicy, Vertical") << "linearVertical";
    QTest::newRow("DuiGridLayoutPolicy") << "grid";
    QTest::newRow("DuiFlowLayoutPolicy") << "flow";
    QTest::newRow("DuiFreestyleLayoutPolicy") << "freestyle";
}

void Ut_DuiLayout::testLayoutBoundingBox()
{
    QFETCH(QString, policyType); // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...

    qsrand(0);
    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    QGraphicsLinearLayout *outerLayout = new QGraphicsLinearLayout();
    outerLayout->setContentsMargins(10.0, 10.0, 10.0, 10.0);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(outerLayout);
    outerLayout->addItem(layout);
    DuiAbstractLayoutPolicy *policy;
    if (policyType == "grid")
        policy = new DuiGridLayoutPolicy(layout);
    else if (policyType == "flow")
        policy = new DuiFlowLayoutPolicy(layout);
    else if (policyType == "linearHorizontal")
        policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    else if (policyType == "linearVertical")
        policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    else if (policyType == "freestyle")
        policy = new DuiFreestyleLayoutPolicy(layout);
    else {
        QVERIFY(false); // should never happen
        return;
    }

    policy->setContentsMargins(20.0, 20.0, 20.0, 20.0);
    QList<QGraphicsWidget *> widgets;
    int widgetsMax = 10;
    for (int i = 0; i < widgetsMax; ++i) {
        widgets << new QGraphicsWidget();
        qreal width = ((100.0 * qrand()) / RAND_MAX) + 1;
        qreal height = ((100.0 * qrand()) / RAND_MAX) + 1;
        widgets[i]->setMinimumSize(QSizeF(width, height));
        if (policyType == "grid") {
            int row = i % 3;
            int col = i / 3;
            dynamic_cast<DuiGridLayoutPolicy *>(policy)->addItem(
                widgets[i], row, col);
        } else if (policyType == "linearHorizontal" || policyType == "linearVertical")
            dynamic_cast<DuiLinearLayoutPolicy *>(policy)->addItem(widgets[i]);
        else if (policyType == "flow")
            dynamic_cast<DuiFlowLayoutPolicy *>(policy)->addItem(widgets[i]);
        else if (policyType == "freestyle") {
            // I use a geometry with a fixed width and height here on purpose.
            // The item has a randomized minimum width and height (see above),
            // i.e. the item might resize if I add it to the layout using a
            // fixed geometry.
            //
            // I also use a fixed position in the geometry on purpose,
            // if an item is added at a geometry overlapping an
            // already existing item, the layout policy should move
            // the items so that they don't overlap anymore. In this
            // unit test we check that the final geometry of the items
            // is still within the contents area of the layout.
            QRectF geom(20.0, 20.0, 20, 20.0);
            dynamic_cast<DuiFreestyleLayoutPolicy *>(policy)->addItemAtGeometry(
                widgets[i], geom);
        } else
            QVERIFY(false); // should never happen
    }
    qApp->processEvents();
    qApp->processEvents();
    qreal marginLeft, marginRight, marginTop, marginBottom;
    layout->getContentsMargins(&marginLeft, &marginRight,
                               &marginTop, &marginBottom);
    // layoutContentsRect is area in the layout within the margins, i.e. the
    // area which should contain the items. We remove a tiny amount from
    // the margins because otherwise the test sometimes fails because of rounding
    // errors.
    QRectF layoutContentsRect =
        layout->geometry().adjusted(marginLeft - 0.0001,
                                    marginTop - 0.0001,
                                    -marginRight + 0.0001,
                                    -marginBottom + 0.0001);

    foreach(QGraphicsWidget * w, widgets) {
        //qDebug() << "Ut_DuiLayout::testLayoutBoundingBox: policyType, outerLayout, layout, layoutContentsRect, widget geometry" << policyType << outerLayout->geometry() << layout->geometry() << layoutContentsRect << w->geometry();
        QCOMPARE(w->geometry(), layout->d_ptr->states.at(layout->indexOf(w)).targetGeometry());
        QVERIFY(layoutContentsRect.contains(w->geometry()));
    }
    delete(layoutContainer);
}

void Ut_DuiLayout::testLayoutAddItemWithTwoPolicies_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType1");
    QTest::addColumn<QString>("policyType2");
    QTest::newRow("linearHorizontal, linearHorizontal") << "linearHorizontal" << "linearHorizontal";
    QTest::newRow("linearHorizontal, linearVertical") << "linearHorizontal" << "linearVertical";
    QTest::newRow("linearHorizontal, grid") << "linearHorizontal" << "grid";
    QTest::newRow("linearHorizontal, flow") << "linearHorizontal" << "flow";
    QTest::newRow("linearHorizontal, freestyle") << "linearHorizontal" << "freestyle";

    QTest::newRow("linearVertical, linearVertical") << "linearVertical" << "linearVertical";
    QTest::newRow("linearVertical, linearHorizontal") << "linearVertical" << "linearHorizontal";
    QTest::newRow("linearVertical, grid") << "linearVertical" << "grid";
    QTest::newRow("linearVertical, flow") << "linearVertical" << "flow";
    QTest::newRow("linearVertical, freestyle") << "linearVertical" << "freestyle";

    QTest::newRow("grid, grid") << "grid" << "grid";
    QTest::newRow("grid, linearHorizontal") << "grid" << "linearHorizontal";
    QTest::newRow("grid, linearVertical") << "grid" << "linearVertical";
    QTest::newRow("grid, flow") << "grid" << "flow";
    QTest::newRow("grid, freestyle") << "grid" << "freestyle";

    QTest::newRow("flow, flow") << "flow" << "flow";
    QTest::newRow("flow, linearHorizontal") << "flow" << "linearHorizontal";
    QTest::newRow("flow, linearVertical") << "flow" << "linearVertical";
    QTest::newRow("flow, grid") << "flow" << "grid";
    QTest::newRow("flow, freestyle") << "flow" << "freestyle";

    QTest::newRow("freestyle, freestyle") << "freestyle" << "freestyle";
    QTest::newRow("freestyle, linearHorizontal") << "freestyle" << "linearHorizontal";
    QTest::newRow("freestyle, linearVertical") << "freestyle" << "linearVertical";
    QTest::newRow("freestyle, grid") << "freestyle" << "grid";
    QTest::newRow("freestyle, flow") << "freestyle" << "flow";
}

void Ut_DuiLayout::testLayoutAddItemWithTwoPolicies()
{
    QFETCH(QString, policyType1);
    QFETCH(QString, policyType2);

    QList<QString> policyTypes;
    policyTypes << policyType1;
    policyTypes << policyType2;

    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(layout);

    QList<DuiAbstractLayoutPolicy *> policies;
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid")
            policies << new DuiGridLayoutPolicy(layout);
        else if (policyTypes[i] == "flow")
            policies << new DuiFlowLayoutPolicy(layout);
        else if (policyTypes[i] == "linearHorizontal")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
        else if (policyTypes[i] == "linearVertical")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Vertical);
        else if (policyTypes[i] == "freestyle")
            policies << new DuiFreestyleLayoutPolicy(layout);
    }

    QGraphicsWidget *widget = new QGraphicsWidget();

    // Neither of the two policies should have any items at the beginning:
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid") {
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->rowCount(), 0);
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->columnCount(), 0);
        } else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical") {
            QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->count(), 0);
        } else if (policyTypes[i] == "flow") {
            QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->count(), 0);
        } else if (policyTypes[i] == "freestyle") {
            QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->count(), 0);
        } else
            QVERIFY(false); // should never happen
    }

    // add item:
    layout->setPolicy(policies[0]);
    if (policyTypes[0] == "grid")
        dynamic_cast<DuiGridLayoutPolicy *>(policies[0])->addItem(widget, 0, 0);
    else if (policyTypes[0] == "linearHorizontal" || policyTypes[0] == "linearVertical")
        dynamic_cast<DuiLinearLayoutPolicy *>(policies[0])->addItem(widget);
    else if (policyTypes[0] == "flow")
        dynamic_cast<DuiFlowLayoutPolicy *>(policies[0])->addItem(widget);
    else if (policyTypes[0] == "freestyle")
        dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[0])->addItemAtGeometry(widget, QRectF(20, 20, 20, 20));
    else
        QVERIFY(false); // should never happen

    // check whether the item has been added correctly to the first policy:
    if (policyTypes[0] == "grid") {
        QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[0])->rowCount(), 1);
        QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[0])->columnCount(), 1);
        QVERIFY(NULL != dynamic_cast<DuiGridLayoutPolicy *>(policies[0])->itemAt(0, 0));
    } else if (policyTypes[0] == "linearHorizontal" || policyTypes[0] == "linearVertical") {
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[0])->count(), 1);
        QVERIFY(NULL != dynamic_cast<DuiLinearLayoutPolicy *>(policies[0])->itemAt(0));
    } else if (policyTypes[0] == "flow") {
        QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[0])->count(), 1);
        QVERIFY(NULL != dynamic_cast<DuiFlowLayoutPolicy *>(policies[0])->itemAt(0));
    } else if (policyTypes[0] == "freestyle") {
        QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[0])->count(), 1);
        QVERIFY(NULL != dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[0])->itemAt(0));
    } else
        QVERIFY(false); // should never happen

    // check if the item has been mistakenly added to the second policy:
    if (policyTypes[1] == "grid") {
        QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[1])->rowCount(), 0);
        QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[1])->columnCount(), 0);
    } else if (policyTypes[1] == "linearHorizontal" || policyTypes[1] == "linearVertical") {
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[1])->count(), 0);
    } else if (policyTypes[1] == "flow") {
        QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[1])->count(), 0);
    } else if (policyTypes[1] == "freestyle") {
        //stateless policies show all the items in the layout
    } else
        QVERIFY(false); // should never happen

    delete(layoutContainer);
}

void Ut_DuiLayout::testLayoutRemoveItemWithTwoPolicies_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType1");
    QTest::addColumn<QString>("policyType2");
    QTest::newRow("linearHorizontal, linearHorizontal") << "linearHorizontal" << "linearHorizontal";
    QTest::newRow("linearHorizontal, linearVertical") << "linearHorizontal" << "linearVertical";
    QTest::newRow("linearHorizontal, grid") << "linearHorizontal" << "grid";
    QTest::newRow("linearHorizontal, flow") << "linearHorizontal" << "flow";
    QTest::newRow("linearHorizontal, freestyle") << "linearHorizontal" << "freestyle";


    QTest::newRow("linearVertical, linearVertical") << "linearVertical" << "linearVertical";
    QTest::newRow("linearVertical, linearHorizontal") << "linearVertical" << "linearHorizontal";
    QTest::newRow("linearVertical, grid") << "linearVertical" << "grid";
    QTest::newRow("linearVertical, flow") << "linearVertical" << "flow";
    QTest::newRow("linearVertical, freestyle") << "linearVertical" << "freestyle";
    QTest::newRow("grid, grid") << "grid" << "grid";
    QTest::newRow("grid, linearHorizontal") << "grid" << "linearHorizontal";
    QTest::newRow("grid, linearVertical") << "grid" << "linearVertical";
    QTest::newRow("grid, flow") << "grid" << "flow";
    QTest::newRow("grid, freestyle") << "grid" << "freestyle";

    QTest::newRow("flow, flow") << "flow" << "flow";
    QTest::newRow("flow, linearHorizontal") << "flow" << "linearHorizontal";
    QTest::newRow("flow, linearVertical") << "flow" << "linearVertical";
    QTest::newRow("flow, grid") << "flow" << "grid";
    QTest::newRow("flow, freestyle") << "flow" << "freestyle";
}

void Ut_DuiLayout::testLayoutRemoveItemWithTwoPolicies()
{
    QFETCH(QString, policyType1);
    QFETCH(QString, policyType2);

    QList<QString> policyTypes;
    policyTypes << policyType1;
    policyTypes << policyType2;

    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(layout);

    QList<DuiAbstractLayoutPolicy *> policies;
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid")
            policies << new DuiGridLayoutPolicy(layout);
        else if (policyTypes[i] == "flow")
            policies << new DuiFlowLayoutPolicy(layout);
        else if (policyTypes[i] == "linearHorizontal")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
        else if (policyTypes[i] == "linearVertical")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Vertical);
        else if (policyTypes[i] == "freestyle")
            policies << new DuiFreestyleLayoutPolicy(layout);
    }
    // make first policy the current policy of the layout (it is already by default):
    layout->setPolicy(policies[0]);

    QGraphicsWidget *widget = new QGraphicsWidget();

    // Neither of the two policies should have any items at the beginning:
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid") {
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->rowCount(), 0);
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->columnCount(), 0);
        } else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical") {
            QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->count(), 0);
        } else if (policyTypes[i] == "flow") {
            QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->count(), 0);
        } else if (policyTypes[i] == "freestyle") {
            QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->count(), 0);
        } else
            QVERIFY(false); // should never happen
    }

    // add item to both policies:
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid")
            dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->addItem(widget, 0, 0);
        else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical")
            dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->addItem(widget);
        else if (policyTypes[i] == "flow")
            dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->addItem(widget);
        else if (policyTypes[i] == "freestyle")
            dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->addItemAtGeometry(widget, QRectF(20, 20, 20, 20));
        else
            QVERIFY(false); // should never happen
    }

    // check whether the item has been added correctly to both policies:
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid") {
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->rowCount(), 1);
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->columnCount(), 1);
        } else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical") {
            QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->count(), 1);
            QVERIFY(NULL != dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->itemAt(0));
        } else if (policyTypes[i] == "flow") {
            QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->count(), 1);
            QVERIFY(NULL != dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->itemAt(0));
        } else if (policyTypes[i] == "freestyle") {
            QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->count(), 1);
            QVERIFY(NULL != dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->itemAt(0));
        } else
            QVERIFY(false); // should never happen
    }

    // remove item from the layout and check whether this removes
    // it correctly from both policies:
    layout->removeItem(widget);
    // Both policies should again have no items now::
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid") {
        } else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical") {
            // the count of a linear layout policy doesn't go back
            // to 0 if all items are removed. Looks like a bug:
            QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->count(), 0);
        } else if (policyTypes[i] == "flow") {
            QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->count(), 0);
        } else if (policyTypes[i] == "freestyle") {
            QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->count(), 0);
        } else
            QVERIFY(false); // should never happen
    }

    // add item to both policies again:
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid")
            dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->addItem(widget, 0, 0);
        else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical")
            dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->addItem(widget);
        else if (policyTypes[i] == "flow")
            dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->addItem(widget);
        else if (policyTypes[i] == "freestyle")
            dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->addItemAtGeometry(widget, QRectF(20, 20, 20, 20));
        else
            QVERIFY(false); // should never happen
    }

    // check whether the item has been again been added correctly to both policies:
    for (int i = 0; i <= 1; ++i) {
        QCOMPARE(policies[i]->count(), 1);
        QVERIFY(NULL != policies[i]->itemAt(0));
        if (policyTypes[i] == "grid") {
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->rowCount(), 1);
            QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->columnCount(), 1);
            QVERIFY(NULL != dynamic_cast<DuiGridLayoutPolicy *>(policies[i])->itemAt(0, 0));
        } else if (policyTypes[i] == "linearHorizontal" || policyTypes[i] == "linearVertical") {
            QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->count(), 1);
            QVERIFY(NULL != dynamic_cast<DuiLinearLayoutPolicy *>(policies[i])->itemAt(0));
        } else if (policyTypes[i] == "flow") {
            QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->count(), 1);
            QVERIFY(NULL != dynamic_cast<DuiFlowLayoutPolicy *>(policies[i])->itemAt(0));
        } else if (policyTypes[i] == "freestyle") {
            QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->count(), 1);
            QVERIFY(NULL != dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[i])->itemAt(0));
        } else
            QVERIFY(false); // should never happen
    }

    // remove the item only from the first policy and check whether it has been
    // correctly removed:
    layout->setPolicy(policies[0]);
    policies[0]->removeItem(widget);
    QCOMPARE(policies[0]->count(), 0);

    // Currently there is no policy where removing items without
    // removing them from other policies as well works.
    //
    // The grid layout policy and the linear layout policy don't even
    // have a method to remove items at all, one can only call the
    // remove methods of the layout and as we already know this
    // removes the item from all policies.  This is apparently
    // intentional and we even tested above that this works.
    //
    // check if the item has been mistakenly removed from the second policy as well:
    if (policyTypes[1] == "grid") {
        QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[1])->rowCount(), 1);
        QCOMPARE(dynamic_cast<DuiGridLayoutPolicy *>(policies[1])->columnCount(), 1);
        QVERIFY(NULL != dynamic_cast<DuiGridLayoutPolicy *>(policies[1])->itemAt(0, 0));
    } else if (policyTypes[1] == "linearHorizontal" || policyTypes[1] == "linearVertical") {
        QCOMPARE(dynamic_cast<DuiLinearLayoutPolicy *>(policies[1])->count(), 1);
        QVERIFY(NULL != dynamic_cast<DuiLinearLayoutPolicy *>(policies[1])->itemAt(0));
    } else if (policyTypes[1] == "flow") {
        QCOMPARE(dynamic_cast<DuiFlowLayoutPolicy *>(policies[1])->count(), 1);
    } else if (policyTypes[1] == "freestyle") {
        QCOMPARE(dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[1])->count(), 1);
    } else
        QVERIFY(false); // should never happen

    delete widget;
    delete layoutContainer;
}
void Ut_DuiLayout::testForCrash()
{
    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(layout);

    DuiLinearLayoutPolicy *policy1 = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiLinearLayoutPolicy *policy2 = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    Q_UNUSED(policy2);

    QGraphicsWidget *widget = new QGraphicsWidget();
    policy1->addItem(widget);

    delete(layoutContainer);
    qApp->processEvents();
}

void Ut_DuiLayout::testLayoutItemPositionRestoreWhenSwitchingPolicies_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType1");
    QTest::addColumn<QString>("policyType2");
    QTest::newRow("linearHorizontal, linearHorizontal") << "linearHorizontal" << "linearHorizontal";
    QTest::newRow("linearHorizontal, linearVertical") << "linearHorizontal" << "linearVertical";
    QTest::newRow("linearHorizontal, grid") << "linearHorizontal" << "grid";
    QTest::newRow("linearHorizontal, flow") << "linearHorizontal" << "flow";
    QTest::newRow("linearHorizontal, freestyle") << "linearHorizontal" << "freestyle";

    QTest::newRow("linearVertical, linearVertical") << "linearVertical" << "linearVertical";
    QTest::newRow("linearVertical, linearHorizontal") << "linearVertical" << "linearHorizontal";
    QTest::newRow("linearVertical, grid") << "linearVertical" << "grid";
    QTest::newRow("linearVertical, flow") << "linearVertical" << "flow";
    QTest::newRow("linearVertical, freestyle") << "linearVertical" << "freestyle";
    QTest::newRow("grid, grid") << "grid" << "grid";
    QTest::newRow("grid, linearHorizontal") << "grid" << "linearHorizontal";
    QTest::newRow("grid, linearVertical") << "grid" << "linearVertical";
    QTest::newRow("grid, flow") << "grid" << "flow";
    QTest::newRow("grid, freestyle") << "grid" << "freestyle";

    QTest::newRow("flow, flow") << "flow" << "flow";
    QTest::newRow("flow, linearHorizontal") << "flow" << "linearHorizontal";
    QTest::newRow("flow, linearVertical") << "flow" << "linearVertical";
    QTest::newRow("flow, grid") << "flow" << "grid";
    QTest::newRow("flow, freestyle") << "flow" << "freestyle";
}

void Ut_DuiLayout::testLayoutItemPositionRestoreWhenSwitchingPolicies()
{
    QFETCH(QString, policyType1);
    QFETCH(QString, policyType2);

    QList<QString> policyTypes;
    policyTypes << policyType1;
    policyTypes << policyType2;

    qsrand(0);
    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(layout);

    QList<DuiAbstractLayoutPolicy *> policies;
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid")
            policies << new DuiGridLayoutPolicy(layout);
        else if (policyTypes[i] == "flow")
            policies << new DuiFlowLayoutPolicy(layout);
        else if (policyTypes[i] == "linearHorizontal")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
        else if (policyTypes[i] == "linearVertical")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Vertical);
        else if (policyTypes[i] == "freestyle")
            policies << new DuiFreestyleLayoutPolicy(layout);
        else
            QVERIFY(false);
    }
    policies[0]->setContentsMargins(20.0, 20.0, 20.0, 20.0);
    policies[1]->setContentsMargins(20.0, 20.0, 20.0, 20.0);

    int widgetsMax = 10;
    QList<QGraphicsWidget *> widgets;
    for (int policyIndex = 0; policyIndex <= 1; ++policyIndex) {
        // if items are added to a non-active policy, they should get negative
        // coordinates to be invisible. That does not seem to work, at least I found
        // that it does not  work for the DuiGridLayoutPolicy. This is probably a bug.
        // I wrote an extra unittest for that bug.
        // For the test here, I make the policies active before adding items.
        layout->setPolicy(policies[policyIndex]);
        for (int i = 0; i < widgetsMax; ++i) {
            int widgetIndex = i + policyIndex * widgetsMax;
            widgets << new QGraphicsWidget();
            qreal width = ((100.0 * qrand()) / RAND_MAX) + 1;
            qreal height = ((100.0 * qrand()) / RAND_MAX) + 1;
            widgets[widgetIndex]->setMinimumSize(QSizeF(width, height));
            if (policyTypes[policyIndex] == "grid") {
                int row = i % 3;
                int col = i / 3;
                dynamic_cast<DuiGridLayoutPolicy *>(policies[policyIndex])->addItem(
                    widgets[widgetIndex], row, col);
                qApp->processEvents();
            } else if (policyTypes[policyIndex] == "linearHorizontal"
                       || policyTypes[policyIndex] == "linearVertical") {
                dynamic_cast<DuiLinearLayoutPolicy *>(policies[policyIndex])->addItem(widgets[widgetIndex]);
            } else if (policyTypes[policyIndex] == "flow") {
                dynamic_cast<DuiFlowLayoutPolicy *>(policies[policyIndex])->addItem(widgets[widgetIndex]);
            } else if (policyTypes[policyIndex] == "freestyle") {
                // I use a geometry with a fixed width and height here on purpose.
                // The item has a randomized minimum width and height (see above),
                // i.e. the item might resize if I add it to the layout using a
                // fixed geometry.
                //
                // I also use a fixed position in the geometry on purpose,
                // if an item is added at a geometry overlapping an
                // already existing item, the layout policy should move
                // the items so that they don't overlap anymore.
                QRectF geom(20.0, 20.0, 20, 20.0);
                dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[policyIndex])->addItemAtGeometry(
                    widgets[widgetIndex], geom);
            } else
                QVERIFY(false); // should never happen
        }
    }

    // remember the geometry of all widgets when policy 0 is active:
    layout->setPolicy(policies[0]);
    layoutContainer->setGeometry(rect);
    rect = layoutContainer->geometry();
    qApp->processEvents();
    QList<QRectF> geometries0;
    foreach(QGraphicsWidget * w, widgets) {
        geometries0 << w->geometry();
    }

    layout->setPolicy(policies[1]);
    qApp->processEvents();

    // Switch back to policy 0 and test whether the visible item's geometries are restored.
    layout->setPolicy(policies[0]);
    layout->setMinimumWidth(rect.width());
    layoutContainer->setGeometry(rect);
    qApp->processEvents();
    for (int i = 0; i < widgetsMax; ++i)
        QCOMPARE(widgets[i]->geometry(), geometries0[i]);
}

void Ut_DuiLayout::testLayoutGeometryOfItemsAddedToInactivePolicies_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType1");
    QTest::addColumn<QString>("policyType2");
    QTest::newRow("linearHorizontal, linearHorizontal") << "linearHorizontal" << "linearHorizontal";
    QTest::newRow("linearHorizontal, linearVertical") << "linearHorizontal" << "linearVertical";
    QTest::newRow("linearHorizontal, grid") << "linearHorizontal" << "grid";
    QTest::newRow("linearHorizontal, flow") << "linearHorizontal" << "flow";
    QTest::newRow("linearHorizontal, freestyle") << "linearHorizontal" << "freestyle";

    QTest::newRow("linearVertical, linearVertical") << "linearVertical" << "linearVertical";
    QTest::newRow("linearVertical, linearHorizontal") << "linearVertical" << "linearHorizontal";
    QTest::newRow("linearVertical, grid") << "linearVertical" << "grid";
    QTest::newRow("linearVertical, flow") << "linearVertical" << "flow";
    QTest::newRow("linearVertical, freestyle") << "linearVertical" << "freestyle";

    QTest::newRow("grid, grid") << "grid" << "grid";
    QTest::newRow("grid, linearHorizontal") << "grid" << "linearHorizontal";
    QTest::newRow("grid, linearVertical") << "grid" << "linearVertical";
    QTest::newRow("grid, flow") << "grid" << "flow";
    QTest::newRow("grid, freestyle") << "grid" << "freestyle";

    QTest::newRow("flow, flow") << "flow" << "flow";
    QTest::newRow("flow, linearHorizontal") << "flow" << "linearHorizontal";
    QTest::newRow("flow, linearVertical") << "flow" << "linearVertical";
    QTest::newRow("flow, grid") << "flow" << "grid";
    QTest::newRow("flow, freestyle") << "flow" << "freestyle";
    QTest::newRow("freestyle, freestyle") << "freestyle" << "freestyle";
    QTest::newRow("freestyle, linearHorizontal") << "freestyle" << "linearHorizontal";
    QTest::newRow("freestyle, linearVertical") << "freestyle" << "linearVertical";
    QTest::newRow("freestyle, grid") << "freestyle" << "grid";
    QTest::newRow("freestyle, flow") << "freestyle" << "flow";
}

void Ut_DuiLayout::testLayoutGeometryOfItemsAddedToInactivePolicies()
{
    QFETCH(QString, policyType1);
    QFETCH(QString, policyType2);

    QList<QString> policyTypes;
    policyTypes << policyType1;
    policyTypes << policyType2;

    qsrand(0);
    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(layout);

    QList<DuiAbstractLayoutPolicy *> policies;
    for (int i = 0; i <= 1; ++i) {
        if (policyTypes[i] == "grid")
            policies << new DuiGridLayoutPolicy(layout);
        else if (policyTypes[i] == "flow")
            policies << new DuiFlowLayoutPolicy(layout);
        else if (policyTypes[i] == "linearHorizontal")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
        else if (policyTypes[i] == "linearVertical")
            policies << new DuiLinearLayoutPolicy(layout, Qt::Vertical);
        else if (policyTypes[i] == "freestyle")
            policies << new DuiFreestyleLayoutPolicy(layout);
    }
    policies[0]->setContentsMargins(20.0, 20.0, 20.0, 20.0);
    policies[1]->setContentsMargins(20.0, 20.0, 20.0, 20.0);

    // add items to two policies, while doing this the first policy is active, the second
    // is not active.
    int widgetsMax = 1;
    QList<QGraphicsWidget *> widgets;
    layout->setPolicy(policies[0]);
    QVERIFY(policies[0]->isActive());
    QVERIFY(!policies[1]->isActive());
    for (int policyIndex = 0; policyIndex <= 1; ++policyIndex) {
        for (int i = 0; i < widgetsMax; ++i) {
            int widgetIndex = i + policyIndex * widgetsMax;
            widgets << new QGraphicsWidget();
            qreal width = ((100.0 * qrand()) / RAND_MAX) + 1;
            qreal height = ((100.0 * qrand()) / RAND_MAX) + 1;
            widgets[widgetIndex]->setMinimumSize(QSizeF(width, height));
            if (policyTypes[policyIndex] == "grid") {
                int row = i % 3;
                int col = i / 3;
                dynamic_cast<DuiGridLayoutPolicy *>(policies[policyIndex])->addItem(
                    widgets[widgetIndex], row, col);
                qApp->processEvents();
            } else if (policyTypes[policyIndex] == "linearHorizontal"
                       || policyTypes[policyIndex] == "linearVertical") {
                dynamic_cast<DuiLinearLayoutPolicy *>(policies[policyIndex])->addItem(widgets[widgetIndex]);
                static_cast<QGraphicsLayout *>(layout)->setGeometry(layout->geometry());
            } else if (policyTypes[policyIndex] == "flow") {
                dynamic_cast<DuiFlowLayoutPolicy *>(policies[policyIndex])->addItem(widgets[widgetIndex]);
                static_cast<QGraphicsLayout *>(layout)->setGeometry(layout->geometry());
            } else if (policyTypes[policyIndex] == "freestyle") {
                // I use a geometry with a fixed width and height here on purpose.
                // The item has a randomized minimum width and height (see above),
                // i.e. the item might resize if I add it to the layout using a
                // fixed geometry.
                //
                // I also use a fixed position in the geometry on purpose,
                // if an item is added at a geometry overlapping an
                // already existing item, the layout policy should move
                // the items so that they don't overlap anymore.
                QRectF geom(20.0, 20.0, 20, 20.0);
                dynamic_cast<DuiFreestyleLayoutPolicy *>(policies[policyIndex])->addItemAtGeometry(
                    widgets[widgetIndex], geom);
            } else
                QVERIFY(false); // should never happen
        }
    }
    QCOMPARE(policies[0]->count(), widgetsMax);
    QCOMPARE(policies[1]->count(), widgetsMax);
    QCOMPARE(layout->count(), 2 * widgetsMax);
    for (int i = 0; i < layout->count(); i++) {
        bool inFirstPolicy = i < widgetsMax;
        QVERIFY(policies[inFirstPolicy?0:1]->indexOf(layout->itemAt(i)) != -1);
        QVERIFY(policies[inFirstPolicy?1:0]->indexOf(layout->itemAt(i)) == -1);
    }

    qApp->processEvents();

    // As the first policy was active while the items were added and
    // the second policy was not and has never been made active, all
    // the items in the first policy should have positive coordinates
    // now and all the items in the second policy should have negative
    // coordinates. Having negative coordinates makes sure the items
    // are not visible, items in an inactive policy should be
    // invisible.
    for (int policyIndex = 0; policyIndex <= 1; ++policyIndex) {
        for (int i = 0; i < widgetsMax; ++i) {
            int widgetIndex = i + policyIndex * widgetsMax;
            qreal x1, y1, x2, y2;
            widgets[widgetIndex]->geometry().getCoords(&x1, &y1, &x2, &y2);
            //qDebug() << "policyIndex,i,geo:"<<policyIndex<<i<<widgets[widgetIndex]->geometry();
            if (policyIndex == 0) {
                QVERIFY(x1 >= 0);
                QVERIFY(y1 >= 0);
                QVERIFY(x2 >= 0);
                QVERIFY(y2 >= 0);
                QCOMPARE(widgets[widgetIndex]->isVisible(), true);
            } else {
                QCOMPARE(widgets[widgetIndex]->isVisible(), false);
            }
        }
    }
}

void Ut_DuiLayout::testLayoutItemOverlap_data()
{
    // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...
    QTest::addColumn<QString>("policyType");
    QTest::newRow("DuiLinearLayoutPolicy, Horizontal") << "linearHorizontal";
    QTest::newRow("DuiLinearLayoutPolicy, Vertical") << "linearVertical";
    QTest::newRow("DuiGridLayoutPolicy") << "grid";
    QTest::newRow("DuiFlowLayoutPolicy") << "flow";
    QTest::newRow("DuiFreestyleLayoutPolicy") << "freestyle";
}

void Ut_DuiLayout::testLayoutItemOverlap()
{
    QFETCH(QString, policyType); // Whether the policy is a DuiLinearLayoutPolicy, DuiGridLayoutPolicy, ...

    qsrand(0);
    QGraphicsWidget *layoutContainer = new QGraphicsWidget();
    QRectF rect(100.0, 100.0, 1.0, 1.0);
    // size will grow as needed when adding items
    layoutContainer->setGeometry(rect);
    QCOMPARE(rect, layoutContainer->geometry());
    m_scene->addItem(layoutContainer);
    QGraphicsLinearLayout *outerLayout = new QGraphicsLinearLayout();
    outerLayout->setContentsMargins(10.0, 10.0, 10.0, 10.0);
    DuiLayoutTest *layout = new DuiLayoutTest();
    layout->setAnimation(NULL);
    layoutContainer->setLayout(outerLayout);
    outerLayout->addItem(layout);
    DuiAbstractLayoutPolicy *policy;
    if (policyType == "grid")
        policy = new DuiGridLayoutPolicy(layout);
    else if (policyType == "flow")
        policy = new DuiFlowLayoutPolicy(layout);
    else if (policyType == "linearHorizontal")
        policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    else if (policyType == "linearVertical")
        policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    else if (policyType == "freestyle")
        policy = new DuiFreestyleLayoutPolicy(layout);
    else {
        QVERIFY(false); // should never happen
        return;
    }

    policy->setContentsMargins(20.0, 20.0, 20.0, 20.0);
    policy->setSpacing(0.01); //we need a non-zero size spacing to avoid rounding errors when testing for intersection
    QList<QGraphicsWidget *> widgets;
    int widgetsMax = 10;
    for (int i = 0; i < widgetsMax; ++i) {
        widgets << new QGraphicsWidget();
        qreal width = ((100.0 * qrand()) / RAND_MAX) + 1;
        qreal height = ((100.0 * qrand()) / RAND_MAX) + 1;

        widgets[i]->setMinimumSize(QSizeF(width, height));
        if (policyType == "grid") {
            int row = i % 3;
            int col = i / 3;
            dynamic_cast<DuiGridLayoutPolicy *>(policy)->addItem(
                widgets[i], row, col);
        } else if (policyType == "linearHorizontal" || policyType == "linearVertical")
            dynamic_cast<DuiLinearLayoutPolicy *>(policy)->addItem(widgets[i]);
        else if (policyType == "flow")
            dynamic_cast<DuiFlowLayoutPolicy *>(policy)->addItem(widgets[i]);
        else if (policyType == "freestyle") {
            // I use a geometry with a fixed width and height here on purpose.
            // The item has a randomized minimum width and height (see above),
            // i.e. the item might resize if I add it to the layout using a
            // fixed geometry.
            //
            // I also use a fixed position in the geometry on purpose,
            // if an item is added at a geometry overlapping an
            // already existing item, the freestyle layout policy should move
            // the items so that they don't overlap anymore, which is what
            // we want to test here.
            QRectF geom(20.0, 20.0, 20.0, 20.0);
            dynamic_cast<DuiFreestyleLayoutPolicy *>(policy)->addItemAtGeometry(
                widgets[i], geom);
        } else
            QVERIFY(false); // should never happen
    }
    qApp->processEvents();
    foreach(QGraphicsWidget * w1, widgets) {
        foreach(QGraphicsWidget * w2, widgets) {
            if (w1 == w2)
                continue;
//            qDebug() << "Ut_DuiLayout::testLayoutItemOverlap: policyType, outerLayout, layout, layoutContentsRect, widget1 geometry, widget2 geometry" << policyType << outerLayout->geometry() << layout->geometry() << w1->geometry() << w2->geometry();
//            qDebug() << layout->d_ptr->states.at(layout->indexOf(w1)).targetGeometry();
//            qDebug() << layout->d_ptr->states.at(layout->indexOf(w2)).targetGeometry();

            QCOMPARE(layout->d_ptr->states.at(layout->indexOf(w1)).targetGeometry(), w1->geometry());
            QCOMPARE(layout->d_ptr->states.at(layout->indexOf(w2)).targetGeometry(), w2->geometry());

            QVERIFY(! w1->geometry().intersects(w2->geometry()));
        }
    }
    delete(layoutContainer);
}

void Ut_DuiLayout::testChildItems_data()
{
    QTest::addColumn<bool>("addLayoutFirst");
    QTest::addColumn<bool>("useQtClass");
    QTest::addColumn<bool>("innerLayoutInWidget");
    for (int i = 0; i < 1 << 3; i++) {
        bool addLayoutFirst = i & 1;
        bool useQtClass = i & (1 << 1);
        bool innerLayoutInWidget = i & (1 << 2);
        QString description = (useQtClass) ? "Qt Class - " : "";
        description += "Adding layout ";
        description += (addLayoutFirst) ? "first" : "last";
        description += (innerLayoutInWidget) ? " (inside widget)" : "";
        QTest::newRow(description.toLatin1()) << addLayoutFirst << useQtClass << innerLayoutInWidget;
    }
}


void Ut_DuiLayout::testChildItems()
{
    //Check that childItems() and children()  are correct
    //The Qt class is compared as well, to check that we do the same Qt
    QFETCH(bool, addLayoutFirst);
    QFETCH(bool, useQtClass);
    QFETCH(bool, innerLayoutInWidget);

    QGraphicsWidget *form = new QGraphicsWidget;
    QGraphicsLayout *layout = NULL;
    DuiLinearLayoutPolicy *policy = NULL;

    QGraphicsWidget *widget1 = new QGraphicsWidget;
    QGraphicsWidget *widget2 = new QGraphicsWidget;

    QGraphicsLinearLayout *innerLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    QGraphicsWidget *widget3 = new QGraphicsWidget;
    innerLayout->addItem(widget3);

    QGraphicsWidget *innerLayoutForm;
    if (innerLayoutInWidget) {
        innerLayoutForm = new QGraphicsWidget;
        innerLayoutForm->setLayout(innerLayout);
    }

    if (!useQtClass) {
        layout = new DuiLayoutTest(addLayoutFirst ? form : NULL);
        policy = new DuiLinearLayoutPolicy(static_cast<DuiLayout *>(layout), Qt::Horizontal);
        policy->addItem(widget1);
        policy->addItem(widget2);
        if (innerLayoutInWidget)
            policy->addItem(innerLayoutForm);
        else
            policy->addItem(innerLayout);
    } else {
        QGraphicsLinearLayout *gridlayout = new QGraphicsLinearLayout(Qt::Horizontal, addLayoutFirst ? form : NULL);
        gridlayout->addItem(widget1);
        gridlayout->addItem(widget2);
        if (innerLayoutInWidget)
            gridlayout->addItem(innerLayoutForm);
        else
            gridlayout->addItem(innerLayout);

        layout = gridlayout;
    }

    QVERIFY(widget1->parent() == NULL);
    QVERIFY(widget2->parent() == NULL);
    QVERIFY(form->parent() == NULL);

    QVERIFY(widget1->parentItem() == (addLayoutFirst ? form : NULL));
    QVERIFY(widget2->parentItem() == (addLayoutFirst ? form : NULL));

    if (!addLayoutFirst)
        form->setLayout(layout);

    /* Test the QGraphicsItem::childItems() */
    QCOMPARE(form->childItems().count(), 3);
    QCOMPARE(form->childItems().at(0), widget1);
    QCOMPARE(form->childItems().at(1), widget2);
    if (innerLayoutInWidget) {
        QCOMPARE(form->childItems().at(2), innerLayoutForm);
        QCOMPARE(innerLayoutForm->childItems().count(), 1);
        QCOMPARE(innerLayoutForm->childItems().at(0), widget3);
    } else {
        QCOMPARE(form->childItems().at(2), widget3);
    }
    QVERIFY(widget1->parent() == NULL);
    QVERIFY(widget2->parent() == NULL);
    QVERIFY(form->parent() == NULL);
    QVERIFY(widget1->parentItem() == form);
    QVERIFY(widget2->parentItem() == form);

    /* Test the QObject::children() */
    QCOMPARE(form->children().count(), 0);
    if (innerLayoutInWidget)
        QCOMPARE(innerLayoutForm->children().count(), 0);
    delete form;
}

void compareContentsMargins(DuiAbstractLayoutPolicy *policy, qreal _left, qreal _top, qreal _right, qreal _bottom)
{
    qreal left, top, right, bottom;
    policy->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, _left); QCOMPARE(top, _top); QCOMPARE(right, _right); QCOMPARE(bottom, _bottom);
}
void compareContentsMargins(DuiLayout *layout, qreal _left, qreal _top, qreal _right, qreal _bottom)
{
    qreal left, top, right, bottom;
    layout->getContentsMargins(&left, &top, &right, &bottom);
    QCOMPARE(left, _left); QCOMPARE(top, _top); QCOMPARE(right, _right); QCOMPARE(bottom, _bottom);
}

void Ut_DuiLayout::testUserChangingValues()
{
    //This is to test for BUG 144705 Items are not visible after they're removed from and readded to a DuiFlowLayoutPolicy
    //Which was due to animationDone() being called if the start and end geometries were the saying, even if the visibility was not,
    //thus clearing the TO_BE_SHOWN flag
    QGraphicsWidget *form = new QGraphicsWidget;
    DuiLayout *layout = new DuiLayoutTest(form);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    //Set contents margins first, then set object name (to reload style)
    policy->setContentsMargins(12, 12, 12, 12);
    policy->setHorizontalSpacing(7);

    compareContentsMargins(policy, 12, 12, 12, 12);
    compareContentsMargins(layout, 12, 12, 12, 12);
    QCOMPARE(policy->horizontalSpacing(), 7.0);
    QCOMPARE(policy->verticalSpacing(), 6.0); //From CSS

    policy->setObjectName("packed"); //This should not change the margins since we, the, user specified them manually
    compareContentsMargins(policy, 12, 12, 12, 12);
    compareContentsMargins(layout, 12, 12, 12, 12);
    QCOMPARE(policy->horizontalSpacing(), 7.0);
    QCOMPARE(policy->verticalSpacing(), 0.0); // From CSS

    policy->setObjectName("");
    compareContentsMargins(policy, 12, 12, 12, 12);
    compareContentsMargins(layout, 12, 12, 12, 12);
    QCOMPARE(policy->horizontalSpacing(), 7.0);
    QCOMPARE(policy->verticalSpacing(), 6.0); // From CSS

    policy->setVerticalSpacing(6);
    QCOMPARE(policy->verticalSpacing(), 6.0); // Now manually set
    policy->setObjectName("packed");
    QCOMPARE(policy->verticalSpacing(), 6.0);
}
void Ut_DuiLayout::testAddingAndRemovingWithSpacing()
{
    QGraphicsWidget *form = new QGraphicsWidget;
    m_scene->addItem(form);
    DuiLayout *layout = new DuiLayoutTest(form);
    layout->setContentsMargins(0, 0, 0, 0);
    DuiLinearLayoutPolicy *policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(-1, -1, -1, -1);
    policy->setSpacing(6);

    QList<QGraphicsWidget *> widgets;
    for (int i = 0; i < 4; i++) {
        QGraphicsWidget *widget = new QGraphicsWidget;
        widget->setMinimumSize(100, 100);
        widget->setPreferredSize(100, 100);
        policy->addItem(widget);
        widgets << widget;
    }
    QCOMPARE(form->minimumSize(), QSizeF(100, 400 + 6 * 3));
    QCOMPARE(form->preferredSize(), QSizeF(100, 400 + 6 * 3));
    qApp->processEvents();
    for (int i = 0; i < 4; i++)
        QCOMPARE(widgets[i]->isVisible(), true);

    //Remove the first item, and check that the layout shrinks by the size of widget and the vertical spacing
    layout->removeAt(0);
    QCOMPARE(form->minimumSize(), QSizeF(100, 300 + 6 * 2));
    QCOMPARE(form->preferredSize(), QSizeF(100, 300 + 6 * 2));
    //Now remove them all and readd them
    for (int i = 0; i < 4; i++)
        layout->removeAt(0);

    QCOMPARE(form->minimumSize(), QSizeF(0, 0));
    QCOMPARE(form->preferredSize(), QSizeF(0, 0));

    for (int i = 0; i < 4; i++)
        policy->addItem(widgets[i]);

    QCOMPARE(form->minimumSize(), QSizeF(100, 400 + 6 * 3));
    QCOMPARE(form->preferredSize(), QSizeF(100, 400 + 6 * 3));

    //Check that they are visible
    qApp->processEvents();

    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    for (int i = 0; i < 4; i++)
        QCOMPARE(widgets[i]->isVisible(), true);

    //Cleanup test
    for (int i = 0; i < 4; i++)
        delete layout->takeAt(0);
    delete form;
}

void Ut_DuiLayout::testSwitchingPoliciesHidesItems()
{
    QGraphicsWidget *form = new QGraphicsWidget;
    m_scene->addItem(form);
    DuiLayout *layout = new DuiLayoutTest(form);
    DuiFlowLayoutPolicy *policy1 = new DuiFlowLayoutPolicy(layout);
    DuiFlowLayoutPolicy *policy2 = new DuiFlowLayoutPolicy(layout);

    QList<QGraphicsWidget *> widgets;
    for (int i = 0; i < 4; i++) {
        QGraphicsWidget *widget = new QGraphicsWidget;
        widget->setMinimumSize(100, 100);
        widget->setPreferredSize(100, 100);
        if (i < 2)
            policy1->addItem(widget);
        else
            policy2->addItem(widget);
        widgets << widget;
    }

    qApp->processEvents();
    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), true);
    QCOMPARE(widgets[1]->isVisible(), true);
    QCOMPARE(widgets[2]->isVisible(), false);
    QCOMPARE(widgets[3]->isVisible(), false);

    policy2->activate();
    qApp->processEvents();
    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), false);
    QCOMPARE(widgets[1]->isVisible(), false);
    QCOMPARE(widgets[2]->isVisible(), true);
    QCOMPARE(widgets[3]->isVisible(), true);

    layout->setLandscapePolicy(policy1);
    layout->setPortraitPolicy(policy2);

    qApp->processEvents();
    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), true);
    QCOMPARE(widgets[1]->isVisible(), true);
    QCOMPARE(widgets[2]->isVisible(), false);
    QCOMPARE(widgets[3]->isVisible(), false);

    //Rotate, to switch to policy2
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);

    qApp->processEvents();
    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), false);
    QCOMPARE(widgets[1]->isVisible(), false);
    QCOMPARE(widgets[2]->isVisible(), true);
    QCOMPARE(widgets[3]->isVisible(), true);

    while (!widgets.isEmpty())
        delete widgets.takeAt(0);
    delete form;
}

void Ut_DuiLayout::testSwitchingPoliciesInsidePolicyHidesItems()
{
    //Test a layout with policies inside of another layout with policies.
    //Switching policies should show/hide the items correctly.  Note that
    //currently adding the inner layout to only one outer policy is not supported.
    QGraphicsWidget *form = new QGraphicsWidget;
    m_scene->addItem(form);
    DuiLayout *layout = new DuiLayoutTest(form);
    DuiFlowLayoutPolicy *policy1 = new DuiFlowLayoutPolicy(layout);
    DuiFlowLayoutPolicy *policy2 = new DuiFlowLayoutPolicy(layout);

    DuiLayout *innerLayout = new DuiLayoutTest;
    DuiFlowLayoutPolicy *innerPolicy1 = new DuiFlowLayoutPolicy(innerLayout);
    DuiFlowLayoutPolicy *innerPolicy2 = new DuiFlowLayoutPolicy(innerLayout);

    //Add inner layout to both policies.  Anything else is not supported :-/
    policy1->addItem(innerLayout);
    policy2->addItem(innerLayout);

    QList<QGraphicsWidget *> widgets;
    for (int i = 0; i < 4; i++) {
        QGraphicsWidget *widget = new QGraphicsWidget;
        widget->setMinimumSize(100, 100);
        widget->setPreferredSize(100, 100);
        if (i < 2)
            innerPolicy1->addItem(widget);
        else
            innerPolicy2->addItem(widget);
        widgets << widget;
    }

    qApp->processEvents();
    while (layout->animation()->isAnimating() || innerLayout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), true);
    QCOMPARE(widgets[1]->isVisible(), true);
    QCOMPARE(widgets[2]->isVisible(), false);
    QCOMPARE(widgets[3]->isVisible(), false);

    innerPolicy2->activate();
    qApp->processEvents();
    while (layout->animation()->isAnimating() || innerLayout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), false);
    QCOMPARE(widgets[1]->isVisible(), false);
    QCOMPARE(widgets[2]->isVisible(), true);
    QCOMPARE(widgets[3]->isVisible(), true);

    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle0, Dui::ImmediateOrientationChange);

    innerLayout->setLandscapePolicy(innerPolicy1);
    innerLayout->setPortraitPolicy(innerPolicy2);

    qApp->processEvents();
    while (layout->animation()->isAnimating() || innerLayout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), true);
    QCOMPARE(widgets[1]->isVisible(), true);
    QCOMPARE(widgets[2]->isVisible(), false);
    QCOMPARE(widgets[3]->isVisible(), false);

    //Rotate, to switch to innerPolicy2
    DuiApplication::activeWindow()->setOrientationAngle(Dui::Angle90, Dui::ImmediateOrientationChange);

    qApp->processEvents();
    while (layout->animation()->isAnimating() || innerLayout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), false);
    QCOMPARE(widgets[1]->isVisible(), false);
    QCOMPARE(widgets[2]->isVisible(), true);
    QCOMPARE(widgets[3]->isVisible(), true);

    //Switch the outer layout policy.  This shouldn't affect the inner one's item's visibility
    policy2->activate();
    QCOMPARE(widgets[0]->isVisible(), false);
    QCOMPARE(widgets[1]->isVisible(), false);
    QCOMPARE(widgets[2]->isVisible(), true);
    QCOMPARE(widgets[3]->isVisible(), true);

    qApp->processEvents();
    while (layout->animation()->isAnimating() || innerLayout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widgets[0]->isVisible(), false);
    QCOMPARE(widgets[1]->isVisible(), false);
    QCOMPARE(widgets[2]->isVisible(), true);
    QCOMPARE(widgets[3]->isVisible(), true);

    while (!widgets.isEmpty())
        delete widgets.takeAt(0);
    delete form;
}

void Ut_DuiLayout::testAddingRemovingAddingToPolicy()
{
    //Add an item to a policy, then remove it, then readd it.  It should be visible at the end
    QGraphicsWidget *form = new QGraphicsWidget;
    m_scene->addItem(form);
    DuiLayout *layout = new DuiLayoutTest(form);
    DuiFlowLayoutPolicy *policy = new DuiFlowLayoutPolicy(layout);
    QGraphicsWidget *widget = new QGraphicsWidget;
    policy->addItem(widget);
    qApp->processEvents();

    QCOMPARE(widget->isVisible(), true);

    policy->removeItem(widget);
    policy->addItem(widget);
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), true);
    while (layout->animation()->isAnimating())
        QTest::qWait(50);

    QCOMPARE(widget->isVisible(), true);
}

void Ut_DuiLayout::testExplicitlyHidingItems_data()
{
    QTest::addColumn<bool>("withAnimation");
    QTest::newRow("Without Animation") << false;
    QTest::newRow("With Animation") << true;
}
void Ut_DuiLayout::testExplicitlyHidingItems()
{
    /* Test calling widget->hide() on a widget and check that it remains hidden even when switching between policies.
     * Also test    widget->show() on a widget and check that it only shows if not hidden by the layout.
     * Note that this only works for a DuiWidget */

    QFETCH(bool, withAnimation);

    QGraphicsWidget *form = new QGraphicsWidget;
    m_scene->addItem(form);
    DuiLayout *layout = new DuiLayoutTest(form);
    if (!withAnimation)
        layout->setAnimation(NULL);
    DuiFlowLayoutPolicy *flowPolicy = new DuiFlowLayoutPolicy(layout);
    DuiLinearLayoutPolicy *linearPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiGridLayoutPolicy *gridPolicy = new DuiGridLayoutPolicy(layout);

    DuiWidget *widget = new DuiWidget;
    widget->hide();  //Explicitly hide it
    flowPolicy->addItem(widget);
    linearPolicy->addItem(widget);
    // we do not add it to the grid layout policy

    //It's currently in a visible policy but explicitly hidden.
    QCOMPARE(widget->isVisible(), false);
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(widget->isVisible(), false);
    //Check that switching policies to a policy containing the item does not make it show
    linearPolicy->activate();
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);
    //Check that switching policies to a policy not containing the item does not make it show
    gridPolicy->activate();
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);
    //Check that switching back to a policy containing the item does not make it show
    flowPolicy->activate();
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);

    //Now show it explicitly again, it should show in the flow and linear, but not grid
    widget->show();
    QCOMPARE(widget->isVisible(), true);
    linearPolicy->activate();
    qApp->processEvents();
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(widget->isVisible(), true);
    QCOMPARE(widget->opacity(), qreal(1.0));
    gridPolicy->activate();
    qApp->processEvents();
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(widget->isVisible(), false);
    // Now test hiding, while the object is currently hiding
    widget->hide();
    QCOMPARE(widget->isVisible(), false);
    linearPolicy->activate();
    QCOMPARE(widget->isVisible(), false);
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);
    gridPolicy->activate();
    QCOMPARE(widget->isVisible(), false);
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);
    // And now test showing again, while the object is in a policy not showing it
    widget->show();
    QCOMPARE(widget->isVisible(), false);
    linearPolicy->activate();
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), true);
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(widget->isVisible(), true);
    QCOMPARE(widget->opacity(), qreal(1.0));

    // Switch to a policy without the item and remove it.  It should be still visible
    layout->removeItem(widget);
    QCOMPARE(widget->isVisible(), true);
    QCOMPARE(widget->opacity(), qreal(1.0));

    // Readd - linear is currently activated
    flowPolicy->addItem(widget);
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), false);
    linearPolicy->addItem(widget);
    qApp->processEvents();
    QCOMPARE(widget->isVisible(), true);
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(widget->isVisible(), true);
    QCOMPARE(widget->opacity(), qreal(1.0));

    //Remove from current policy (linear), forcing it to hide
    linearPolicy->removeItem(widget);
    qApp->processEvents();
    while (withAnimation && layout->animation()->isAnimating())
        QTest::qWait(50);
    QCOMPARE(widget->isVisible(), false);
    QCOMPARE(widget->opacity(), qreal(1.0));

    delete form;
}

QTEST_APPLESS_MAIN(Ut_DuiLayout)
