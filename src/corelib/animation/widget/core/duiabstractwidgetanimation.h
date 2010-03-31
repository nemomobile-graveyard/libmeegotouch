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

#ifndef DUIABSTRACTWIDGETANIMATION_H
#define DUIABSTRACTWIDGETANIMATION_H

#include <duiabstractwidgetanimationstyle.h>
#include <duiparallelanimationgroup.h>

class DuiAbstractWidgetAnimationPrivate;

/*!
  \class DuiAbstractWidgetAnimation
  \brief DuiAbstractWidgetAnimation class is a base class for all widget animations.

 */
class DUI_EXPORT DuiAbstractWidgetAnimation : public DuiParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiAbstractWidgetAnimation)
    DUI_ANIMATION_GROUP(DuiAbstractWidgetAnimationStyle)

protected:
    /*!
        \brief Constructs the widget animation.

        This constructor is meant to be used inside the libdui to share the
        private data class pointer.
     */
    DuiAbstractWidgetAnimation(DuiAbstractWidgetAnimationPrivate *dd, QObject *parent);

public:

    /*!
        \brief Constructs the widget animation.
     */
    DuiAbstractWidgetAnimation(QObject *parent = NULL);

    /*!
        \brief Destructs the widget animation.
     */
    virtual ~DuiAbstractWidgetAnimation();

    /*!
        Restores the properties of the target widget back to their
        original state, before the animation changed them.
     */
    virtual void restoreTargetWidgetState() = 0;

    virtual void setTargetWidget(DuiWidgetController *widget);

    DuiWidgetController *targetWidget();
    const DuiWidgetController *targetWidget() const;
};

#endif
