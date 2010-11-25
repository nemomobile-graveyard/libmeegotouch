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

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneEvent>

#include "mapplicationmenuview.h"
#include "mapplicationmenuview_p.h"
#include "mapplicationmenu.h"
#include "mbutton.h"
#include "mbuttongroup.h"
#include "mviewcreator.h"
#include "mtheme.h"
#include "mwidgetaction.h"
#include "mscalableimage.h"
#include "mpannableviewport.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscenemanager.h"
#include "mwindow.h"
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mgridlayoutpolicy.h"
#include "mbasiclistitem.h"
#include "mcombobox.h"

const int maxCommandActionsWithStyle = 6;
const int maxColumns = 1;

MApplicationMenuViewPrivate::MApplicationMenuViewPrivate(MApplicationMenu *menu)
    : q_ptr(0),
      controllerLayout(0),
      styleCommandLayout(0),
      actionCommandLayout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      stylePolicy(0),
      styleButtonGroup(0),
      leasedWidgets(),
      buttons(),
      controller(menu)
{
    //mainlayout for the application menu
    controllerLayout = new QGraphicsLinearLayout(Qt::Vertical, controller);
    controllerLayout->setContentsMargins(0, 0, 0, 0);
    controllerLayout->setSpacing(0);

    //pannable layout for the action commmands
    actionCommandLayout = new MLayout();
    actionCommandLayout->setContentsMargins(0, 0, 0, 0);
    actionCommandLayout->setMinimumSize(0,0);
    QGraphicsWidget* actionWidget = new QGraphicsWidget(controller);
    actionWidget->setLayout(actionCommandLayout);
    actionCommandViewport = new MPannableViewport(controller);
    actionCommandViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    actionCommandViewport->setWidget(actionWidget);

    //layout for the style commands
    styleCommandLayout = new MLayout();
    styleCommandLayout->setContentsMargins(0, 0, 0, 0);
    stylePolicy = new MLinearLayoutPolicy(styleCommandLayout, Qt::Horizontal);
    stylePolicy->setContentsMargins(0, 0, 0, 0);
    stylePolicy->setSpacing(0);
    styleButtonGroup = new MButtonGroup();
    styleButtonGroup->setExclusive(true);

    //add sublayouts to the mainlayout
    controllerLayout->addItem(styleCommandLayout);
    controllerLayout->addItem(actionCommandViewport);
    controllerLayout->setAlignment(actionCommandViewport, Qt::AlignCenter);

    controller->installEventFilter(this);
}

MApplicationMenuViewPrivate::~MApplicationMenuViewPrivate()
{
    clearWidgets(leasedWidgets);
    clearWidgets(buttons);
    removeEventFilter(controller);
    delete styleButtonGroup;
}

void MApplicationMenuViewPrivate::init()
{
    Q_Q(MApplicationMenuView);
    
    createPolicy(M::Landscape);
    createPolicy(M::Portrait);

    addActions();
    
    q->connect(controller, SIGNAL(displayEntered()), SLOT(_q_displayEntered()));
}

void MApplicationMenuViewPrivate::createPolicy(M::Orientation orientation)
{
    if (orientation == M::Landscape) {
        landscapePolicy = new MGridLayoutPolicy(actionCommandLayout);
        actionCommandLayout->setLandscapePolicy(landscapePolicy);
        landscapePolicy->setStyleName("menulandscape");
    } else {
        portraitPolicy = new MLinearLayoutPolicy(actionCommandLayout, Qt::Vertical);
        actionCommandLayout->setPortraitPolicy(portraitPolicy);
        portraitPolicy->setStyleName("menuportrait");
    }
}

void MApplicationMenuViewPrivate::add(QAction *action, QAction *before)
{
    if (!action ||
            !isLocationValid(action, MAction::ApplicationMenuLocation) ||
            (hasWidget(action) && !isWidgetUsable(action)) ||
            !canAddMoreActions(action)) {
        return;
    }

    MWidget *w = createWidget(action);
    // add to policies only if the action is visible
    if (action->isVisible()) {
        MWidget *beforeWidget = getWidget(before);
        if (isStyleAction(action)) {
            addStyleWidget(w, beforeWidget);
        } else {
            addActionCommandWidget(w, beforeWidget);
        }
    } else if (w) {
        w->setVisible(false);
    }
}

