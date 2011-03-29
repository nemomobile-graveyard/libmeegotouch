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

#include <QApplication>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneResizeEvent>
#include "mtoolbar.h"
#include "mtoolbar_p.h"
#include "mtoolbarlayoutpolicy.h"
#include "mtheme.h"
#include "mbutton.h"
#include "mbuttongroup.h"
#include "mwidgetaction.h"
#include "private/mwidgetview_p.h"

#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mtextedit.h"
#include "mtoolbarview.h"
#include "mtoolbarview_p.h"
#include "mscalableimage.h"
#include "mscenemanager.h"

MToolBarViewPrivate::MToolBarViewPrivate(MToolBar *controller)
    : QObject(),
      q_ptr(0),
      layout(0),
      landscapePolicy(0),
      portraitPolicy(0),
      buttonGroup(0),
      iconsEnabled(true),
      labelsEnabled(false),
      labelOnlyAsCommonButton(true),
      widgetAlignment(Qt::AlignHCenter),
      itemsEnabled(true),
      alignmentMargins(0),
      centerOffset(0)
{
    this->controller = controller;
    controller->installEventFilter(this);
    removedActionsButtons.setMaxCost(10);
}


MToolBarViewPrivate::~MToolBarViewPrivate()
{
    removeEventFilter(controller);
    if(buttonGroup) {
        delete buttonGroup;
        buttonGroup = NULL;
    }

    landscapePolicy->removeAllWidgets();
    portraitPolicy->removeAllWidgets();

    QHashIterator<QAction *, MWidget *> iterator(leasedWidgets);
    while (iterator.hasNext()) {
        iterator.next();
        releaseWidget(iterator.key(), iterator.value());
    }
    leasedWidgets.clear();

    qDeleteAll(buttons);
    removedActionsButtons.clear();
    delete layout;
    layout = NULL;
}

void MToolBarViewPrivate::init()
{
    layout = new MLayout;
    layout->setAnimation(NULL);
    layout->setContentsMargins(0, 0, 0, 0);

    landscapePolicy = new MToolBarLayoutPolicy(layout);
    landscapePolicy->setStyleName("ToolBarLandscapePolicy");
    layout->setLandscapePolicy(landscapePolicy);

    portraitPolicy = new MToolBarLayoutPolicy(layout);
    portraitPolicy->setStyleName("ToolBarPortraitPolicy");
    layout->setPortraitPolicy(portraitPolicy);

    QGraphicsLinearLayout *controllerlayout = (QGraphicsLinearLayout *)(controller->layout());
    controllerlayout->addItem(layout);

    //Add any existing actions now
    foreach(QAction *action, controller->actions()) {
        add(action);
    }

    updateWidgetAlignment();
}

int MToolBarViewPrivate::policyIndexForAddingAction(QAction *action, MToolBarLayoutPolicy *policy) const {
    Q_Q(const MToolBarView);
    //We need to add the action's given widget to the policy
    //This is a bit complicated because we ideally want to add it in the right place,
    //preserving the same order as in the controller->actions()
    QList< QAction *> actions = controller->actions();
    int parentIndex = actions.indexOf(action)+1;
    int index = policy->widgetCount();
    while(parentIndex < actions.size()) {
        MWidget *w = q->getWidget(actions.at(parentIndex));
        if(w) {
            int policyIndex = policy->widgetIndexOf(w);
            if(policyIndex >= 0) {
                index = policyIndex;
                break;
            }
        }
        parentIndex++;
    }
    if(policy->roomForWidget(index, hasTextEditWidget(action)))
        return index;
    return -1;
}

void MToolBarViewPrivate::add(QAction *action)
{
    if (!action || !action->isVisible() || hasUnusableWidget(action))
        return; //Cancel adding action

    // add to policies only if the action is visible
    int landscapeIndex = -1;
    int portraitIndex = -1;
    if (isLocationValid(action, MAction::ToolBarLandscapeLocation))
        landscapeIndex = policyIndexForAddingAction(action, landscapePolicy);
    if (isLocationValid(action, MAction::ToolBarPortraitLocation))
        portraitIndex = policyIndexForAddingAction(action, portraitPolicy);

    if (landscapeIndex == -1 && portraitIndex == -1)
        return; //Cancel adding action

    MWidget *widget = createWidget(action);
    if (!widget)
        return;

    updateWidgetFromAction(widget, action);

    if (landscapeIndex != -1)
        landscapePolicy->insertWidgetAndRemoveOverflow( landscapeIndex, widget );
    if (portraitIndex != -1)
        portraitPolicy->insertWidgetAndRemoveOverflow( portraitIndex, widget );

    updateEmptinessProperty();
}

