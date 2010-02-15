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

#ifndef DUIGROUPANIMATION_H
#define DUIGROUPANIMATION_H

#include <duianimation.h>

#include <duigroupanimationstyle.h>

class DuiGroupAnimationPrivate;

/*! \brief DuiGroupAnimation provides an base class for groups of animations.
 */
class DUI_EXPORT DuiGroupAnimation : public DuiAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiGroupAnimation)
    DUI_ANIMATION(DuiGroupAnimationStyle)

public:
    enum Type {
        Parallel,
        Sequential
    };

protected:
    /*!
      \brief Constructs the animation with parallel animation group inside.

      This constructor is meant to be used inside the libdui to share the
      private data class pointer.
     */
    DuiGroupAnimation(DuiGroupAnimationPrivate *dd, DuiGroupAnimation::Type type = Parallel, QObject *parent = 0);

public:
    /*!
      \brief Constructs the animation with paraller animation group inside.
     */
    DuiGroupAnimation(DuiGroupAnimation::Type type = Parallel, QObject *parent = NULL);

    /*!
      \brief Destructs the animation and all subanimations.
     */
    virtual ~DuiGroupAnimation();

    //! \reimp
    // from QtAbstractAnimation
    virtual int duration() const;
    //! \reimp_end

protected:
    /*!
      \brief Getter for the main animation group.

      Derived classes can use this to populate the animation group hierarchy.
     */
    QAnimationGroup *group();

    /*!
      \brief Const getter for the main animation group.
     */
    const QAnimationGroup *group() const;

    //! \reimp
    // from QtAbstractAnimation
    virtual void updateCurrentTime(int msecs);
    virtual void updateState(QAbstractAnimation::State oldState, QAbstractAnimation::State newState);
    virtual void updateDirection(QAbstractAnimation::Direction direction);
    //! \reimp_end
};

#endif