void MApplicationMenuViewPrivate::remove(QAction *action)
{
    MWidget *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;
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
    refreshPolicies(true);
}

void MApplicationMenuViewPrivate::change(QAction *action)
{
    if (!changeLocation(action)) {
        return;
    }
    changeData(action);
    changeVisibility(action);
    changeStyleAttribute(action);
}

void MApplicationMenuViewPrivate::_q_displayEntered()
{
    actionCommandViewport->setPosition(QPointF(0,0));
}

bool MApplicationMenuViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    switch (e->type()) {
        case QEvent::ActionRemoved: {
            QActionEvent *actionEvent = static_cast<QActionEvent *>(e);
            remove(actionEvent->action());
            makeLandscapePolicyColumnsEqual();
            break;
        }
        case QEvent::ActionAdded: {
            QActionEvent *actionEvent = static_cast<QActionEvent *>(e);
            add(actionEvent->action(), actionEvent->before());
            makeLandscapePolicyColumnsEqual();
            break;
        }
        case QEvent::ActionChanged: {
            QActionEvent *actionEvent = static_cast<QActionEvent *>(e);
            change(actionEvent->action());
            break;
        }
        default: {
            break;
        }
    }
    return QObject::eventFilter(obj, e);
}

void MApplicationMenuViewPrivate::addActions()
{
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        add(acts.at(i), 0);
    }
}

MWidget *MApplicationMenuViewPrivate::createWidget(QAction *action)
{
    // If widget is not already created then create it
    MWidget *widget = buttons.value(action);
    if (!widget) {
        widget = leasedWidgets.value(action);
    }
    if (!widget) {
        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
        if (widgetAction) {
            widget = requestWidget(widgetAction);
            if (widget) {
                MComboBox *comboBox = qobject_cast<MComboBox *>(widget);
                if (comboBox) {
                    comboBox->setStyleName("menucomboboxcommand");
                    connect(comboBox, SIGNAL(clicked()), widgetAction, SIGNAL(triggered()));
                }
                leasedWidgets.insert(action, widget);
            }
        }
    }
    if (!widget) {
        widget = createButton(action);
        buttons.insert(action, widget);
    }

    connect(action, SIGNAL(triggered()), controller, SLOT(disappear()));
    widget->setVisible(true);
    widget->setEnabled(action->isEnabled());
    return widget;
}

MWidget *MApplicationMenuViewPrivate::createButton(QAction *action)
{
    MWidget *w = 0;
    bool isStyle = isStyleAction(action);
    if (isStyle) {
        MButton *button = new MButton(action->text());
        MAction *mAction = qobject_cast<MAction *>(action);
        if (mAction) {
            button->setIconID(mAction->iconID());
        }
        connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered()));
        button->setCheckable(true);
        button->setChecked(action->isChecked());
        styleButtonGroup->addButton(button);
        button->setObjectName("menustylecommand");
        w = button;
    } else {
        MBasicListItem *item = new MBasicListItem(MBasicListItem::SingleTitle);
        item->setTitle(action->text());
        item->setObjectName("menuactioncommand");
        connect(item, SIGNAL(clicked()), action, SIGNAL(triggered()));
        w = item;
    }

    if (w && action && !action->objectName().isEmpty())
        w->setObjectName(w->objectName() + '_' + action->objectName());

    return w;
}

bool MApplicationMenuViewPrivate::isStyleAction(QAction *action) const
{
    bool isStyleAction = false;
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction && mAction->isStyleAction()) {
        isStyleAction = true;
    }
    return isStyleAction;
}

bool MApplicationMenuViewPrivate::isLocationValid(QAction *action, MAction::Location loc)
{
    bool valid = true;
    MAction *mAction = qobject_cast<MAction *>(action);
    if (mAction) {
        valid = mAction->location().testFlag(loc);
    }
    return valid;
}

bool MApplicationMenuViewPrivate::isVisible(QAction *action)
{
    return action &&
           action->isVisible();
}

void MApplicationMenuViewPrivate::clearWidgets(QHash<QAction *, MWidget *>& widgets)
{
    QHashIterator<QAction *, MWidget *> iterator(widgets);
    while (iterator.hasNext()) {
        iterator.next();
        deleteWidget(iterator.key(), iterator.value());
    }
    widgets.clear();
}

