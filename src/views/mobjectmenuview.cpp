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
#include "mimagewidget.h"
#include "mlabel.h"
#include "mseparator.h"
#include "mviewcreator.h"
#include "mobjectmenu.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

#include "mwidgetaction.h"
#include "mscenemanager.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mpannableviewport.h"

MObjectMenuViewPrivate::MObjectMenuViewPrivate() :
      q_ptr(0),
      mainLayout(0),
      actionLayout(0),
      titleLayout(0),
      controller(0),
      titleArea(0),
      titleLabel(0),
      titleIcon(0),
      titleSeparator(0),
      actionViewport(0),
      actionWidget(0)
{
}

MObjectMenuViewPrivate::~MObjectMenuViewPrivate()
{
    if (!titleArea->parentItem() && !titleArea->parentLayoutItem()) {
        delete titleArea;
    }
}

void MObjectMenuViewPrivate::init()
{
    mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0.0,0.0,0.0,0.0);
    mainLayout->setSpacing(0);
    controller->setLayout(mainLayout);

    //create title bar
    titleLayout = new QGraphicsGridLayout();
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0.0,0.0,0.0,0.0);
    titleArea = new MObjectMenuTitleArea(controller);
    titleArea->setStyleName("ObjectMenuTitleArea");
    titleArea->setObjectName(titleArea->styleName());
    titleArea->setLayout(titleLayout);

    //create and add title icon
    titleIcon = new MImageWidget(titleArea);
    titleLayout->addItem(titleIcon, 0, 0);
    titleLayout->setAlignment(titleIcon, Qt::AlignCenter);

    //create and add title label
    titleLabel = new MLabel("", titleArea);
    titleLabel->setTextElide(true);
    titleLayout->addItem(titleLabel, 0, 1);
    titleLayout->setAlignment(titleLabel, Qt::AlignCenter);

    //create and add title separator
    titleSeparator = new MSeparator(titleArea);
    titleLayout->addItem(titleSeparator, 1, 0, 1, 2);
    titleLayout->setAlignment(titleSeparator, Qt::AlignCenter);

    //setup pannable viewport for the action buttons
    actionLayout = new QGraphicsLinearLayout(Qt::Vertical);
    actionLayout->setSpacing(0);
    actionLayout->setContentsMargins(0.0,0.0,0.0,0.0);
    actionWidget = new QGraphicsWidget;
    actionWidget->setLayout(actionLayout);
    actionViewport = new MPannableViewport(controller);
    actionViewport->setWidget(actionWidget);
    actionViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    actionViewport->setObjectName("ObjectMenuActionViewport");
    mainLayout->addItem(actionViewport);

    Q_Q(MObjectMenuView);
    QObject::connect(controller, SIGNAL(appearing()), q, SLOT(appearing()));
}

void MObjectMenuViewPrivate::updateIcon()
{
    Q_Q(MObjectMenuView);

    //setup the icon, first try to load it from filesystem,
    //if it fails load it from theme, if iconId is "" hide
    //the image widget
    if( !q->model()->iconId().isEmpty() ) {
        QPixmap pm(q->model()->iconId());
        if( !pm.isNull() )
            titleIcon->setPixmap(pm);
        else
            titleIcon->setImage(q->model()->iconId());
        titleIcon->show();
        titleLayout->addItem(titleIcon, 0, 0);
        titleLayout->setAlignment(titleIcon, Qt::AlignCenter);
    } else {
        titleIcon->setImage(q->model()->iconId());
        titleIcon->hide();
        titleLayout->removeAt(0);
    }
}

void MObjectMenuViewPrivate::updateTitleAreaVisibility()
{
    Q_Q(MObjectMenuView);

    //hide whole title area if title == "" and icon is not visible
    if( q->model()->title().isEmpty() && q->model()->iconId().isEmpty() ) {
        mainLayout->removeItem(titleArea);
        actionViewport->setStyleName("objectMenuViewportWithoutTitle");
        titleArea->hide();
        if (controller->scene()) {
            //Remove from the scene as otherwise it might distract appear/disappear animation
            controller->scene()->removeItem(titleArea);
        }
    } else if( mainLayout->itemAt(0) != titleArea ) {
        mainLayout->insertItem(0, titleArea);
        actionViewport->setStyleName("objectMenuViewportWithTitle");
        titleArea->show();
    }
}

