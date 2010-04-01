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

#ifndef DUIMODALSCENEWINDOW_H
#define DUIMODALSCENEWINDOW_H

#include "duiscenewindow.h"
#include "duimodalscenewindowmodel.h"

class DuiModalSceneWindowPrivate;

/*!
 * \class DuiModalSceneWindow
 * \brief The DuiModalSceneWindow class provides an empty scene window
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
class DUI_EXPORT DuiModalSceneWindow : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiModalSceneWindow)

public:

    /*!
     * \brief Constructs a modalscenewindow.
     */
    DuiModalSceneWindow();

protected:
    DuiModalSceneWindow(DuiModalSceneWindowPrivate *dd, DuiModalSceneWindowModel *model, DuiSceneWindow::WindowType windowType);

public:

    /*!
     * \brief Destructor for modalscenewindow class
     */
    virtual ~DuiModalSceneWindow();

private:
    Q_DECLARE_PRIVATE(DuiModalSceneWindow)
    Q_DISABLE_COPY(DuiModalSceneWindow)

    friend class DuiModalSceneWindowView;
    friend class DuiModalSceneWindowViewPrivate;
};

#endif