void MApplicationMenuViewPrivate::deleteWidget(QAction *action, MWidget *widget)
{
    if (!releaseWidget(action, widget)) {
        delete widget;
    }
}

bool MApplicationMenuViewPrivate::releaseWidget(QAction *action, MWidget *widget)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        MComboBox *comboBox = qobject_cast<MComboBox *>(widget);
        if (comboBox) {
            disconnect(comboBox, SIGNAL(clicked()), widgetAction, SIGNAL(triggered()));
        }
        widgetAction->releaseWidget(widget);
    }
    return (widgetAction != 0);
}

MWidget *MApplicationMenuViewPrivate::requestWidget(MAction *action)
{
    MWidget *widget = 0;
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        widget = widgetAction->requestWidget(controller);
    }
    return widget;
}

bool MApplicationMenuViewPrivate::isWidgetInUseByView(MWidgetAction *widgetAction)
{
    return (buttons.contains(widgetAction) || leasedWidgets.contains(widgetAction));
}

bool MApplicationMenuViewPrivate::isWidgetUsable(QAction *action)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    return(widgetAction && isWidgetUsable(widgetAction));
}

bool MApplicationMenuViewPrivate::hasWidget(QAction *action)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    return(widgetAction && widgetAction->widget());
}

bool MApplicationMenuViewPrivate::isWidgetUsable(MWidgetAction *widgetAction)
{
    return (!widgetAction->isWidgetInUse() || isWidgetInUseByView(widgetAction));
}

MWidget *MApplicationMenuViewPrivate::getWidget(QAction *action) const
{
    MWidget *button = buttons.value(action);
    return button ? button : leasedWidgets.value(action);
}

bool MApplicationMenuViewPrivate::canAddMoreActions(QAction *action) const
{
    bool canAdd = true;

    if (isStyleAction(action)) {
        int count = 0;
        if (!actionCountAndExists(action, true, count)) {
            canAdd = count < maxCommandActionsWithStyle;
        }
    }
    else { // is Command Action
        int count = 0;
        if (!actionCountAndExists(action, false, count)) {
            canAdd = true;
        }
    } // Command action

    return canAdd;
}

bool MApplicationMenuViewPrivate::actionCountAndExists(QAction *action, bool isStyle, int& count) const
{
    count = 0;
    bool exists = actionCountAndExists(action, buttons, isStyle, count);
    exists |= actionCountAndExists(action, leasedWidgets, isStyle, count);
    return exists;
}

// Iterate through all of the specified widgets to count actions of specified type (style or command)
// while simultaneously checking if incoming action is contained in the list.
// These operations are combined here to avoid having to traverse the list twice.
bool MApplicationMenuViewPrivate::actionCountAndExists(QAction *action,
                                                       QHash<QAction *, MWidget *> const &widgets, bool isStyle, int& count ) const
{
    bool exists = false;
    QHashIterator<QAction *, MWidget *> iterator(widgets);
    while (iterator.hasNext()) {
        iterator.next();
        QAction *widgetAction = iterator.key();
        if (isStyle == isStyleAction(widgetAction)) {
            ++count;
        }
        if (widgetAction == action) {
            exists = true;
        }
    }
    return exists;
}

bool MApplicationMenuViewPrivate::changeLocation(QAction *action)
{
    // If the location of an action gets changed then
    // remove it from the applicationmenu
    if (!isLocationValid(action, MAction::ApplicationMenuLocation)) {
        remove(action);
        return false;
    }
    if (!getWidget(action)) {
        //action does not exit (e.g. location has been reverted back)
        add(action, 0);
    }
    return true;
}

