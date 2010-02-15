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

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneEvent>

#include "duiapplicationmenuview_p.h"
#include "duiapplicationmenuview.h"
#include "duiapplicationmenu.h"
#include "duibutton.h"
#include "duibuttongroup.h"
#include "duiviewcreator.h"
#include "duitheme.h"
#include "duiwidgetaction.h"
#include "duiscalableimage.h"
#include "duipannableviewport.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiscenemanager.h"
#include "duiwindow.h"
#include "duilayout.h"
#include "duilinearlayoutpolicy.h"
#include "duigridlayoutpolicy.h"
#include "duicontentitem.h"
#include "duicombobox.h"

const int maxCommandActions = 8;
const int maxCommandActionsWithStyle = 6;

DuiApplicationMenuViewPrivate::DuiApplicationMenuViewPrivate(DuiApplicationMenu *menu)
    : controllerLayout(0),
      styleCommandLayout(0),
      actionCommandLayout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      stylePolicy(0),
      styleButtonGroup(0),
      leasedWidgets(),
      buttons()
{
    controller = menu;

    controllerLayout = new QGraphicsLinearLayout(Qt::Vertical, controller);
    controllerLayout->setContentsMargins(0, 0, 0, 0);
    controllerLayout->setSpacing(0);

    actionCommandLayout = new DuiLayout();
    actionCommandLayout->setContentsMargins(0, 0, 0, 0);
    actionCommandLayout->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    styleCommandLayout = new DuiLayout();
    styleCommandLayout->setContentsMargins(0, 0, 0, 0);
    styleCommandLayout->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    controllerLayout->addItem(styleCommandLayout);
    controllerLayout->addItem(actionCommandLayout);

    stylePolicy = new DuiLinearLayoutPolicy(styleCommandLayout, Qt::Horizontal);
    stylePolicy->setContentsMargins(0, 0, 0, 0);
    stylePolicy->setSpacing(0);

    styleButtonGroup = new DuiButtonGroup();
    styleButtonGroup->setExclusive(true);

    controller->installEventFilter(this);
}

DuiApplicationMenuViewPrivate::~DuiApplicationMenuViewPrivate()
{
    clearWidgets(leasedWidgets);
    clearWidgets(buttons);
    removeEventFilter(controller);
}

void DuiApplicationMenuViewPrivate::init()
{
    createPolicy(Dui::Landscape);
    createPolicy(Dui::Portrait);

    addActions();
}

void DuiApplicationMenuViewPrivate::createPolicy(Dui::Orientation orientation)
{
    if (orientation == Dui::Landscape) {
        landscapePolicy = new DuiGridLayoutPolicy(actionCommandLayout);
        actionCommandLayout->setLandscapePolicy(landscapePolicy);
        landscapePolicy->setObjectName("menulandscape");
    } else {
        portraitPolicy = new DuiLinearLayoutPolicy(actionCommandLayout, Qt::Vertical);
        actionCommandLayout->setPortraitPolicy(portraitPolicy);
        portraitPolicy->setObjectName("menuportrait");
    }
}

void DuiApplicationMenuViewPrivate::add(QAction *action, QAction *before)
{
    if (!action ||
            !isLocationValid(action, DuiAction::ApplicationMenuLocation) ||
            (hasWidget(action) && !isWidgetUsable(action)) ||
            !canAddMoreActions(action)) {
        return;
    }

    DuiWidget *w = createWidget(action);
    // add to policies only if the action is visible
    if (action->isVisible()) {
        DuiWidget *beforeWidget = getWidget(before);
        if (isStyleAction(action)) {
            addStyleWidget(w, beforeWidget);
        } else {
            addActionCommandWidget(w, beforeWidget);
        }
    } else if (w) {
        w->setVisible(false);
    }
}

void DuiApplicationMenuViewPrivate::remove(QAction *action)
{
    DuiWidget *button = buttons.value(action);
    DuiWidget *leased = leasedWidgets.value(action);
    DuiWidget *widget = (button != 0) ? button : leased;
    if (widget) {
        if (isStyleAction(action)) {
            removeStyleWidget(widget);
        } else {
            actionCommandLayout->removeItem(widget);
            updateItemMode();
        }
    }
    if (button) {
        buttons.remove(action);
        delete button;
    } else if (leased) {
        releaseWidget(action, leased);
        leasedWidgets.remove(action);
    }
    disconnect(action, SIGNAL(triggered()), controller, SLOT(disappear()));
}