void MToolBarViewPrivate::remove(QAction *action, bool hideOnly)
{
    Q_Q(MToolBarView);
    MButton *button = buttons.value(action);
    MWidget *leased = leasedWidgets.value(action);
    MWidget *widget = (button != 0) ? button : leased;
    if(!widget)
        return;

    if (button && buttonGroup)
        buttonGroup->removeButton(button);
    if(hideOnly) {
        landscapePolicy->removeWidget(widget);
        portraitPolicy->removeWidget(widget);
        if (button && buttonGroup)
            button->setChecked(false);
    } else {
        //Need to fully remove the action
        layout->removeItem(widget);

        if (button) {
            button->setChecked(false);
            button->hide();

            buttons.remove(action);
            MButton *removedButton = removedActionsButtons.take(action);
            if (removedButton && removedButton != button) {
                delete removedButton;
            }
            // Disconnect signals because these might end up being
            // connected again if the button is reused
            button->disconnect(q);
            button->disconnect(action);
            action->disconnect(q);

            removedActionsButtons.insert(action, button);
        } else {
            releaseWidget(action, leased);
            leasedWidgets.remove(action);
        }
    }

    //There might be space now for any actions not already added.  Signal a change action which
    //will check if an item now has room to be shown
    foreach(QAction *action2, controller->actions()) {
        if (action2 != action && action2->isVisible()) {
            if ((isLocationValid(action2, MAction::ToolBarLandscapeLocation) && landscapePolicy->freeWidgetSlots()) ||
                (isLocationValid(action2, MAction::ToolBarPortraitLocation) && portraitPolicy->freeWidgetSlots()))
                change(action2);
        }
    }

    updateEmptinessProperty();
}

void MToolBarViewPrivate::change(QAction *action)
{
    Q_Q(MToolBarView);

    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction && leasedWidgets.contains(action) &&
        (widgetAction->widget() != leasedWidgets.value(action))) {
        // Widget in action is changed
        if (!widgetAction->widget()) {
            remove(action, false);
            return;
        }
        leasedWidgets.remove(action);
    }

    if(hasUnusableWidget(action))
        return;
    if(!action->isVisible()) {
        remove(action, true); //Remove action, but only hiding the widget, not deleting/releasing it
        return;
    }
    bool validInLandscape = isLocationValid(action, MAction::ToolBarLandscapeLocation);
    bool validInPortrait = isLocationValid(action, MAction::ToolBarPortraitLocation);
    if (!validInLandscape && !validInPortrait) {
        remove(action, true);
        return;
    }

    //Check that the widget is in the controller actions
    QList< QAction *> actions = controller->actions();
    int indexOfAction = actions.indexOf(action);
    if(indexOfAction == -1) {
        remove(action, false); // I don't think this is possible
        return;
    }

    MWidget *widget = q->getWidget(action);
    if (!widget) {
        //We need to add the action
        add(action);
        return;
    }
    if(buttonGroup) {
        MButton *button = buttons.value(action);
        if(button)
            buttonGroup->addButton(button);
    }

    //We have now an action and a widget for it
    int landscapeIndex = landscapePolicy->widgetIndexOf(widget);
    int portraitIndex = portraitPolicy->widgetIndexOf(widget);
    if(!validInLandscape && landscapeIndex >= 0) {
        //We are showing it in landscape view but should not be
        landscapePolicy->removeWidgetAt(landscapeIndex);
    }
    if(!validInPortrait && portraitIndex >= 0) {
        //We are showing it in portrait view but should not be
        portraitPolicy->removeWidgetAt(portraitIndex);
    }

    updateWidgetFromAction(widget, action);

    if(validInLandscape && landscapeIndex == -1) {
        int index = policyIndexForAddingAction(action, landscapePolicy);
        if(index != -1)
            landscapePolicy->insertWidgetAndRemoveOverflow( index, widget );
    }
    if(validInPortrait && portraitIndex == -1) {
        int index = policyIndexForAddingAction(action, portraitPolicy);
        if(index != -1)
            portraitPolicy->insertWidgetAndRemoveOverflow( index, widget );
    }

    updateEmptinessProperty();
}

