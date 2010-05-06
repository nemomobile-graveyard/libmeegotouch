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

#include "mlistindexview.h"
#include "mlistindexview_p.h"
#include "mlistview_p.h"

#include <MApplicationPage>
#include <MLabel>
#include <MPannableViewport>
#include <MScalableImage>
#include <MSceneManager>

#include <QGraphicsSceneMouseEvent>
#include <qmath.h>

#include "mlistindex.h"
#include "mlist.h"

MListIndexViewPrivate::MListIndexViewPrivate()
{
    shortcutHeight = 0;
    eventScene = NULL;
    container = NULL;
}

MListIndexViewPrivate::~MListIndexViewPrivate()
{
    clearVisible();
}

void MListIndexViewPrivate::initLayout()
{
    clearVisible();
    
    if (controller->model()) {
        controllerModel = controller->model();
        shortcutsCount = 0;

        createContainer();
        installSceneEventHandler();

        if (controller->model()->shortcutLabels().count() > 0) {
            //create the first shortcut label and get the font metrics from it
            MLabel *firstShortcut = createLabel(0);
            shortcuts.append(firstShortcut);

            QFontMetrics fm(firstShortcut->font());
            shortcutHeight = fm.height();

            shortcutsCount = controllerModel->shortcutLabels().count();
            updateVisible();
        }

        disconnect(this, SLOT(listParentChanged()));
        if (controllerModel && controllerModel->list()) {
            connect(controllerModel->list(), SIGNAL(parentChanged()), this, SLOT(listParentChanged()));
        }

    }
}

void MListIndexViewPrivate::updateLayout()
{
    if(container) {
        controller->resize(controller->preferredWidth(), containerRect.height());
        controller->setPos((containerRect.x() + containerRect.width()) - controller->preferredWidth(), containerRect.y());
    }
    updateShortcutPositions();
}

void MListIndexViewPrivate::updateShortcutPositions() const
{
    if (shortcuts.count() > 0) {
        int shortcutsGap = controller->size().height() / shortcuts.count();
        QFontMetrics fm(shortcuts[0]->font());
        for (int i = 0; i < shortcuts.count(); i++) {
            shortcuts[i]->setPos((controller->size().width() - fm.width(shortcuts[i]->text())) / 2, i*shortcutsGap);
        }
    }
}

QModelIndex MListIndexViewPrivate::locateShortcutIndex(int y, int x)
{
    Q_UNUSED(x);

    foreach (MLabel *shortcut, shortcuts) {
        if (shortcut->pos().y() <= y && shortcut->pos().y() + shortcut->contentsRect().height() >= y ) {
            int flatIndex = controller->model()->shortcutLabels().indexOf(shortcut->text());
            return controller->model()->shortcutIndexes()[flatIndex];
        }
    }
    return QModelIndex();
}

MLabel *MListIndexViewPrivate::createLabel(int index)
{
    Q_Q(MListIndexView);
    Q_ASSERT(index < controllerModel->shortcutLabels().count());

    MLabel *shortcut = new MLabel(controller);
    shortcut->setObjectName(q->style()->shortcutObjectName());
    shortcut->setText(controllerModel->shortcutLabels()[index]);

    return shortcut;
}

void MListIndexViewPrivate::clearVisible()
{
    foreach (MLabel *shortcut, shortcuts) {
        delete shortcut;
    }
    shortcuts.clear();
}

void MListIndexViewPrivate::updateVisible()
{
    if (shortcutHeight == 0)
        return;

    int fitCount = (controller->contentsRect().height()) / shortcutHeight;

    if (fitCount > 0 && fitCount != shortcuts.count()) {
        int skipCount = qCeil((qreal)shortcutsCount / (qreal)fitCount);
        if (skipCount > 0 && shortcuts.count() != shortcutsCount / skipCount) {
            clearVisible();
            for (int i = 0; i < shortcutsCount; i += skipCount) {
                if (shortcutsCount - i <= skipCount)
                    shortcuts.append(createLabel(shortcutsCount - 1));
                else
                    shortcuts.append(createLabel(i));
            }
        }
    }
    updateShortcutPositions();
}

void MListIndexViewPrivate::createContainer()
{
    disconnect(this, SLOT(exposedContentRectChanged()));
    if (controller->model()->list()) {
        container = MListViewPrivateNamespace::findParentWidgetOfType<MApplicationPage>(controller->model()->list());

        if (container) {
            controller->setParentItem(container);

            connect(container, SIGNAL(exposedContentRectChanged()), this, SLOT(exposedContentRectChanged()));
            exposedContentRectChanged();
        }

    }

}

void MListIndexViewPrivate::installSceneEventHandler()
{
    uninstallSceneEventHandler();

    if (controller->scene()) {
        eventScene = controller->scene();
        eventScene->installEventFilter(this);
    }
}

void MListIndexViewPrivate::uninstallSceneEventHandler()
{
    if (eventScene)
        eventScene->removeEventFilter(this);
}

bool MListIndexViewPrivate::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);

    QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if (mouseEvent && controller->isVisible()) {
        QPointF mousePos = controller->mapFromScene(mouseEvent->scenePos());

        bool mouseOver = false;
        if (mousePos.y() >= 0 && mousePos.y() <= controller->size().height() &&
           mousePos.x() >= 0 && mousePos.x() <= controller->size().width())
            mouseOver = true;

        switch (mouseEvent->type()) {
        case QGraphicsSceneMouseEvent::GraphicsSceneMousePress:
        case QGraphicsSceneMouseEvent::GraphicsSceneMouseMove:{
                if (mouseOver) {
                    QModelIndex scrollTo = locateShortcutIndex(mousePos.y(), 0);
                    if (scrollTo.isValid()) {
                        controllerModel->list()->scrollTo(scrollTo, MList::PositionAtTopHint);
                    }
                    return true;
                }
            }
            break;
        default:
            break;
        }
    }

    return false;
}

void MListIndexViewPrivate::listParentChanged()
{
    initLayout();
}

void MListIndexViewPrivate::exposedContentRectChanged()
{
    containerRect = container->exposedContentRect();
    updateLayout();
}

MListIndexView::MListIndexView(MListIndex *controller) : MWidgetView(controller), d_ptr(new MListIndexViewPrivate)
{
    Q_D(MListIndexView);

    d->q_ptr = this;
    d->controller = controller;
}

MListIndexView::~MListIndexView()
{
    delete d_ptr;
}

void MListIndexView::setGeometry(const QRectF &rect)
{
    Q_D(MListIndexView);

    d->updateVisible();
    MWidgetView::setGeometry(rect);
}

void MListIndexView::setupModel()
{
    Q_D(MListIndexView);

    MWidgetView::setupModel();

    d->initLayout();
}

void MListIndexView::applyStyle()
{
    Q_D(MListIndexView);

    MWidgetView::applyStyle();

    if (style()->preferredSize().isValid())
        d->controller->setPreferredWidth(style()->preferredSize().width());
    else
        d->controller->setPreferredWidth(0.0);

    foreach (MLabel *shortcut, d->shortcuts) {
        shortcut->setObjectName(style()->shortcutObjectName());
    }
}

void MListIndexView::updateData(const QList<const char *> &modifications)
{
    Q_D(MListIndexView);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];

        if (member == MListIndexModel::ShortcutLabels || member == MListIndexModel::List) {
            if (model()->list())
                d->initLayout();
        }
    }

    MWidgetView::updateData(modifications);
}

M_REGISTER_VIEW_NEW(MListIndexView, MListIndex)
