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

#ifndef DUIHOMEBUTTONPANEL_H
#define DUIHOMEBUTTONPANEL_H

#include "duiscenewindow.h"
#include "duiscenewindowmodel.h"

/*!
 * \class DuiHomeButtonPanel
 * \brief The DuiHomeButtonPanel class displays a home button on top of all GUI.
 *
 * The home button is always visible and accessible, standing on top of all
 * application GUI, including DuiOverlay instances and modal dialogs.
 */
class DUI_EXPORT DuiHomeButtonPanel : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSceneWindow)

public:

    /*!
     * \brief Constructs a widget with the given \a parent.
     */
    DuiHomeButtonPanel(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~DuiHomeButtonPanel();

Q_SIGNALS:
    /*!
     * \brief Emitted when the home button was clicked.
     */
    void buttonClicked();

protected:
    DuiHomeButtonPanel(DuiSceneWindowPrivate *dd, DuiSceneWindowModel *model,
                       DuiSceneWindow::WindowType windowType,
                       const QString &viewType = QString(""), QGraphicsItem *parent = 0);

private:

    Q_DISABLE_COPY(DuiHomeButtonPanel)
};

#endif