void MObjectMenuViewPrivate::updateActionViewportSize()
{
    Q_Q(MObjectMenuView);

    //viewport doesnt update its size without this
    actionViewport->setWidget(actionWidget);
    actionViewport->updateGeometry();

    //force scene window to update it's size
    emit q->geometryAttributesChanged();
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

    //show action immediately if the menu is not disappering,
    //otherwise it will be shown when the menu is shown next time
    if( d->controller->sceneWindowState() != MSceneWindow::Disappearing ) {
        // show only if it is visible
        if (action->isVisible()) {

            // create button for this action
            MButton *button = new MButton(action->iconID(), action->text(), d->actionWidget);

            d->controller->connect(button, SIGNAL(clicked(bool)), SLOT(dismiss()));
            QObject::connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));

            button->setEnabled(action->isEnabled());
            d->actionLayout->addItem(button);
            int buttonCount = d->actionLayout->count();
            if(buttonCount == 1) {
                // make the only button to use "single button" background
                button->setLayoutPosition(M::DefaultPosition);
            } else if( buttonCount > 1 ) {
                MButton* prev = (MButton*)d->actionLayout->itemAt(buttonCount - 2);
                // we have more than one in layout
                if(buttonCount == 2) {
                    prev->setLayoutPosition(M::VerticalTopPosition);
                } else {
                    prev->setLayoutPosition(M::VerticalCenterPosition);
                }
                button->setLayoutPosition(M::VerticalBottomPosition);
            }

            d->buttons.insert(action, button);

            d->updateActionViewportSize();
        }
    } else {
        d->delayedActionEvents.append(qMakePair((int)QEvent::ActionAdded, action));
    }
}
void MObjectMenuView::actionRemoved(MAction *action)
{
    Q_D(MObjectMenuView);

    //remove the  action immediately if the menu is not disappering,
    //otherwise it will be removed when the menu is shown next time
    if( d->controller->sceneWindowState() != MSceneWindow::Disappearing ) {
        MButton *button = d->buttons.value(action, NULL);
        if (button) {
            d->buttons.remove(action);
            delete button;
        }
        d->updateActionViewportSize();
    }
    else {
        d->delayedActionEvents.append(qMakePair((int)QEvent::ActionRemoved, action));
    }
}

void MObjectMenuView::actionModified(MAction *action)
{
    Q_D(MObjectMenuView);
    //modify the  action immediately if the menu is not disappering,
    //otherwise it will be modified when the menu is shown next time
    if( d->controller->sceneWindowState() != MSceneWindow::Disappearing ) {
        MButton *button = d->buttons.value(action, NULL);
        if (button) {
            if (!action->isVisible()) {
                actionRemoved(action);
            } else {
                // update button data accordingly
                button->setText(action->text());
                button->setIconID(action->iconID());
                button->setToggledIconID(action->toggledIconID());
                button->setEnabled(action->isEnabled());
            }
        } else {
            // there is no button yet, action must've been invisible
            actionAdded(action);
        }
    }
    else {
        d->delayedActionEvents.append(qMakePair((int)QEvent::ActionChanged, action));
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
        } else if (member == MObjectMenuModel::Title ) {
            d->titleLabel->setText(model()->title());
            d->updateTitleAreaVisibility();
        } else if (member == MObjectMenuModel::IconId ) {
            d->updateIcon();
            d->updateTitleAreaVisibility();
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

    const MObjectMenuStyle *s = static_cast<const MObjectMenuStyle *>(style().operator ->());

    d->titleArea->setStyleName(s->titleStyleName());
    d->titleIcon->setStyleName(s->titleIconStyleName());
    d->titleLabel->setStyleName(s->titleLabelStyleName());
    d->titleSeparator->setStyleName(s->titleSeparatorStyleName());

    d->titleLabel->setText(model()->title());
    d->updateIcon();
    d->updateTitleAreaVisibility();
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

void MObjectMenuViewPrivate::appearing()
{
    Q_Q(MObjectMenuView);

    //add the delayed actions to the object menu
    QPair<int, MAction*> pair;
    foreach(pair, delayedActionEvents) {
        if (pair.first == QEvent::ActionAdded)
            q->actionAdded(pair.second);
        else if (pair.first == QEvent::ActionRemoved)
            q->actionRemoved(pair.second);
        else if (pair.first == QEvent::ActionChanged)
            q->actionModified(pair.second);
    }
    delayedActionEvents.clear();
}

M_REGISTER_VIEW_NEW(MObjectMenuView, MObjectMenu)
#include "moc_mobjectmenuview.cpp"