void MToolBarViewPrivate::updateWidgetFromAction(MWidget *widget, QAction *action) const
{
    widget->setEnabled(action->isEnabled() && itemsEnabled);

    MButton *button = qobject_cast<MButton *>(widget);
    if (button && buttons.contains(action)) {
        // Update button data accordingly only if we created it
        button->setCheckable(action->isCheckable());
        button->setChecked(action->isChecked());

        button->setText(action->text());
        button->setTextVisible(labelsEnabled);

        MAction *mAction = qobject_cast<MAction *>(action);
        if (mAction) {
            button->setIconID(mAction->iconID());
            button->setIconVisible(iconsEnabled || isIconOnly(button));
        }

        updateViewAndStyling(button);
    }
}

bool MToolBarViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
    switch (e->type()) {
        case QEvent::ActionRemoved:
            remove(static_cast<QActionEvent *>(e)->action(), false);
            break;
        case QEvent::ActionAdded:
            add(static_cast<QActionEvent *>(e)->action());
            break;
        case QEvent::ActionChanged:
            change(static_cast<QActionEvent *>(e)->action());
            break;

        case QEvent::DynamicPropertyChange:
            {
                QDynamicPropertyChangeEvent *propertyEvent = static_cast<QDynamicPropertyChangeEvent*>(e);
                if (propertyEvent->propertyName() == _M_IsEnabledPreservingSelection) {
                    bool enabledPreservingSelection = obj->property(_M_IsEnabledPreservingSelection).toBool();
                    setEnabledPreservingSelection(enabledPreservingSelection);
                } else if (propertyEvent->propertyName() == "widgetAlignment") {
                    updateWidgetAlignment();
                    updateCenterOffset();
                }
            }
            break;

        default:
            break;
    }
    return QObject::eventFilter(obj, e);
}

MWidget *MToolBarViewPrivate::createWidget(QAction *action)
{
    Q_Q(MToolBarView);
    // If widget is not already created then create it
    MWidget *widget = buttons.value(action);
    if (!widget)
        widget = leasedWidgets.value(action);
    if (!widget) {
        MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
        if (widgetAction) {
            widget = widgetAction->requestWidget(NULL);
            if (widget) {
                leasedWidgets.insert(action, widget);
                widget->show();
            }
        } else {
            MButton *button = removedActionsButtons.take(action);
            if (button) {
                button->show();
            } else {
                button = new MButton;
                button->setMinimumSize(0,0);
            }
            button->setObjectName("toolbaractioncommand");
            if (action && !action->objectName().isEmpty())
                button->setObjectName(button->objectName() + "_" + action->objectName());

            if (buttonGroup) {
                buttonGroup->addButton(button);
                connect(button, SIGNAL(toggled(bool)), q, SLOT(_q_groupButtonClicked(bool)));
                connect(action, SIGNAL(toggled(bool)), q, SLOT(_q_groupActionToggled(bool)));
            } else
                connect(button, SIGNAL(toggled(bool)), action, SLOT(setChecked(bool)));

            connect(button, SIGNAL(clicked(bool)), action, SIGNAL(triggered(bool)));

            buttons.insert(action, button);
            widget = button;
        }
    }

    return widget;
}
bool MToolBarViewPrivate::isLocationValid(QAction *action, MAction::Location loc) const
{
    MAction *mAction = qobject_cast<MAction *>(action);
    if(!mAction)
        return true; //A normal QAction is valid to place on toolbar
    return mAction->location().testFlag(loc);
}

bool MToolBarViewPrivate::releaseWidget(QAction *action, MWidget *widget) const
{
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        widgetAction->releaseWidget(widget);
    }
    return (widgetAction != 0);
}

bool MToolBarViewPrivate::hasTextEditWidget(QAction *action) const
{
    MTextEdit *textEditWidget = 0;
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if (widgetAction) {
        textEditWidget = qobject_cast<MTextEdit *>(widgetAction->widget());
    }
    return (textEditWidget != 0);
}

bool MToolBarViewPrivate::hasUnusableWidget(QAction *action) const
{
    if( buttons.contains(action) || leasedWidgets.contains(action) )
        return false; //If we are already using it, it must be usable
    MWidgetAction *widgetAction = qobject_cast<MWidgetAction *>(action);
    if(!widgetAction || !widgetAction->widget())
        return false; //We can create a button for it
    return widgetAction->isWidgetInUse();
}

MWidget *MToolBarView::getWidget(QAction *action) const
{
    Q_D(const MToolBarView);
    MWidget *button = d->buttons.value(action);
    if( button )
        return button;
    return d->leasedWidgets.value(action);
}

