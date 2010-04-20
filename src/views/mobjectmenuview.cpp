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

#include <QGraphicsSceneMouseEvent>

#include "mobjectmenuview_p.h"
#include "mbutton.h"
#include "mviewcreator.h"
#include "mobjectmenu.h"

#include "mpannableviewport.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mgridlayoutpolicy.h"
#include <QGraphicsLinearLayout>

#include "mwidgetaction.h"
#include "mscenemanager.h"
#include "mscalableimage.h"
#include "mapplication.h"
#include "mapplicationwindow.h"

MObjectMenuViewPrivate::MObjectMenuViewPrivate()
    : viewport(0),
      container(0),
      layout(0),
      portraitPolicy(0),
      landscapePolicy(0),
      controller(0)
{
}

MObjectMenuViewPrivate::~MObjectMenuViewPrivate()
{
}

void MObjectMenuViewPrivate::init()
{
    // create pannable viewport
    viewport = new MPannableViewport();

    // create container widget for pannable viewport
    container = new QGraphicsWidget;
    viewport->setWidget(container);

    // create layout for controller, make it pannable
    QGraphicsLinearLayout *controllerLayout = new QGraphicsLinearLayout();
    controllerLayout->setContentsMargins(0, 0, 0, 0);
    controllerLayout->addItem(viewport);
    controller->setLayout(controllerLayout);

    // create layout policies for portrait & landscape orientation
    layout = new MLayout(container);
    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    landscapePolicy = new MGridLayoutPolicy(layout);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
}


MObjectMenuView::MObjectMenuView(MObjectMenu *controller) :
    MSceneWindowView(controller),
    d_ptr(new MObjectMenuViewPrivate)
{
    Q_D(MObjectMenuView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MObjectMenuView::MObjectMenuView(MObjectMenuViewPrivate &dd, MObjectMenu *controller) :
    MSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(MObjectMenuView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MObjectMenuView::~MObjectMenuView()
{
#ifdef HAVE_CONTENTACTION
    Q_D(MObjectMenuView);
    QHash<MAction*, ContentAction::Action>::iterator i = d->contentActions.begin(),
                                                       e = d->contentActions.end();
    for(; i!=e; ++i) {
        actionRemoved(i.key());
        delete i.key();
    }
    d->contentActions.clear();
#endif

    MActionList actions = model()->actions();
    const int count = actions.count();
    for (int i = 0; i < count; ++i) {
        actionRemoved(actions.at(i));
    }

    delete d_ptr;
}

void MObjectMenuView::actionAdded(MAction *action)
{
    Q_D(MObjectMenuView);

    // show only if it is visible
    if (action->isVisible()) {

        // create button for this action
        MButton *button = new MButton(action->iconID(), action->text(), d->container);

        QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
        d->controller->connect(button, SIGNAL(clicked(bool)), SLOT(dismiss()));

        button->setEnabled(action->isEnabled());
        d->portraitPolicy->addItem(button);
        d->landscapePolicy->addItem(button, (d->buttons.count()) / 2, (d->buttons.count()) % 2);

        d->buttons.insert(action, button);
    }
}

void MObjectMenuView::actionRemoved(MAction *action)
{
    Q_D(MObjectMenuView);

    MButton *button = d->buttons.value(action, NULL);
    if (button) {
        d->buttons.remove(action);
        delete button;
    }
}

void MObjectMenuView::actionModified(MAction *action)
{
    Q_D(MObjectMenuView);
    MButton *button = d->buttons.value(action, NULL);
    if (button) {
        if (!action->isVisible()) {
            actionRemoved(action);
        } else {
            // update button data accordingly
            button->setText(action->text());
            button->setIconID(action->iconID());
            button->setEnabled(action->isEnabled());
        }
    } else {
        // there is no button yet, action must've been invisible
        actionAdded(action);
    }
}

void MObjectMenuView::updateData(const QList<const char *> &modifications)
{
    Q_D(MObjectMenuView);

    foreach(const char * member, modifications) {
        // TODO: this could be done without first removing all actions.
        if (member == MObjectMenuModel::Actions) {
            foreach(MAction * action, d->buttons.keys()) {
                actionRemoved(action);
            }

            MActionList actions = model()->actions();
            const int count = actions.count();
            for (int i = 0; i < count; ++i) {
                actionAdded(actions.at(i));
            }
        } else if (member == MObjectMenuModel::ContentURI) {
#ifdef HAVE_CONTENTACTION
            // remove & release the old content uri dependant actions
            QHash<MAction*, ContentAction::Action>::iterator i = d->contentActions.begin(),
                                                               e = d->contentActions.end();
            for(; i!=e; ++i) {
                actionRemoved(i.key());
                delete i.key();
            }
            d->contentActions.clear();

            QList<ContentAction::Action> contentActionList = ContentAction::Action::actions(model()->contentURI());
            foreach(ContentAction::Action contentAction, contentActionList) {
                // TODO: fetch the correct text from contentAction and maybe also an icon
                MAction* action = new MAction(contentAction.name(), this);
                connect(action, SIGNAL(triggered()), SLOT(contentActionTriggered()));
                d->contentActions.insert(action, contentAction);

                actionAdded(action);
            }
#endif
        }
    }
}

void MObjectMenuView::setupModel()
{
    MSceneWindowView::setupModel();

    Q_D(MObjectMenuView);

#ifdef HAVE_CONTENTACTION
    // remove & release the old content uri dependant actions
    QHash<MAction*, ContentAction::Action>::iterator i = d->contentActions.begin(),
                                                       e = d->contentActions.end();
    for(; i!=e; ++i) {
        actionRemoved(i.key());
        delete i.key();
    }
    d->contentActions.clear();
#endif

    // remove & release the old manually added actions
    foreach(MAction * action, d->buttons.keys()) {
        actionRemoved(action);
    }

    connect(model(), SIGNAL(actionAdded(MAction *)),
            this, SLOT(actionAdded(MAction *)));
    connect(model(), SIGNAL(actionRemoved(MAction *)),
            this, SLOT(actionRemoved(MAction *)));
    connect(model(), SIGNAL(actionModified(MAction *)),
            this, SLOT(actionModified(MAction *)));

    MActionList actions = model()->actions();
    const int count = actions.count();
    for (int i = 0; i < count; ++i) {
        actionAdded(actions.at(i));
    }

#ifdef HAVE_CONTENTACTION
    QList<ContentAction::Action> contentActionList = ContentAction::Action::actions(model()->contentURI());
    foreach(ContentAction::Action contentAction, contentActionList) {
        // TODO: fetch the correct text from contentAction and maybe also an icon
        MAction* action = new MAction(contentAction.name(), this);
        connect(action, SIGNAL(triggered()), SLOT(contentActionTriggered()));
        d->contentActions.insert(action, contentAction);

        actionAdded(action);
    }
#endif
}

void MObjectMenuView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //Do not pass the events through the menu window
    event->accept();
}

void MObjectMenuView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MObjectMenuView);
    d->controller->dismiss();
}