void DuiApplicationMenuViewPrivate::change(QAction *action)
{
    if (!changeLocation(action)) {
        return;
    }
    changeData(action);
    changeVisibility(action);
    changeStyleAttribute(action);
}

bool DuiApplicationMenuViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    QActionEvent *actionEvent = dynamic_cast<QActionEvent *>(e);

    if (actionEvent) {
        switch (e->type()) {
        case QEvent::ActionRemoved: {
            remove(actionEvent->action());
            break;
        }
        case QEvent::ActionAdded: {
            add(actionEvent->action(), actionEvent->before());
            break;
        }
        case QEvent::ActionChanged: {
            change(actionEvent->action());
            break;
        }
        default: {
            break;
        }
        }
    }

    return QObject::eventFilter(obj, e);
}

void DuiApplicationMenuViewPrivate::addActions()
{
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        add(acts.at(i), 0);
    }
}

DuiWidget *DuiApplicationMenuViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    DuiWidget *widget = buttons.value(action);
    if (!widget) {
        widget = leasedWidgets.value(action);
    }
    if (!widget) {
        DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
        if (widgetAction) {
            widget = requestWidget(widgetAction);
            leasedWidgets.insert(action, widget);
        } else {
            widget = createButton(action);
            buttons.insert(action, widget);
        }
    }
    connect(action, SIGNAL(triggered()), controller, SLOT(disappear()));
    widget->setVisible(true);
    widget->setEnabled(action->isEnabled());
    return widget;
}

DuiWidget *DuiApplicationMenuViewPrivate::createButton(QAction *action)
{
    DuiWidget *w = 0;
    bool isStyle = isStyleAction(action);
    if (isStyle) {
        DuiButton *button = new DuiButton(action->text());
        DuiAction *duiAction = qobject_cast<DuiAction *>(action);
        if (duiAction) {
            button->setIconID(duiAction->iconID());
        }
        connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
        button->setCheckable(true);
        button->setChecked(action->isChecked());
        styleButtonGroup->addButton(button);
        button->setObjectName("menustylecommand");
        w = button;
    } else {
        DuiContentItem *item = new DuiContentItem(DuiContentItem::SingleTextLabel);
        item->setTitle(action->text());
        item->setObjectName("menuactioncommand");
        connect(item, SIGNAL(clicked()), action, SIGNAL(triggered()));
        w = item;
    }
    return w;
}

bool DuiApplicationMenuViewPrivate::isStyleAction(QAction *action)
{
    bool isStyleAction = false;
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction && duiAction->isStyleAction()) {
        isStyleAction = true;
    }
    return isStyleAction;
}

bool DuiApplicationMenuViewPrivate::isLocationValid(QAction *action, DuiAction::Location loc)
{
    bool valid = true;
    DuiAction *duiAction = qobject_cast<DuiAction *>(action);
    if (duiAction) {
        valid = duiAction->location().testFlag(loc);
    }
    return valid;
}

bool DuiApplicationMenuViewPrivate::isVisible(QAction *action)
{
    return action &&
           action->isVisible();
}

void DuiApplicationMenuViewPrivate::clearWidgets(QHash<QAction *, DuiWidget *>& widgets)
{
    QHashIterator<QAction *, DuiWidget *> iterator(widgets);
    while (iterator.hasNext()) {
        iterator.next();
        deleteWidget(iterator.key(), iterator.value());
    }
    widgets.clear();
}

void DuiApplicationMenuViewPrivate::deleteWidget(QAction *action, DuiWidget *widget)
{
    if (!releaseWidget(action, widget)) {
        delete widget;
    }
}

bool DuiApplicationMenuViewPrivate::releaseWidget(QAction *action, DuiWidget *widget)
{
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    if (widgetAction) {
        widgetAction->releaseWidget(widget);
    }
    return (widgetAction != 0);
}

DuiWidget *DuiApplicationMenuViewPrivate::requestWidget(DuiAction *action)
{
    DuiWidget *widget = 0;
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    if (widgetAction) {
        widget = widgetAction->requestWidget(controller);
    }
    return widget;
}