MWidget *MToolBarViewPrivate::getWidget(QAction *action) const
{
    return buttons.value(action);
}

void MToolBarViewPrivate::setEnabledPreservingSelection(bool enabled)
{
    // To ensure that buttons/widgets are restored correctly to their original state,
    // AND the state of their associated actions with the enabled parameter

    itemsEnabled = enabled;

    for (QHash<QAction *, MButton *>::const_iterator bit = buttons.constBegin(); bit != buttons.constEnd(); ++bit) {
        QAction *action = bit.key();
        MButton *button = bit.value();
        button->setEnabled(enabled && action->isEnabled());
    }

    for (QHash<QAction *, MWidget *>::const_iterator wit = leasedWidgets.constBegin(); wit != leasedWidgets.constEnd(); ++wit) {
        QAction *action = wit.key();
        MWidget *widget = wit.value();
        widget->setEnabled(enabled && action->isEnabled());
    }
}

void MToolBarViewPrivate::_q_groupButtonClicked(bool checked)
{
    Q_Q(MToolBarView);
    MButton *button = qobject_cast<MButton *>(q->sender());
    if(!button)
        return; //impossible?

    button->setChecked(checked);

    QAction *action = buttons.key(button);
    if(action->isChecked() != checked)
        action->setChecked(checked);
}

void MToolBarViewPrivate::_q_groupActionToggled(bool checked)
{
    Q_Q(MToolBarView);
    QAction* action = qobject_cast<QAction *>(q->sender());
    Q_ASSERT(action);
    MButton *button = buttons.value(action);
    if (button) {
        button->setChecked(checked);
    }
}

void MToolBarViewPrivate::setCapacity(int newCapacity)
{
    if (newCapacity == -1)
        newCapacity = MToolBarLayoutPolicy::unlimitedCapacity;

    setCapacity(newCapacity, portraitPolicy);
    setCapacity(newCapacity, landscapePolicy);
}

void MToolBarViewPrivate::setCapacity(int newCapacity, MToolBarLayoutPolicy *policy)
{
    if (policy->maxWidgetSlots() == newCapacity)
        return;

    bool newRoom = (policy->effectiveMaxWidgetSlots() < newCapacity && policy->freeWidgetSlots() == 0);

    policy->setMaxWidgetSlots(newCapacity);

    if (newRoom)
        addActionsFromLeftOvers();
}

void MToolBarViewPrivate::addActionsFromLeftOvers()
{
    foreach (QAction *action, controller->actions()) {
        if (action->isVisible() &&
            (isLocationValid(action, MAction::ToolBarLandscapeLocation) ||
             isLocationValid(action, MAction::ToolBarPortraitLocation)))
            change(action);
    }
}

void MToolBarViewPrivate::setIconsEnabled(bool enabled)
{
    iconsEnabled = enabled;

    for (QHash<QAction *, MButton *>::const_iterator bit = buttons.constBegin(); bit != buttons.constEnd(); ++bit) {
        MButton *button = bit.value();
        if (button)
            button->setIconVisible(iconsEnabled || isIconOnly(button));
    }
}

void MToolBarViewPrivate::setLabelsEnabled(bool enabled)
{
    labelsEnabled = enabled;

    for (QHash<QAction *, MButton *>::const_iterator bit = buttons.constBegin(); bit != buttons.constEnd(); ++bit) {
        MButton *button = bit.value();
        if (isLabelOnly(button))
            button->setTextVisible(true);
        else if (button)
            button->setTextVisible(labelsEnabled);
    }
}

void MToolBarViewPrivate::setSpacesEnabled(bool enabled)
{
    currentPolicy()->setSpacesBetween(enabled);
}

void MToolBarViewPrivate::updateViewAndStyling(MButton *button) const
{
    QString toolBarButtonDefaultViewType = buttonGroup ? "toolbartab" : "toolbar";

    if (isLabelOnly(button)) {
        // Only label -> could use different styling
        button->setTextVisible(true); //In this case we will show label (as it is all we have)
        if (labelOnlyAsCommonButton) {
            if (button->viewType() != MButton::defaultType)
                button->setViewType(MButton::defaultType);
            button->setStyleName(labelOnlyCommonButtonStyleName);
        } else {
            if (button->viewType() != toolBarButtonDefaultViewType)
                button->setViewType(toolBarButtonDefaultViewType);
            button->setStyleName(labelOnlyButtonStyleName);
        }
    } else {
        if (button->viewType() != toolBarButtonDefaultViewType)
            button->setViewType(toolBarButtonDefaultViewType);
        if (iconsEnabled && !labelsEnabled)
            button->setStyleName(iconButtonStyleName);
        else
            button->setStyleName(iconLabelButtonStyleName);
        button->setTextVisible(labelsEnabled);
    }
}

