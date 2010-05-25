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

#ifndef MBASICORIENTATIONANIMATION_P_H
#define MBASICORIENTATIONANIMATION_P_H

#include "morientationanimation_p.h"

#include <QHash>

class QParallelAnimationGroup;
class QPropertyAnimation;
class MBasicOrientationAnimation;
class MSceneWindow;

class MBasicOrientationAnimationPrivate : public MOrientationAnimationPrivate
{
    Q_DECLARE_PUBLIC(MBasicOrientationAnimation)

public:
    void _q_onPhase0Finished();
    void _q_onPhase1Finished();
    void _q_onPhase2Finished();

    void hideComponents();
    void showComponents();

    void setupNavigationBarAnimations();
    void setupHomeButtonAnimations();
    void setupEscapeButtonAnimations();
    void setupNavigationButtonAnimations(MSceneWindow *button,
                                         QPropertyAnimation *slideInAnimation,
                                         QPropertyAnimation *slideOutAnimation);
    void setupDockWidgetAnimations();

    void addGenericSceneWindowAnimations(MSceneWindow *sceneWindow);
    void removeGenericSceneWindowAnimations(MSceneWindow *sceneWindow);

    void addApplicationPageAnimations(MSceneWindow *applicationPage);
    void removeApplicationPageAnimations(MSceneWindow *applicationPage);

    MSceneWindow *fetchLayerEffect(MSceneWindow *sceneWindow);

    QParallelAnimationGroup   *phase0;
    QParallelAnimationGroup   *phase1;
    QParallelAnimationGroup   *phase2;
    QParallelAnimationGroup   *upcomingPhase;

    QPropertyAnimation        *rotationAnimation;
    QPropertyAnimation        *positionAnimation;
    QPropertyAnimation        *originAnimation;
    QPropertyAnimation        *navigationBarSlideInAnimation;
    QPropertyAnimation        *navigationBarSlideOutAnimation;

    QPropertyAnimation        *homeButtonSlideInAnimation;
    QPropertyAnimation        *homeButtonSlideOutAnimation;

    QPropertyAnimation        *escapeButtonSlideInAnimation;
    QPropertyAnimation        *escapeButtonSlideOutAnimation;

    QPropertyAnimation        *dockWidgetSlideInAnimation;
    QPropertyAnimation        *dockWidgetSlideOutAnimation;

    MSceneWindow *navigationBar;
    MSceneWindow *homeButtonPanel;
    MSceneWindow *escapeButtonPanel;
    MSceneWindow *dockWidget;

    QSize landscapeScreenSize;

    M::OrientationAngle startOrientationAngle;
    M::OrientationAngle endOrientationAngle;

    class SceneWindowAnimationPair
    {
    public:
        QPropertyAnimation *fadeOutAnimation;
        QPropertyAnimation *fadeInAnimation;
    };

    QHash<MSceneWindow *, SceneWindowAnimationPair> genericAnimationsHash;

    class QAnimationGroup *sequentialPhasesAnimation;
};

#endif