bool DuiApplicationMenuViewPrivate::isWidgetInUseByView(DuiWidgetAction *widgetAction)
{
    return (buttons.contains(widgetAction) || leasedWidgets.contains(widgetAction));
}

bool DuiApplicationMenuViewPrivate::isWidgetUsable(QAction *action)
{
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    return(widgetAction && isWidgetUsable(widgetAction));
}

bool DuiApplicationMenuViewPrivate::hasWidget(QAction *action)
{
    DuiWidgetAction *widgetAction = qobject_cast<DuiWidgetAction *>(action);
    return(widgetAction && widgetAction->widget());
}

bool DuiApplicationMenuViewPrivate::isWidgetUsable(DuiWidgetAction *widgetAction)
{
    return (!widgetAction->isWidgetInUse() || isWidgetInUseByView(widgetAction));
}

DuiWidget *DuiApplicationMenuViewPrivate::getWidget(QAction *action)
{
    DuiWidget *button = buttons.value(action);
    DuiWidget *leased = leasedWidgets.value(action);
    return (button != 0) ? button : leased;
}

bool DuiApplicationMenuViewPrivate::canAddMoreActions(QAction *action)
{
    bool canAdd = true;
    if (!isStyleAction(action)) {
        int commandActionsCount, styleActionsCount;
        visibleActionsCount(commandActionsCount, styleActionsCount);
        canAdd = (commandActionsCount < maxCommandActions);
        if (styleActionsCount > 0)
            canAdd = (commandActionsCount < maxCommandActionsWithStyle);
    }
    return canAdd;
}

void DuiApplicationMenuViewPrivate::visibleActionsCount(int &commandActionsCount, int &styleActionsCount)
{
    commandActionsCount = 0;
    styleActionsCount = 0;
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        QAction *action = acts.at(i);
        bool isStyle = isStyleAction(action);
        styleActionsCount += isStyle;
        commandActionsCount += (!isStyle);
    }
}

bool DuiApplicationMenuViewPrivate::changeLocation(QAction *action)
{
    // If the location of an action gets changed then
    // remove it from the applicationmenu
    if (!isLocationValid(action, DuiAction::ApplicationMenuLocation)) {
        remove(action);
        return false;
    }
    if (!getWidget(action)) {
        //action does not exit (e.g. location has been reverted back)
        add(action, 0);
    }
    return true;
}

void DuiApplicationMenuViewPrivate::changeData(QAction *action)
{
    DuiWidget *widget = buttons.value(action);
    DuiButton *button = qobject_cast<DuiButton *>(widget);
    if (button) {
        // Update button data accordingly
        button->setText(action->text());
        button->setEnabled(action->isEnabled());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        DuiAction *duiAction = qobject_cast<DuiAction *>(action);
        if (duiAction) {
            button->setIconID(duiAction->iconID());
        }
    }
}

void DuiApplicationMenuViewPrivate::changeVisibility(QAction *action)
{
    DuiWidget *widget = getWidget(action);
    if (widget) {
        bool wasVisible = (landscapePolicy->indexOf(widget) >= 0) ||
                          (portraitPolicy->indexOf(widget) >= 0);
        //Check if visibility has been changed
        bool visibilityChanged = (!action->isVisible() && wasVisible) ||
                                 (action->isVisible() && !wasVisible);

        refreshPolicies(visibilityChanged);
    }
}

void DuiApplicationMenuViewPrivate::changeStyleAttribute(QAction *action)
{
    DuiWidget *widget = getWidget(action);
    if (widget) {
        bool wasStyleCommand = (stylePolicy->indexOf(widget) >= 0);

        //Check if style attribute has been changed
        bool isStyleCommand = isStyleAction(action);
        bool changed = (wasStyleCommand && !isStyleCommand) ||
                       (!wasStyleCommand && isStyleCommand);

        refreshPolicies(changed);
    }
}

void DuiApplicationMenuViewPrivate::refreshPolicies(bool refresh)
{
    if (refresh) {
        clearPolicy(landscapePolicy);
        clearPolicy(portraitPolicy);
        clearPolicy(stylePolicy);
        addActions();
        updateItemMode();
    }
}

