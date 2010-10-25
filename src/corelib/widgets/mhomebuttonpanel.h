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

#ifndef MHOMEBUTTONPANEL_H
#define MHOMEBUTTONPANEL_H

#include "mscenewindow.h"
#include "mscenewindowmodel.h"

/*!
 * \class MHomeButtonPanel
 * \brief The MHomeButtonPanel class displays a home button on top of all GUI.
 *
 * The home button is always visible and accessible, standing on top of all
 * application GUI, including MOverlay instances and modal dialogs.
 */
class M_CORE_EXPORT MHomeButtonPanel : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

public:

    /*!
     * \brief Constructs a widget with the given \a parent.
     */
    MHomeButtonPanel(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~MHomeButtonPanel();

Q_SIGNALS:
    /*!
     * \brief Emitted when the home button was clicked.
     */
    void buttonClicked();

protected:
    MHomeButtonPanel(MSceneWindowPrivate *dd, MSceneWindowModel *model,
                       MSceneWindow::WindowType windowType,
                       const QString &viewType = QString(""), QGraphicsItem *parent = 0);

private:

    Q_DISABLE_COPY(MHomeButtonPanel)
};

#endif
