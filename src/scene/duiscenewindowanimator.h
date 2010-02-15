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

// Make doxygen skip this internal class
//! \cond
#ifndef DUISCENEWINDOWANIMATOR_H
#define DUISCENEWINDOWANIMATOR_H

#include <QObject>

class DuiSceneWindow;
class DuiSceneLayerEffect;
class DuiSceneWindowAnimatorPrivate;

class DuiSceneWindowAnimator : public QObject
{
    Q_OBJECT

public:

    DuiSceneWindowAnimator(DuiSceneWindow *window, DuiSceneLayerEffect *effect);
    virtual ~DuiSceneWindowAnimator();

    void updateStyle();

    /*!
     * Set the SceneWindow pointer
     */
    void setWindow(DuiSceneWindow *window);

    DuiSceneWindow *window() const;

    DuiSceneLayerEffect *effect() const;

    void forward();
    void backward();

    void forwardNow();
    void backwardNow();

Q_SIGNALS:
    void windowAnimationCompleted(DuiSceneWindowAnimator *, bool visible);

protected:
    virtual void styleUpdated();

    DuiSceneWindowAnimatorPrivate *const d_ptr;


private Q_SLOTS:
    void windowAnimationDone();
    void windowAnimating(qreal);

private:
    Q_DECLARE_PRIVATE(DuiSceneWindowAnimator)
};

#endif
//! \endcond

