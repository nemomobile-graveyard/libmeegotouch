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

#ifndef DUISCENE_H
#define DUISCENE_H

#include "duiexport.h"
#include "duinamespace.h"

#include <QGraphicsScene>

class DuiSceneManager;
class DuiScenePrivate;
class DuiWindowPrivate;

/*!
 \class DuiScene
 \brief DuiScene is the scene graph of DirectUI applications

 The scene of an application is a virtual infinite surface on which primitive items or
 full featured widgets can be placed. For general information of how the scene functions,
 please refer to the QGraphicsScene documentation.

 DuiScene inherits from and extends QGraphicsScene with the following features:

 - Automatic scene management. DuiSceneWindow derived classes such as DuiApplicationPage or DuiDialog
   become managed by a DuiSceneManager instance when they appear in a DuiSceene. The feature is comparable
   to how native windows are managed by a window manager in desktop environments.
 - Emulation of multitouch events for single-touch enviroments, as in a development environment.
 - Calcuation of the widget on-display property, see DuiWidget::isOnDisplay()
 - Development mode visual overlays, providing information to help debug applications:
    - Bounding rectangles, toggled by <Ctrl>-B
    - Margins, toggled by <Ctrl>-M
    - Positions, toggled by <Ctrl>-P
    - Sizes, toggled by <Ctrl>-S
    - Object names, toggled by <Ctrl>-N
    - Frames per second, toggled by <Ctrl>-F

  \sa DuiWindow, DuiApplicationWindow
 */
class DUI_EXPORT DuiScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /*!
     Constructs a DuiScene.
     */
    explicit DuiScene(QObject *parent = 0);

    /*!
     Destroys the DuiScene.
     */
    virtual ~DuiScene();

    /*!
     Returns the scene manager of the scene.

     Note that the scene is not owned by the manager, instead the manager owns the scene.

     If no manager is assigned, 0 is returned.
     */
    DuiSceneManager *sceneManager();

protected:
    //! \reimp
    bool event(QEvent *event);    
    void drawForeground(QPainter *painter, const QRectF &rect);
    //! \reimp_end

    //! \internal
    DuiScenePrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(DuiScene)
    Q_DECLARE_PRIVATE(DuiScene)

#ifdef UNIT_TEST
    friend class Ut_DuiScene;
#endif // UNIT_TEST

    friend class DuiSceneManager;
    friend class DuiWindowPrivate;
};

#endif
