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

#include <QGraphicsLinearLayout>

#include "mapplicationmenuview.h"
#include "mapplicationmenuview_p.h"
#include "mapplicationmenu.h"
#include "mlist.h"
#include "mlabel.h"
#include "mimagewidget.h"
#include "maction.h"
#include "mwidgetaction.h"
#include "mscalableimage.h"
#include "mpannableviewport.h"
#include "mcombobox.h"
#include "mstylablewidget.h"
#include "mpositionindicator.h"

/////////////////////////////////////////////////////////////////////////////

Q_DECLARE_METATYPE(QAction*)

MActionsItemModel::MActionsItemModel(QGraphicsWidget* actionsProvider, QObject *parent)
    : QAbstractListModel(parent), actionsProvider(actionsProvider), needsUpdate(true), frozen(false)
{
    actionsProvider->installEventFilter(this);
    update();
}

MActionsItemModel::~MActionsItemModel()
{
    actionsProvider->removeEventFilter(this);
}

QVariant MActionsItemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() < filteredActions.count()) {
        QAction* action = filteredActions[index.row()];

        if (action && role == Qt::DisplayRole)
            return QVariant(action->text());

        if (action && role == ActionRole)
            return QVariant::fromValue(action);

        MAction* maction = qobject_cast<MAction*>(action);
        if (maction && role == Qt::DecorationRole)
            return QVariant(maction->iconID());
    }
    return QVariant();
}

void MActionsItemModel::freeze()
{
    frozen = true;
}

void MActionsItemModel::unfreeze()
{
    frozen = false;
    if (needsUpdate)
        update();
}

void MActionsItemModel::update()
{
    beginResetModel();
    filteredActions.clear();
    foreach(QAction* action, actionsProvider->actions()) {
        if (!action->isVisible())
            continue;

        MAction* maction = qobject_cast<MAction*>(action);
        bool validLocation = !maction || maction->location().testFlag(MAction::ApplicationMenuLocation);
        if (validLocation)
            filteredActions.append(action);
    }
    endResetModel();
    needsUpdate = false;
}