void MToolBarViewPrivate::updateStyleNames()
{
    Q_Q(MToolBarView);

    const MToolbarStyle *s = static_cast<const MToolbarStyle *>(q->style().operator ->());

    labelOnlyCommonButtonStyleName = s->labelOnlyCommonButtonStyleName();
    labelOnlyButtonStyleName = s->labelOnlyButtonStyleName();
    iconButtonStyleName = s->iconButtonStyleName();
    iconLabelButtonStyleName = s->iconLabelButtonStyleName();

    for (QHash<QAction *, MButton *>::const_iterator bit = buttons.constBegin(); bit != buttons.constEnd(); ++bit) {
        MButton *button = bit.value();
        if (button) {
            if (isLabelOnly(button)) {
                if (labelOnlyAsCommonButton)
                    button->setStyleName(labelOnlyCommonButtonStyleName);
                else
                    button->setStyleName(labelOnlyButtonStyleName);
            } else if (iconsEnabled && !labelsEnabled)
                button->setStyleName(iconButtonStyleName);
            else
                button->setStyleName(iconLabelButtonStyleName);
        }
    }
}

void MToolBarViewPrivate::updateWidgetAlignment()
{
    QVariant v = controller->property("widgetAlignment");
    widgetAlignment = v.isValid() ? static_cast<Qt::AlignmentFlag>(v.toInt()) : Qt::AlignHCenter;

    /* In here the Qt::Alignment is used like this:
       AlignLeft = no prereserved slots on left
       AlignRight = no prereserved slots on right
       AlignJustify = no prereserved slots on left and right
       AlignHCenter = prereserved slots on left and right */

    bool alignmentAffectsCapacity = (v.isValid() && !buttonGroup);
    landscapePolicy->setWidgetAlignment(widgetAlignment, alignmentAffectsCapacity);
    portraitPolicy->setWidgetAlignment(widgetAlignment, alignmentAffectsCapacity);

    if (widgetAlignment != Qt::AlignHCenter || !v.isValid())
        addActionsFromLeftOvers();

    updateAlignmentMargins(alignmentMargins);
}

void MToolBarViewPrivate::updateCenterOffset(const QSizeF &size)
{
    qreal offset = 0.0f;

    if (widgetAlignment == Qt::AlignLeft || widgetAlignment == Qt::AlignRight) {
        if (size.isValid()) {
            if (controller->sceneManager()) {
                qreal widthDiff = controller->sceneManager()->visibleSceneSize(controller->sceneManager()->orientation()).width() - size.width();
                offset = qAbs(widthDiff/2);
            }
        } else
            offset = qAbs(centerOffset);
        if (widgetAlignment == Qt::AlignLeft)
            offset = -offset;
        centerOffset = offset;
    }

    currentPolicy()->setCenterOffset(offset);
}

void MToolBarViewPrivate::setCentering(bool allToParent)
{
    currentPolicy()->setCentering(false, allToParent);
}

void MToolBarViewPrivate::updateAlignmentMargins(int alignmentMargins)
{
    this->alignmentMargins = alignmentMargins;

    bool isLandscapeButtonGroup = (buttonGroup && controller->sceneManager() && controller->sceneManager()->orientation() == M::Landscape);

    switch (widgetAlignment) {
    case Qt::AlignLeft:
        layout->setContentsMargins(isLandscapeButtonGroup ? 0 : alignmentMargins, 0, 0, 0);
        break;
    case Qt::AlignRight:
        layout->setContentsMargins(0, 0, isLandscapeButtonGroup ? 0 : alignmentMargins, 0);
        break;
    case Qt::AlignJustify:
        layout->setContentsMargins(alignmentMargins, 0, alignmentMargins, 0);
        break;
    default:
        layout->setContentsMargins(0, 0, 0, 0);
        break;
    }
}

void MToolBarViewPrivate::updateEmptinessProperty()
{
    controller->setProperty("isEmpty", currentPolicy()->widgetCount() == 0);
}