QSizeF MObjectMenuView::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    Q_D(const MObjectMenuView);
    QSize sceneSize;
    QSizeF hint;
    if(d->controller->sceneManager()) {
        hint = sceneSize = d->controller->sceneManager()->visibleSceneSize();

        if(d->controller->sceneManager()->orientation() == M::Landscape)
            hint.rheight() = d->landscapePolicy->sizeHint(which, constraint).height();
        else
            hint.rheight() = d->portraitPolicy->sizeHint(which, constraint).height();
    } else {
        if (MApplication::activeWindow())
            sceneSize = MApplication::activeWindow()->visibleSceneSize();
        else if (!MApplication::windows().isEmpty())
            sceneSize = MApplication::windows().at(0)->visibleSceneSize();
        hint = sceneSize;

        hint.rheight() = d->layout->effectiveSizeHint(which, constraint).height();
    }

    if(hint.height() > sceneSize.height()) {
        hint.rheight() = sceneSize.height();
    //    d->viewport->setEnabled(true);
    } else {
    //    d->viewport->setEnabled(false);
    }
    return hint;
}

void MObjectMenuViewPrivate::contentActionTriggered()
{
#ifdef HAVE_CONTENTACTION
    Q_Q(MObjectMenuView);

    MAction* action = qobject_cast<MAction*>(q->sender());
    if(!action)
        return;

    QHash<MAction*, ContentAction::Action>::iterator i = contentActions.find(action);
    if(i == contentActions.end())
        return;

    i.value().trigger();
#endif
}

M_REGISTER_VIEW_NEW(MObjectMenuView, MObjectMenu)
#include "moc_mobjectmenuview.cpp"
