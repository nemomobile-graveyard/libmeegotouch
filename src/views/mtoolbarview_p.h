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

#ifndef MTOOLBARVIEW_P_H
#define MTOOLBARVIEW_P_H

#include <QObject>
#include <QHash>
#include <QCache>
#include <QList>

#include "maction.h"
#include "mnamespace.h"

class QGraphicsLayoutItem;
class QGraphicsWidget;
class QEvent;
class QAction;
class MToolBar;
class MWidget;
class MLinearLayoutPolicy;
class MToolBarView;
class MButton;
class MWidgetAction;
class MLayout;
class MToolBarLayoutPolicy;
class MButtonGroup;
class MAction;

//! \internal
class MToolBarViewPrivate : public QObject
{
    Q_DECLARE_PUBLIC(MToolBarView)

public:
    MToolBarViewPrivate(MToolBar *controller);
    virtual ~MToolBarViewPrivate();

    void init();

    void add(QAction *action);
    void remove(QAction *action, bool hideOnly);
    void change(QAction *action);

    void setCapacity(int newCapacity);
    void setIconsEnabled(bool enabled);
    void setLabelsEnabled(bool enabled);
    void setSpacesEnabled(bool enabled);
    void setLabelOnlyAsCommonButton(bool enable, bool centerToParent);
    void setCentering(bool allToParent);
    void refreshDisabledWidgets() const;

    QCache<QAction *, MButton> removedActionsButtons;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    MWidget *createWidget(QAction *action);

    int policyIndexForAddingAction(QAction *action, MToolBarLayoutPolicy *policy) const;
    void insert(MToolBarLayoutPolicy *policy, int index, QAction *action, MWidget *widget);
    bool isLocationValid(QAction *action, MAction::Location loc) const;
    bool releaseWidget(QAction *action, MWidget *widget) const;
    bool hasTextEditWidget(QAction *action) const;
    bool hasUnusableWidget(QAction *action) const;
    void updateWidgetFromAction(MWidget *widget, QAction *action) const;
    MWidget *getWidget(QAction *action) const;
    void setEnabledPreservingSelection(bool enabled);
    void _q_groupButtonClicked(bool);
    void _q_groupActionToggled(bool);
    void setCapacity(int newCapacity, MToolBarLayoutPolicy *policy);
    void addActionsFromLeftOvers();
    void updateViewAndStyling(MButton *button) const;
    virtual void updateWidgetAlignment();
    void updateCenterOffset(const QSizeF &size = QSizeF());
    void updateEmptinessProperty();
    void updateAlignmentMargins(int alignmentMargins);
    void updateStyleNames();
    bool isLabelOnly(MButton *button) const;
    bool isIconOnly(MButton *button) const;
    MToolBarLayoutPolicy *currentPolicy() const;

protected:
    MToolBarView *q_ptr;
    MToolBar *controller;
    MLayout *layout;
    MToolBarLayoutPolicy *landscapePolicy;
    MToolBarLayoutPolicy *portraitPolicy;
    QHash<QAction *, MWidget *> leasedWidgets;
    QHash<QAction *, MButton *> buttons;
    MButtonGroup * buttonGroup; /* If this is non-null, created buttons will be placed in this group */
    bool iconsEnabled;
    bool labelsEnabled;
    bool labelOnlyAsCommonButton;
    Qt::AlignmentFlag widgetAlignment;
    bool itemsEnabled;
    int alignmentMargins;
    qreal centerOffset;
    QString labelOnlyCommonButtonStyleName;
    QString labelOnlyButtonStyleName;
    QString iconButtonStyleName;
    QString iconLabelButtonStyleName;
};
//! \internal_end

#endif