bool MActionsItemModel::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::ActionRemoved
     || event->type() == QEvent::ActionAdded
     || event->type() == QEvent::ActionChanged)
    {
        if (!frozen)
            update();
        else
            needsUpdate = true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////

MMenuListItem::MMenuListItem(QGraphicsItem* parent)
    : MListItem(parent), icon(0), title(0)
{
    new QGraphicsLinearLayout(this);
    layout()->setContentsMargins(0, 0, 0, 0);
}

MMenuListItem::~MMenuListItem()
{
    delete icon;
    delete title;
}

void MMenuListItem::updateLayout(ItemStyle itemStyle)
{
    QGraphicsLinearLayout* linearLayout = static_cast<QGraphicsLinearLayout*>(layout());
    while (linearLayout->count()) {
        linearLayout->itemAt(0)->graphicsItem()->hide();
        linearLayout->removeAt(0);
    }
    if (itemStyle == IconTitle) {
        linearLayout->addItem(icon);
        icon->show();
        linearLayout->addItem(title);
        title->show();
    } else if (itemStyle == SingleTitle) {
        linearLayout->addItem(title);
        title->show();
    } else if (itemStyle == CustomWidget) {
        linearLayout->addItem(widget);
        if (widget) widget->show();
    }
}

void MMenuListItem::setTitle(const QString &text)
{
    if (!title) {
        title = new MLabel(this);
        title->setTextElide(true);
        title->setStyleName(titleStyleName);
    }
    title->setText(text);
}

void MMenuListItem::setIconID(const QString& id)
{
    if (!icon) {
        icon = new MImageWidget(this);
        icon->setStyleName("CommonMainIcon");
    }
    icon->setImage(id);
}

void MMenuListItem::setCustomWidget(MWidget* newWidget)
{
    if (newWidget == widget)
        return;

    if (widget && widget->parentItem() == this)
        widget->setParentItem(0);

    widget = newWidget;

    if (widget) {
        MMenuListItem* previousParent = dynamic_cast<MMenuListItem*>(widget->parentItem());
        if (previousParent)
            previousParent->setCustomWidget(0);

        MWidgetController *widgetCtrl = qobject_cast<MWidgetController*>(widget);
        if (widgetCtrl && widgetCtrl->styleName().isEmpty())
            widgetCtrl->setStyleName(styleName());
    }
}

void MMenuListItem::setTitleStyleName(const QString &styleName)
{
    titleStyleName = styleName;
    if (title)
        title->setStyleName(titleStyleName);
}

/////////////////////////////////////////////////////////////////////////////

MApplicationMenuCellCreator::MApplicationMenuCellCreator(MList* list)
{
    MMenuListItem* item = new MMenuListItem(list);
    size = item->effectiveSizeHint(Qt::PreferredSize);
    delete item;
}

MWidget* MApplicationMenuCellCreator::createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
{
    MWidget *cell = MListCellCreatorHelper<MMenuListItem>::createCell(recycler, QString(), QString());
    MMenuListItem* item = static_cast<MMenuListItem*>(cell);
    if (item) {
        item->setStyleName(itemStyleName);
        item->setTitleStyleName(itemTitleStyleName);
    }
    updateCell(index, cell);
    return cell;
}

void MApplicationMenuCellCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
   MMenuListItem* item = static_cast<MMenuListItem*>(cell);
   QString text = index.data(Qt::DisplayRole).toString();
   QString iconID = index.data(Qt::DecorationRole).toString();
   QAction* action = index.data(MActionsItemModel::ActionRole).value<QAction*>();
   MWidget* widget = extractWidget(action);
   MWidgetController* widgetCtrl = qobject_cast<MWidgetController*>(widget);

   if (isComboBox(item->customWidget()))
       item->customWidget()->disconnect(SIGNAL(clicked()), item, SLOT(click()));

   if (isComboBox(widget)) {
        if (widget->parentObject())
            widget->disconnect(SIGNAL(clicked()), widget->parentObject(), SLOT(click()));
        item->connect(widget, SIGNAL(clicked()), SLOT(click()));
   }

   if (widget) {
       item->setCustomWidget(widget);
       item->updateLayout(MMenuListItem::CustomWidget);
   } else if (!iconID.isEmpty()) {
       item->setIconID(iconID);
       item->setTitle(text);
       item->updateLayout(MMenuListItem::IconTitle);
   } else {
       item->setTitle(text);
       item->updateLayout(MMenuListItem::SingleTitle);
   }

   if (action)
       item->setEnabled(action->isEnabled());

   if (action && !action->objectName().isEmpty())
       item->setObjectName(QString("menuactioncommand_%1").arg(action->objectName()));
   else
       item->setObjectName("menuactioncommand");

   if (index.model()->rowCount() == 1)
       item->setLayoutPosition(M::DefaultPosition);
   else if (index.row() == 0)
       item->setLayoutPosition(M::VerticalTopPosition);
   else if (index.row() == index.model()->rowCount() - 1)
       item->setLayoutPosition(M::VerticalBottomPosition);
   else
       item->setLayoutPosition(M::VerticalCenterPosition);

   if (widgetCtrl)
       widgetCtrl->setLayoutPosition(item->layoutPosition());
}

bool MApplicationMenuCellCreator::isComboBox(MWidget* widget)
{
    return qobject_cast<MComboBox*>(widget) != 0;
}

MWidget* MApplicationMenuCellCreator::extractWidget(QAction* action) const
{
    MWidgetAction* widgetAction = qobject_cast<MWidgetAction*>(action);
    if (!widgetAction)
        return 0;

    return widgetAction->widget();
}

void MApplicationMenuCellCreator::setItemStyleName(const QString &styleName)
{
    itemStyleName = styleName;
}

void MApplicationMenuCellCreator::setItemTitleStyleName(const QString &styleName)
{
    itemTitleStyleName = styleName;
}

/////////////////////////////////////////////////////////////////////////////
// MApplicationMenuLayout

MApplicationMenuLayout::MApplicationMenuLayout(QGraphicsLayoutItem *parent)
    : QObject(0), QGraphicsLinearLayout(Qt::Vertical, parent)
{
}

void MApplicationMenuLayout::setGeometry(const QRectF &rect)
{
    QGraphicsLinearLayout::setGeometry(rect);
    emit geometrySet();
}

/////////////////////////////////////////////////////////////////////////////

