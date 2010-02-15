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

#ifndef DUIWIDGETACTION_H
#define DUIWIDGETACTION_H

#include "duiaction.h"

class DuiWidget;
class DuiWidgetActionPrivate;

/**
    \class DuiWidgetAction
    \brief The DuiWidgetAction class extends DuiAction by an interface
    for inserting custom widgets into action based containers, such as DuiToolBar.

    A custom widget can be set using setWidget() and that widget will then be used
    if the action is added to a DuiToolBar, or in general to an action container that
    supports DuiWidgetAction. If a DuiWidgetAction is added to two toolbars (e.g.) at the
    same time then the widget is shown  only in the first toolbar the action was added
    to. DuiWidgetAction takes  over ownership of the widget.

    Note that it is up to the widget to activate the action, for example by
    reimplementing mouse event handlers and calling DuiAction::trigger().

    \sa DuiAction
*/

class DUI_EXPORT DuiWidgetAction : public DuiAction
{
    Q_OBJECT

public:
    /**
       \brief Default constructor
       \param parent  Pointer to parent object
     */
    explicit DuiWidgetAction(QObject *parent);

    /**
       \brief Destructor
     */
    virtual ~DuiWidgetAction();

    /**
       \brief  Sets \a widget to the action. The ownership is
       transferred to DuiWidgetAction.
     */
    void setWidget(DuiWidget *w);

    /**
       \brief Returns the widget attached.
     */
    DuiWidget *widget() const;

    /**
       \brief Returns a widget that represents the action, with the given \a parent.

        Container widgets that support actions can call this function to
        request a widget as visual representation of the action.
        \sa releaseWidget(), widget()
     */
    DuiWidget *requestWidget(DuiWidget *parent);

    /**
       \brief Releases the specified \a widget.

        Container widgets that support actions call this function when a widget
        action is removed.
        \sa requestWidget(), widget()
     */
    void releaseWidget(DuiWidget *widget);

    /*!
       \brief Returns the status whether the default widget is in use or not
     */
    bool isWidgetInUse() const;

protected:

    //! \reimp
    virtual bool event(QEvent *);
    virtual bool eventFilter(QObject *, QEvent *);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiWidgetAction)
    Q_DISABLE_COPY(DuiWidgetAction)

#ifdef UNIT_TEST
    friend class Ut_DuiWidgetAction;
#endif
};

#endif // DUIWIDGETACTION_H