void MApplicationMenuViewPrivate::changeData(QAction *action)
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction && widgetAction->widget() != leasedWidgets.value(action)) {
        // Widget in widgetAction is changed
        MWidget *widget = buttons.value(action);
        if (widget) {
            buttons.remove(action);
            delete widget;
        } else if (leasedWidgets.value(action))
            leasedWidgets.remove(action);

        disconnect(action, SIGNAL(triggered()), controller, SLOT(disappear()));

        add(action, 0);

        return;
    }

    MWidget *widget = buttons.value(action);
    MButton *button = qobject_cast<MButton *>(widget);

    if (widget)
        widget->setEnabled(action->isEnabled());
    if (button) {
        // Update button data accordingly
        button->setText(action->text());
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());
        MAction *mAction = qobject_cast<MAction *>(action);
        if (mAction) {
            button->setIconID(mAction->iconID());
        }
    } else {
        MBasicListItem *item = qobject_cast<MBasicListItem *>(widget);
        if (item)
            item->setTitle(action->text());
    }
}

void MApplicationMenuViewPrivate::changeVisibility(QAction *action)
{
    MWidget *widget = getWidget(action);
    if (widget) {
        bool wasVisible = (landscapePolicy->indexOf(widget) >= 0) ||
                          (portraitPolicy->indexOf(widget) >= 0);
        //Check if visibility has been changed
        bool visibilityChanged = (!action->isVisible() && wasVisible) ||
                                 (action->isVisible() && !wasVisible);

        refreshPolicies(visibilityChanged);
    }
}

void MApplicationMenuViewPrivate::changeStyleAttribute(QAction *action)
{
    MWidget *widget = getWidget(action);
    if (widget) {
        bool wasStyleCommand = (stylePolicy->indexOf(widget) >= 0);

        //Check if style attribute has been changed
        bool isStyleCommand = isStyleAction(action);
        bool changed = (wasStyleCommand && !isStyleCommand) ||
                       (!wasStyleCommand && isStyleCommand);

        refreshPolicies(changed);
    }
}

void MApplicationMenuViewPrivate::refreshPolicies(bool refresh)
{
    if (refresh) {
        clearPolicy(landscapePolicy);
        clearPolicy(portraitPolicy);
        clearPolicy(stylePolicy);
        addActions();
        updateItemMode();
    }
}

void MApplicationMenuViewPrivate::clearPolicy(MAbstractLayoutPolicy *policy)
{
    while (policy->count()) {
        policy->removeAt(0);
    }
}

void MApplicationMenuViewPrivate::refreshLandscapePolicy()
{
    clearPolicy(landscapePolicy);
    int index = 0;
    QAction *action = 0;
    QList<QAction *> acts = controller->actions();
    int count = acts.count();
    for (int i = 0; i < count; ++i) {
        action = acts.at(i);
        MWidget *w = getWidget(action);
        if (w && action->isVisible() && !isStyleAction(action)) {
            landscapePolicy->addItem(w, index / maxColumns, index % maxColumns);
            index++;
        }
    }
    updateItemMode();
}

void MApplicationMenuViewPrivate::updateItemMode()
{

    MAbstractLayoutPolicy *policy = landscapePolicy;
    int columnsCount = maxColumns;
    MWindow *window = MApplication::instance()->activeWindow();
    if (window && M::Portrait == window->orientation()) {
            policy = portraitPolicy;
            columnsCount = 1;
     }
    int count = policy->count();

    for (int index = 0; index < count; index++) {
        MWidget *w = (MWidget *)policy->itemAt(index);
        if (qobject_cast<MBasicListItem *>(w) || qobject_cast<MComboBox *>(w)) {
            //Only for MBasicListItem and MComboBox types (those styleaction buttons are ruled out like before)
            MWidgetController *widget = qobject_cast<MWidgetController *>(w);
            widget->setLayoutPosition(calculateLayoutPosition(columnsCount, count, index));
        }
    }
}