void DuiApplicationMenuViewPrivate::clearPolicy(DuiAbstractLayoutPolicy *policy)
{
    while (policy->count()) {
        policy->removeAt(0);
    }
}

void DuiApplicationMenuViewPrivate::refreshLandscapePolicy()
{
    clearPolicy(landscapePolicy);
    int index = 0;
    QAction *action = 0;
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        action = acts.at(i);
        DuiWidget *w = getWidget(action);
        if (w && action->isVisible() && !isStyleAction(action)) {
            landscapePolicy->addItem(w, index / 2, index % 2);
            index++;
        }
    }
    updateItemMode();
}

void DuiApplicationMenuViewPrivate::updateItemMode()
{

    DuiAbstractLayoutPolicy *policy = landscapePolicy;
    int columnsCount = 2;
    DuiWindow *window = DuiApplication::instance()->activeWindow();
    if (window && Dui::Portrait == window->orientation()) {
        policy = portraitPolicy;
        columnsCount = 1;
    }

    int count = policy->count();

    for (int index = 0; index < count; index++) {
        DuiWidget *w = (DuiWidget *)policy->itemAt(index);
        DuiContentItem *contentItem = qobject_cast<DuiContentItem *>(w);
        updateContentItemMode(contentItem, columnsCount, count, index);
        DuiComboBox *combobox = qobject_cast<DuiComboBox *>(w);
        updateComboboxMode(combobox, columnsCount, count, index);
    }

}

void DuiApplicationMenuViewPrivate::updateContentItemMode(DuiContentItem *contentItem,
        int columnsCount,
        int itemCount,
        int index)
{
    if (!contentItem)
        return;

    if (columnsCount == 1) {
        if (index == itemCount - 1) {
            contentItem->setItemMode(DuiContentItem::SingleColumnBottom);
        } else {
            contentItem->setItemMode(DuiContentItem::SingleColumnCenter);
        }
    } else {
        int rowCount = (itemCount + 1) / 2;
        int row = index / 2;
        int col = index % 2;
        if ((itemCount == 1) || (col == 0 && index == (itemCount - 1))) { //only one item in last row
            contentItem->setItemMode(DuiContentItem::SingleColumnBottom);
        } else if (row >= 0 && row < (rowCount - 1)) {
            (col == 0) ? contentItem->setItemMode(DuiContentItem::Left) : contentItem->setItemMode(DuiContentItem::Right);
        } else {
            (col == 0) ? contentItem->setItemMode(DuiContentItem::BottomLeft) : contentItem->setItemMode(DuiContentItem::BottomRight);
        }
    }
}

void DuiApplicationMenuViewPrivate::updateComboboxMode(DuiComboBox *comboBox,
        int columnsCount,
        int itemCount,
        int index)
{
    if (!comboBox)
        return;

    if (columnsCount == 1) {
        if (index == itemCount - 1) {
            comboBox->setObjectName("MenuComboBoxSingleColumnBottom");
        } else {
            comboBox->setObjectName("MenuComboBoxSingleColumnCenter");
        }
    } else {
        int rowCount = (itemCount + 1) / 2;
        int row = index / 2;
        int col = index % 2;
        if ((itemCount == 1) || (col == 0 && index == (itemCount - 1))) { //only one item in last row
            comboBox->setObjectName("MenuComboBoxSingleColumnBottom");
        } else if (row >= 0 && row < (rowCount - 1)) {
            (col == 0) ? comboBox->setObjectName("MenuComboBoxLeft") : comboBox->setObjectName("MenuComboBoxRight");
        } else {
            (col == 0) ? comboBox->setObjectName("MenuComboBoxBottomLeft") : comboBox->setObjectName("MenuComboBoxBottomRight");
        }
    }
}

void DuiApplicationMenuViewPrivate::addStyleWidget(DuiWidget *widget, DuiWidget *before)
{
    int count = stylePolicy->count();
    if (count == 0) {
        addStyleSpacer();
        addStyleSpacer();
    }
    //insert at the second last position (spacer at the end and
    //in the beginning) if no before exists
    int index = stylePolicy->count() - 1;
    if (before) {
        int beforeIndex = -1;
        if ((beforeIndex = stylePolicy->indexOf(before)) >= 0) {
            index = beforeIndex;
        }
    }
    stylePolicy->insertItem(index, widget, Qt::AlignCenter);
}