MApplicationMenuViewPrivate::MApplicationMenuViewPrivate(MApplicationMenu *menu)
    : q_ptr(0),
      controller(menu),
      actionCommandViewport(0),
      topArea(0),
      list(0),
      cellCreator(0)
{}

MApplicationMenuViewPrivate::~MApplicationMenuViewPrivate()
{
    controller->layout()->removeAt(0); //topArea
    controller->layout()->removeAt(0); //actionCommandViewport
    delete topArea;
    delete actionCommandViewport;
}

void MApplicationMenuViewPrivate::init()
{
    bool ok;

    ok = connect(controller, SIGNAL(geometryChanged()), SLOT(onControllerGeometryChanged()));
    if (!ok) qFatal("signal connection failed");

    ok = connect(controller, SIGNAL(appeared()), SLOT(onAppeared()));
    if (!ok) qFatal("signal connection failed");

    MApplicationMenuLayout* controllerLayout = new MApplicationMenuLayout(controller);
    controllerLayout->setContentsMargins(0, 0, 0, 0);
    controllerLayout->setSpacing(0);
    ok = connect(controllerLayout, SIGNAL(geometrySet()),
                 SLOT(checkIfNewControllerGeometryNeeded()),
                 Qt::QueuedConnection);
    if (!ok) qFatal("signal connection failed");

    QGraphicsLinearLayout *actionLayout = new QGraphicsLinearLayout(Qt::Vertical);
    actionLayout->setSpacing(0);
    actionLayout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
    MStylableWidget *actionListHolder = new MStylableWidget;
    actionListHolder->setStyleName("ApplicationMenuActionListHolder");
    actionListHolder->setLayout(actionLayout);

    actionCommandViewport = new MPannableViewport(controller);
    actionCommandViewport->setAcceptGesturesFromAnyDirection(true);
    actionCommandViewport->setStyleName("MApplicationMenuActionViewport");
    actionCommandViewport->setObjectName(actionCommandViewport->styleName());
    actionCommandViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    actionCommandViewport->setWidget(actionListHolder);

    list = new MList(actionListHolder);
    MActionsItemModel* actionItemModel = new MActionsItemModel(controller, this);
    list->setItemModel(actionItemModel);
    cellCreator = new MApplicationMenuCellCreator(list);
    list->setCellCreator(cellCreator);
    list->setSelectionMode(MList::NoSelection);

    actionLayout->addItem(list);

    topArea = new MStylableWidget(controller);
    topArea->setStyleName("MApplicationMenuTopArea");
    topArea->setObjectName(topArea->styleName());

    controllerLayout->addItem(topArea);
    controllerLayout->setAlignment(topArea, Qt::AlignCenter);
    controllerLayout->addItem(actionCommandViewport);
    controllerLayout->setAlignment(actionCommandViewport, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), SLOT(actionTriggered(QModelIndex)));
    connect(controller, SIGNAL(displayEntered()), SLOT(resetListPosition()));

    ok = actionItemModel->connect(controller, SIGNAL(appearing()), SLOT(freeze()));
    if (!ok) qFatal("signal connection failed");

    ok = actionItemModel->connect(controller, SIGNAL(appeared()), SLOT(unfreeze()));
    if (!ok) qFatal("signal connection failed");

    actionItemModel->connect(controller, SIGNAL(disappearing()), SLOT(freeze()));
    actionItemModel->connect(controller, SIGNAL(disappeared()), SLOT(unfreeze()));
}

void MApplicationMenuViewPrivate::actionTriggered(const QModelIndex &index)
{
    controller->disappear();
    QAction* action = index.data(MActionsItemModel::ActionRole).value<QAction*>();
    if (action)
        action->trigger();
}

void MApplicationMenuViewPrivate::resetListPosition()
{
    list->scrollTo(list->itemModel()->index(0, 0), MList::PositionAtTopHint, MList::NonAnimated);
    actionCommandViewport->setPosition(QPointF(0,0));
}

void MApplicationMenuViewPrivate::checkIfNewControllerGeometryNeeded()
{
    if (!controller)
        return;

    Q_Q(MApplicationMenuView);

    if (controller->sceneWindowState() != MSceneWindow::Appeared)
        return;

    QSizeF currentPreferredSize = controller->effectiveSizeHint(Qt::PreferredSize);

    if (currentPreferredSize != preferredSizeWhenControllerGeometryLastSet
        && currentPreferredSize != preferredSizeWhenLastChecked) {

        // Our contents have grown or shrunk while we are being displayed.
        // Therefore we would like to have a new geometry that better reflects our
        // currently preferred size. Emitting the signal below will cause
        // MSceneManager to update the geometry of our controller.
        emit q->geometryAttributesChanged();
    }

    preferredSizeWhenLastChecked = currentPreferredSize;
}

