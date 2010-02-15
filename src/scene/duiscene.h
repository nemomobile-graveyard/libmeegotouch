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

//! DUISCene stub
class DUI_EXPORT DuiScene : public QGraphicsScene
{
    Q_OBJECT

public:
    //! DuiScene constructor
    /*!
     * \param parent QObject* defaults to 0
     * \return DuiScene
     * */
    explicit DuiScene(QObject *parent = 0);
    virtual ~DuiScene();

    /*!
     * Returns a pointer to the scene manager that manages this scene.
     * If no manager is assigned, a null pointer is returned.
     */
    DuiSceneManager *sceneManager();

protected:
    //! \reimp
    void drawForeground(QPainter *painter, const QRectF &rect);
    //! \reimp_end

    DuiScenePrivate *const d_ptr;

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