void DuiApplicationMenuViewPrivate::addActionCommandWidget(DuiWidget *widget, DuiWidget *before)
{
    int portIndex = portraitPolicy->count();
    if (before) {
        int beforeIndex = -1;
        if ((beforeIndex = portraitPolicy->indexOf(before)) >= 0) {
            portIndex = beforeIndex;
        }
    }
    portraitPolicy->insertItem(portIndex, widget);

    //For landscape policy, there is no way to insert the items at any position
    //in the grid. So have to remove them first and then re-add them all
    refreshLandscapePolicy();
}

void DuiApplicationMenuViewPrivate::removeStyleWidget(DuiWidget *widget)
{
    styleCommandLayout->removeItem(widget);
    DuiButton *button = qobject_cast<DuiButton *>(widget);
    if (button) {
        styleButtonGroup->removeButton(button);
    }
    int count = stylePolicy->count();
    if (count == 2) { //delete the spacers as well
        while (stylePolicy->count()) {
            QGraphicsLayoutItem *item = stylePolicy->itemAt(0);
            stylePolicy->removeAt(0);
            delete item;
        }
    }
}

void DuiApplicationMenuViewPrivate::addStyleSpacer()
{
    QGraphicsWidget *w = new QGraphicsWidget(controller);
    w->setMinimumSize(0, 0);
    w->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    stylePolicy->addItem(w);
}

void DuiApplicationMenuViewPrivate::makeLandscapePolicyColumnsEqual()
{
    Q_Q(DuiApplicationMenuView);
    QSize sceneSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    if (controller->sceneManager())
        sceneSize = controller->sceneManager()->visibleSceneSize(Dui::Landscape);
    else if (DuiApplication::activeWindow())
        sceneSize = DuiApplication::activeWindow()->visibleSceneSize(Dui::Landscape);
    else if (!DuiApplication::windows().isEmpty())
        sceneSize = DuiApplication::windows().at(0)->visibleSceneSize(Dui::Landscape);
    qreal l, r;
    actionCommandLayout->getContentsMargins(&l, 0, &r, 0);
    int width = (sceneSize.width() -
                 q->style()->marginLeft() - q->style()->marginRight() -
                 q->style()->paddingLeft() - q->style()->paddingRight() -
                 l - r) / 2;
    landscapePolicy->setColumnPreferredWidth(0, width);
    landscapePolicy->setColumnPreferredWidth(1, width);
}

DuiApplicationMenuView::DuiApplicationMenuView(DuiApplicationMenu *controller) :
    DuiSceneWindowView(* new DuiApplicationMenuViewPrivate(controller), controller)
{
    Q_D(DuiApplicationMenuView);
    d->init();
}

DuiApplicationMenuView::DuiApplicationMenuView(DuiApplicationMenuViewPrivate &dd, DuiApplicationMenu *controller) :
    DuiSceneWindowView(dd, controller)
{
    Q_D(DuiApplicationMenuView);
    d->init();
}

DuiApplicationMenuView::~DuiApplicationMenuView()
{
}

void DuiApplicationMenuView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const DuiApplicationMenuView);

    if (style()->canvasOpacity() > 0.0) {
        // draw canvas
        painter->setOpacity(d->controller->effectiveOpacity() * style()->canvasOpacity());

        QRectF layoutGeometry = d->controllerLayout->geometry();

        if (style()->canvasImage()) {
            style()->canvasImage()->draw(layoutGeometry.toRect(), painter);
        } else {
            QColor color = style()->canvasColor();
            painter->fillRect(layoutGeometry, QBrush(color));
        }
    }
}

void DuiApplicationMenuView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    DuiSceneWindowView::drawContents(painter, option);
}

void DuiApplicationMenuView::applyStyle()
{
    Q_D(DuiApplicationMenuView);

    DuiSceneWindowView::applyStyle();

    d->makeLandscapePolicyColumnsEqual();
    d->updateItemMode();
}

DUI_REGISTER_VIEW_NEW(DuiApplicationMenuView, DuiApplicationMenu)