M::Position MApplicationMenuViewPrivate::calculateLayoutPosition(
        int columnsCount,
        int itemCount,
        int index)
{
    M::Position pos = M::DefaultPosition;

    // Empty or single cell
    if (itemCount <= 1 || columnsCount == 0) {
        pos = M::DefaultPosition;
    }
    // Single vertical column
    else if (columnsCount == 1) {
        pos = index == 0             ? M::VerticalTopPosition :
              index == itemCount - 1 ? M::VerticalBottomPosition :
                                       M::VerticalCenterPosition;
    }
    else {
        int rowCount = itemCount/columnsCount;
        if (rowCount * columnsCount < itemCount) {
            ++rowCount;
        }
        // Single horizontal row
        if ( rowCount == 1) {
            pos = index == 0            ? M::HorizontalLeftPosition :
                  index == itemCount -1 ? M::HorizontalRightPosition :
                                          M::HorizontalCenterPosition;
        }
        // 2-D layout
        else {
            int row = index / columnsCount;
            int col = index % columnsCount;
            pos = row == 0          && col == 0              ? M::TopLeftPosition :
                  row == 0          && col == columnsCount-1 ? M::TopRightPosition :
                  row == rowCount-1 && col == 0              ? M::BottomLeftPosition :
                  row == rowCount-1 && col == columnsCount-1 ? M::BottomRightPosition :
                  row == 0                                   ? M::TopCenterPosition :
                  row == rowCount-1                          ? M::BottomCenterPosition :
                  col == 0                                   ? M::CenterLeftPosition :
                  col == columnsCount-1                      ? M::CenterRightPosition :
                                                               M::CenterPosition;
        } // 2-D layout
    }
    return pos;
}

void MApplicationMenuViewPrivate::addStyleWidget(MWidget *widget, MWidget *before)
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

void MApplicationMenuViewPrivate::addActionCommandWidget(MWidget *widget, MWidget *before)
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

void MApplicationMenuViewPrivate::removeStyleWidget(MWidget *widget)
{
    styleCommandLayout->removeItem(widget);
    MButton *button = qobject_cast<MButton *>(widget);
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

void MApplicationMenuViewPrivate::addStyleSpacer()
{
    QGraphicsWidget *w = new QGraphicsWidget(controller);
    w->setMinimumSize(0, 0);
    w->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    stylePolicy->addItem(w);
}

void MApplicationMenuViewPrivate::makeLandscapePolicyColumnsEqual()
{
    Q_Q(MApplicationMenuView);
    QSize sceneSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    if (controller->sceneManager())
        sceneSize = controller->sceneManager()->visibleSceneSize(M::Landscape);
    else if (MApplication::activeWindow())
        sceneSize = MApplication::activeWindow()->visibleSceneSize(M::Landscape);
    else if (!MApplication::windows().isEmpty())
        sceneSize = MApplication::windows().at(0)->visibleSceneSize(M::Landscape);
    qreal l, r;
    actionCommandLayout->getContentsMargins(&l, 0, &r, 0);
    int width = (sceneSize.width() -
                 q->style()->marginLeft() - q->style()->marginRight() -
                 q->style()->paddingLeft() - q->style()->paddingRight() -
                 l - r) ;

    if (landscapePolicy->count() >= maxColumns) {
        int columnWidth = width/maxColumns;
        int c = 0;
        for (; c < maxColumns; ++c) {
            landscapePolicy->setColumnFixedWidth(c, columnWidth);
        }
        for (; c < landscapePolicy->columnCount(); ++c) {          
            landscapePolicy->setColumnPreferredWidth(c, 0);      
        }        
    } else if( landscapePolicy->count() > 0 ){
        int columnWidth = width/landscapePolicy->count();
        int c = 0;
        for (; c < landscapePolicy->count(); ++c) {
            landscapePolicy->setColumnPreferredWidth(c, columnWidth);
        }
        for (; c < landscapePolicy->columnCount(); ++c) {          
            landscapePolicy->setColumnPreferredWidth(c, 0);      
        }
    } else {
        for (int c = 0; c < landscapePolicy->columnCount(); ++c) {          
            landscapePolicy->setColumnPreferredWidth(c, 0);      
        }
    }
}

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
    Q_UNUSED(option);

    Q_D(const MApplicationMenuView);

    if (style()->canvasOpacity() > 0.0) {
        // draw canvas as a background of the pannable area
        painter->setOpacity(d->controller->effectiveOpacity() * style()->canvasOpacity());
        QRectF layoutGeometry = d->actionCommandViewport->geometry();
        if (style()->canvasImage()) {
            style()->canvasImage()->draw(layoutGeometry.toRect(), painter);
        } else {
            QColor color = style()->canvasColor();
            painter->fillRect(layoutGeometry, QBrush(color));
        }
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

    d->makeLandscapePolicyColumnsEqual();
    d->updateItemMode();
}

M_REGISTER_VIEW_NEW(MApplicationMenuView, MApplicationMenu)
#include "moc_mapplicationmenuview.cpp"
