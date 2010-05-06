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

#include "mobjectmenubasicview.h"
#include "mbutton.h"
#include "mviewcreator.h"
#include "mobjectmenu.h"

#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mgridlayoutpolicy.h"
#include <QGraphicsLinearLayout>

#include "mwidgetaction.h"
#include "mscenemanager.h"
#include "mscalableimage.h"
#include "mapplication.h"
#include "mapplicationwindow.h"


MObjectMenuBasicView::MObjectMenuBasicView(MObjectMenu *controller) :
    MSceneWindowView(controller),
    layout(0),
    portraitPolicy(0),
    landscapePolicy(0),
    controller(controller)
{
    // create layout policies for portrait & landscape orientation
    layout = new MLayout(controller);
    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    landscapePolicy = new MGridLayoutPolicy(layout);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
}

MObjectMenuBasicView::~MObjectMenuBasicView()
{
#ifdef HAVE_CONTENTACTION
    QHash<MAction*, ContentAction::Action>::iterator i = contentActions.begin(),
                                                       e = contentActions.end();
    for(; i!=e; ++i) {
        actionRemoved(i.key());
        delete i.key();
    }
    contentActions.clear();
#endif

    MActionList actions = model()->actions();
    const int count = actions.count();
    for (int i = 0; i < count; ++i) {
        actionRemoved(actions.at(i));
    }
}

void MObjectMenuBasicView::actionAdded(MAction *action)
{
    // show only if it is visible
    if (action->isVisible()) {

        // create button for this action
        MButton *button = new MButton(action->iconID(), action->text(), controller);

        QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
        controller->connect(button, SIGNAL(clicked(bool)), SLOT(dismiss()));

        button->setEnabled(action->isEnabled());
        portraitPolicy->addItem(button);
        landscapePolicy->addItem(button, (buttons.count()) / 2, (buttons.count()) % 2);

        buttons.insert(action, button);
    }
}

void MObjectMenuBasicView::actionRemoved(MAction *action)
{
    MButton *button = buttons.value(action, NULL);
    if (button) {
        buttons.remove(action);
        delete button;
    }
}

void MObjectMenuBasicView::actionModified(MAction *action)
{
    MButton *button = buttons.value(action, NULL);
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

void MObjectMenuBasicView::updateData(const QList<const char *> &modifications)
{
    foreach(const char * member, modifications) {
        // TODO: this could be done without first removing all actions.
        if (member == MObjectMenuModel::Actions) {
            foreach(MAction * action, buttons.keys()) {
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
            QHash<MAction*, ContentAction::Action>::iterator i = contentActions.begin(),
                                                               e = contentActions.end();
            for(; i!=e; ++i) {
                actionRemoved(i.key());
                delete i.key();
            }
            contentActions.clear();

            QList<ContentAction::Action> contentActionList = ContentAction::Action::actions(model()->contentURI());
            foreach(ContentAction::Action contentAction, contentActionList) {
                // TODO: fetch the correct text from contentAction and maybe also an icon
                MAction* action = new MAction(contentAction.name(), this);
                connect(action, SIGNAL(triggered()), SLOT(contentActionTriggered()));
                contentActions.insert(action, contentAction);

                actionAdded(action);
            }
#endif
        }
    }
}

void MObjectMenuBasicView::setupModel()
{
    MSceneWindowView::setupModel();

#ifdef HAVE_CONTENTACTION
    // remove & release the old content uri dependant actions
    QHash<MAction*, ContentAction::Action>::iterator i = contentActions.begin(),
                                                       e = contentActions.end();
    for(; i!=e; ++i) {
        actionRemoved(i.key());
        delete i.key();
    }
    contentActions.clear();
#endif

    // remove & release the old manually added actions
    foreach(MAction * action, buttons.keys()) {
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
        contentActions.insert(action, contentAction);

        actionAdded(action);
    }
#endif
}

void MObjectMenuBasicView::contentActionTriggered()
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

M_REGISTER_VIEW(MObjectMenuBasicView, MObjectMenu)
