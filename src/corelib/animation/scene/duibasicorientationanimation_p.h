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

#ifndef DUIBASICORIENTATIONANIMATION_P_H
#define DUIBASICORIENTATIONANIMATION_P_H

#include "duiparallelanimationgroup_p.h"

#include <QHash>

class QParallelAnimationGroup;
class QPropertyAnimation;
class DuiBasicOrientationAnimation;
class DuiSceneWindowAnimation;
class DuiSceneWindow;

class DuiBasicOrientationAnimationPrivate : public DuiParallelAnimationGroupPrivate
{
    Q_DECLARE_PUBLIC(DuiBasicOrientationAnimation)

public:
    void _q_onPhase0Finished();
    void _q_onPhase1Finished();
    void _q_onPhase2Finished();

    void hideComponents();
    void showComponents();

    void setupNavigationBarAnimations();
    void setupHomeButtonAnimations();
    void setupEscapeButtonAnimations();
    void setupNavigationButtonAnimations(DuiSceneWindow *button,
                                         QPropertyAnimation *slideInAnimation,
                                         QPropertyAnimation *slideOutAnimation);
    void setupDockWidgetAnimations();

    void addGenericSceneWindowAnimations(DuiSceneWindow *sceneWindow);
    void removeGenericSceneWindowAnimations(DuiSceneWindow *sceneWindow);

    void addApplicationPageAnimations(DuiSceneWindow *applicationPage);
    void removeApplicationPageAnimations(DuiSceneWindow *applicationPage);

    DuiSceneWindow *fetchLayerEffect(DuiSceneWindow *sceneWindow);

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

    DuiSceneWindow *navigationBar;
    DuiSceneWindow *homeButtonPanel;
    DuiSceneWindow *escapeButtonPanel;
    DuiSceneWindow *dockWidget;

    QSize landscapeScreenSize;

    Dui::OrientationAngle startOrientationAngle;
    Dui::OrientationAngle endOrientationAngle;

    class SceneWindowAnimationPair
    {
    public:
        QPropertyAnimation *fadeOutAnimation;
        QPropertyAnimation *fadeInAnimation;
    };

    QHash<DuiSceneWindow *, SceneWindowAnimationPair> genericAnimationsHash;

    class QAnimationGroup *sequentialPhasesAnimation;
    QGraphicsWidget *rootElement;
};

#endif