void MApplicationMenuViewPrivate::onAppeared()
{
    preferredSizeWhenLastChecked = QSizeF();
    preferredSizeWhenControllerGeometryLastSet = QSizeF();
}

void MApplicationMenuViewPrivate::onControllerGeometryChanged()
{
    if (!controller)
        return;

    if (controller->sceneWindowState() == MSceneWindow::Appeared) {
        preferredSizeWhenControllerGeometryLastSet =
                controller->effectiveSizeHint(Qt::PreferredSize);
    }
}

void MApplicationMenuViewPrivate::updateStyleNames()
{
    Q_Q(MApplicationMenuView);

    const MApplicationMenuStyle *s = static_cast<const MApplicationMenuStyle *>(q->style().operator ->());

    actionCommandViewport->positionIndicator()->setStyleName(s->positionIndicatorStyleName());
    cellCreator->setItemStyleName(s->itemStyleName());
    cellCreator->setItemTitleStyleName(s->itemTitleStyleName());
}

/////////////////////////////////////////////////////////////////////////////

MApplicationMenuView::MApplicationMenuView(MApplicationMenu *controller) :
    MSceneWindowView(controller),
    d_ptr(new MApplicationMenuViewPrivate(controller))
{
    Q_D(MApplicationMenuView);
    d->q_ptr = this;
    d->init();
}

MApplicationMenuView::MApplicationMenuView(MApplicationMenuViewPrivate &dd, MApplicationMenu *controller) :
    MSceneWindowView(controller),
    d_ptr(&dd)
{
    Q_D(MApplicationMenuView);
    d->q_ptr = this;
    d->init();
}

MApplicationMenuView::~MApplicationMenuView()
{
    delete d_ptr;
}

void MApplicationMenuView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MApplicationMenuView);

    const MApplicationMenuStyle *s = static_cast<const MApplicationMenuStyle *>(style().operator ->());

    if (s->topShadowImage()) {
        s->topShadowImage()->draw(0.0, (qreal)-s->topShadowImage()->pixmap()->size().height(),
                                  boundingRect().width(), (qreal)s->topShadowImage()->pixmap()->size().height(),
                                  painter);
    }
    if (s->leftShadowImage()) {
        s->leftShadowImage()->draw((qreal)-s->leftShadowImage()->pixmap()->size().width(), 0.0,
                                   (qreal)s->leftShadowImage()->pixmap()->size().width(), boundingRect().height(),
                                   painter);
    }
    if (s->bottomShadowImage()) {
        s->bottomShadowImage()->draw(0.0, boundingRect().height(),
                                     boundingRect().width(), (qreal)s->bottomShadowImage()->pixmap()->size().height(),
                                     painter);
    }
    if (s->rightShadowImage()) {
        s->rightShadowImage()->draw(boundingRect().width(), 0.0,
                                    (qreal)s->rightShadowImage()->pixmap()->size().width(), boundingRect().height(),
                                    painter);
    }

    if (s->canvasOpacity() > 0.0) {
        // draw canvas as a background of the pannable area
        painter->setOpacity(d->controller->effectiveOpacity() * s->canvasOpacity());
        QRectF layoutGeometry = d->actionCommandViewport->geometry();
        if (s->canvasImage()) {
            s->canvasImage()->draw(layoutGeometry.toRect(), painter);
        } else {
            QColor color = s->canvasColor();
            painter->fillRect(layoutGeometry, QBrush(color));
        }
    } else {
        MSceneWindowView::drawBackground(painter, option);
    }
}

void MApplicationMenuView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    MSceneWindowView::drawContents(painter, option);
}

void MApplicationMenuView::applyStyle()
{
    Q_D(MApplicationMenuView);

    MSceneWindowView::applyStyle();

    d->updateStyleNames();
}

M_REGISTER_VIEW_NEW(MApplicationMenuView, MApplicationMenu)
#include "moc_mapplicationmenuview.cpp"