void MToolBarViewPrivate::setLabelOnlyAsCommonButton(bool enable, bool centerToParent)
{
    labelOnlyAsCommonButton = enable;

    for (QHash<QAction *, MButton *>::const_iterator bit = buttons.constBegin(); bit != buttons.constEnd(); ++bit) {
        MButton *button = bit.value();
        if (isLabelOnly(button))
            updateViewAndStyling(button);
    }

    currentPolicy()->setLabelOnlyButtonCentering(labelOnlyAsCommonButton, centerToParent);
}

bool MToolBarViewPrivate::isLabelOnly(MButton *button) const
{
    return (button && !button->text().isEmpty() && button->iconID().isEmpty());
}

bool MToolBarViewPrivate::isIconOnly(MButton *button) const
{
    return (button && button->text().isEmpty() && !button->iconID().isEmpty());
}

MToolBarLayoutPolicy *MToolBarViewPrivate::currentPolicy() const
{
    MToolBarLayoutPolicy* currentPolicy;

    // MLayout may not have orientationChanged processed yet, so try to take orientation from MSceneManager
    if (controller->sceneManager())
        currentPolicy = controller->sceneManager()->orientation() == M::Landscape ? landscapePolicy : portraitPolicy;
    else
        currentPolicy = static_cast<MToolBarLayoutPolicy*>(layout->policy());
    return currentPolicy;
}

void MToolBarViewPrivate::refreshDisabledWidgets() const
{
    // Qt does not send events to disabled widgets,
    // so in this case the QEvent::StyleChange does not
    // reach its receivers.
    // To work around this problem we send the event
    // by hand here.

    if (!itemsEnabled) {
        QEvent event(QEvent::StyleChange);

        int count = currentPolicy()->widgetCount();
        for (int i=0; i<count; i++) {
            MWidgetController *wc = dynamic_cast<MWidgetController *>(currentPolicy()->widgetAt(i));
            if (wc && !wc->isEnabled())
                QApplication::sendEvent(wc, &event);
        }
    }
}


MToolBarView::MToolBarView(MToolBar *controller) :
    MWidgetView(controller),
    d_ptr(new MToolBarViewPrivate(controller))
{
    Q_D(MToolBarView);
    d->q_ptr = this;
    d->init();
}

MToolBarView::MToolBarView(MToolBarViewPrivate &dd, MToolBar *controller) :
    MWidgetView(controller),
    d_ptr(&dd)
{
    Q_D(MToolBarView);
    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MToolBarView::~MToolBarView()
{
    delete d_ptr;
}

QRectF MToolBarView::boundingRect() const
{
    QRectF br = MWidgetView::boundingRect();
    if (style()->dropShadowImage()) {
        br.setTop(-style()->dropShadowImage()->pixmap()->size().height());
    }
    return br;
}

void MToolBarView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MToolBarView);

    //draw shadow on top of the actual toolbar on portrait mode
    if (d->controller->sceneManager() && d->controller->sceneManager()->orientation() == M::Portrait) {
        const MToolbarStyle *s = static_cast<const MToolbarStyle *>(style().operator ->());
        if (s->dropShadowImage()) {
            s->dropShadowImage()->draw(0.0,
                    (qreal)-s->dropShadowImage()->pixmap()->size().height(),
                    boundingRect().width(),
                    (qreal)s->dropShadowImage()->pixmap()->size().height(),
                    painter);
        }
    }
    MWidgetView::drawBackground(painter, option);
}

void MToolBarView::applyStyle()
{
    Q_D(MToolBarView);

    MWidgetView::applyStyle();

    const MToolbarStyle *s = static_cast<const MToolbarStyle *>(style().operator ->());

    d->setSpacesEnabled(s->hasSpaces());
    d->setIconsEnabled(s->hasIcons());
    d->setLabelsEnabled(s->hasLabels());
    d->setLabelOnlyAsCommonButton(s->labelOnlyAsCommonButton(), s->centerCommonButtonsToParent());
    d->setCapacity(s->capacity());
    d->updateStyleNames();
    d->updateAlignmentMargins(s->alignmentMargins());
    d->setCentering(s->centerToParent());
    d->updateEmptinessProperty();
}

void MToolBarView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MToolBarView);

    MWidgetView::resizeEvent(event);

    d->updateCenterOffset(event->newSize());
}

void MToolBarView::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_D(MToolBarView);

    MWidgetView::orientationChangeEvent(event);

    d->refreshDisabledWidgets();
}

// bind view and controller together
M_REGISTER_VIEW_NEW(MToolBarView, MToolBar)

#include "moc_mtoolbarview.cpp"
