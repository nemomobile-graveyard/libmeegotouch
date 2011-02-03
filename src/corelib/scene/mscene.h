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

#ifndef MSCENE_H
#define MSCENE_H

#include "mexport.h"
#include "mnamespace.h"

#include <QGraphicsScene>

class MSceneManager;
class MScenePrivate;
class MWindowPrivate;

/*!
 \class MScene
 \brief MScene is the scene graph of MeeGo Touch UI applications

 The scene of an application is a virtual infinite surface on which primitive items or
 full featured widgets can be placed. For general information of how the scene functions,
 please refer to the QGraphicsScene documentation.

 MScene inherits from and extends QGraphicsScene with the following features:

 - Automatic scene management. MSceneWindow derived classes such as MApplicationPage or MDialog
   become managed by a MSceneManager instance when they appear in a MSceene. The feature is comparable
   to how native windows are managed by a window manager in desktop environments.
 - Emulation of multitouch events for single-touch enviroments, as in a development environment.
 - Calcuation of the widget on-display property, see MWidget::isOnDisplay()
 - Development mode visual overlays, providing information to help debug applications:
    - Bounding rectangles, toggled by <Ctrl>-<Shift>-B
    - Margins, toggled by <Ctrl>-<Shift>-M
    - Positions, toggled by <Ctrl>-<Shift>-P
    - Sizes, toggled by <Ctrl>-<Shift>-S
    - Object names, toggled by <Ctrl>-<Shift>-N
    - Frames per second, toggled by <Ctrl>-<Shift>-F
    - Take screenshot, triggered by <Ctrl>-<Shift>-T

  \sa MWindow, MApplicationWindow
 */
class M_CORE_EXPORT MScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /*!
     Constructs a MScene.
     */
    explicit MScene(QObject *parent = 0);

    /*!
     Destroys the MScene.
     */
    virtual ~MScene();

    /*!
     Returns the scene manager of the scene.

     Note that the scene manager is not owned by the scene,
     instead it's the scene that is owned by the scene manager.

     If no manager has been assigned, 0 is returned.
     */
    MSceneManager *sceneManager();

protected:
    //! \reimp
    bool event(QEvent *event);    
    void drawForeground(QPainter *painter, const QRectF &rect);
    //! \reimp_end

    //! \internal
    MScenePrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(MScene)
    Q_DECLARE_PRIVATE(MScene)

#ifdef UNIT_TEST
    friend class Ut_MScene;
#endif // UNIT_TEST

    friend class MSceneManager;
    friend class MWindowPrivate;
    friend class MSceneWindow;
    friend class MWidget;
};

#endif
