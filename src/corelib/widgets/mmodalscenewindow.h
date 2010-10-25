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

#ifndef MMODALSCENEWINDOW_H
#define MMODALSCENEWINDOW_H

#include "mscenewindow.h"
#include "mmodalscenewindowmodel.h"

class MModalSceneWindowPrivate;

/*!
 * \class MModalSceneWindow
 * \brief The MModalSceneWindow class provides an empty scene window
 * with modality and layer effect
 *
 * A modalscenewindow is a top-level scene window which can be used to
 * create custom, modal scene windows. It has modality and a layer
 * effect which visually blocks the scene windows with lower z
 * value. It shares the z value with dialogs.
 *
 * A typical way of using modalscenewindow is to style its size,
 * alignment and offset, instantiate it, set a layout to it and
 * populate the layout with any components the use case requires.
 */
class M_CORE_EXPORT MModalSceneWindow : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MModalSceneWindow)

public:

    /*!
     * \brief Constructs a modalscenewindow.
     */
    MModalSceneWindow();

protected:
    MModalSceneWindow(MModalSceneWindowPrivate *dd, MModalSceneWindowModel *model, MSceneWindow::WindowType windowType);

public:

    /*!
     * \brief Destructor for modalscenewindow class
     */
    virtual ~MModalSceneWindow();

private:
    Q_DECLARE_PRIVATE(MModalSceneWindow)
    Q_DISABLE_COPY(MModalSceneWindow)

    friend class MModalSceneWindowView;
    friend class MModalSceneWindowViewPrivate;
};

#endif
