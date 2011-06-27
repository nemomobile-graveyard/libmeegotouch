/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSBARSLIDEANIMATION_H
#define MSTATUSBARSLIDEANIMATION_H

#include <mabstractwidgetanimation.h>
#include <mstatusbarslideanimationstyle.h>

//! \internal

#include <QPointF>
#include <QPropertyAnimation>
#include <QList>
#include <QWeakPointer>
#include <QGraphicsWidget>

class MSceneWindow;

/*!
  \class MStatusBarSlideAnimation
  \brief Slides an MStatusBar up and down and repositions/resizes other items accordingly

  It slides an MStatusBar up (off the screen) and down (onto the screen) and at the
  same time resizes and dislocates other items to fit within the remaining space
  (i.e. screen rect - status bar rect).

  It gives the effect that when a status bar slides down it squeezes the items below it
  to fit in the smaller remaining space and that when the status bar slides up the items
  below it expand to occupy the entire screen. As if they belonged to a vertical linear
  layout.
 */
class MStatusBarSlideAnimation : public MAbstractWidgetAnimation
{
    Q_OBJECT
    M_ANIMATION_GROUP(MStatusBarSlideAnimationStyle)

public:

    /*!
      \brief Constructs the widget animation.
     */
    MStatusBarSlideAnimation(QObject *parent = NULL);

    /*!
      \brief Destructs the widget animation.
     */
    virtual ~MStatusBarSlideAnimation();

    void setTransitionDirection(TransitionDirection direction);

    //! \reimp
    virtual void restoreTargetWidgetState();

    //! \brief The status bar
    virtual void setTargetWidget(MWidgetController *statusBar);
    //! \reimp_end

    void setRootElementsDisplacedByStatusBar(QList<QGraphicsWidget*> &elementsList);

    /*!
        List of all scene windows whose state != Disappeared.
        Ordered by Z-value, from lowest to highest.
     */
    void setSceneWindowStack(const QList<MSceneWindow *> *list);

    void setSceneManager(MSceneManager *sceneManager);

protected:
    //! \reimp
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    //! \reimp_end

private:
    void setupStatusBarAnimation();
    void setupRootElementsAnimations();
    void destroyRootElementsAnimations();
    void createRootElementsAnimations();
    void destroySceneWindowsAnimations();
    void createAndSetupSceneWindowsAnimations();
    bool played;
    TransitionDirection direction;
    QPropertyAnimation *positionAnimation;
    QPointF originalPos;
    QList< QWeakPointer<QGraphicsWidget> > rootElementsDisplacedByStatusBar;
    QList<QPropertyAnimation *> rootElementsAnimations;
    const QList<MSceneWindow *> *sceneWindowStack;
    QList<QPropertyAnimation *> sceneWindowsAnimations;
    MSceneManager *sceneManager;
};
//! \internal_end

#endif
