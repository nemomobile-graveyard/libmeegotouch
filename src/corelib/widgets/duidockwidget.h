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

#ifndef DUIDOCKWIDGET_H
#define DUIDOCKWIDGET_H

#include "duiscenewindow.h"

class DuiDockWidgetPrivate;
class DuiToolBar;

//! \internal
/*!
 * \class DuiDockWidget
 * \brief DuiDockWidget implements a dock widget
 *
 * DuiDockWidget doesn't have that many parameters which are read from the
 * theme. The theming is done changing the items which lay inside the
 * dockwidget.
 */

// TODO: Get rid of this class altogether.
class DuiDockWidget : public DuiSceneWindow
{
    Q_OBJECT

public:
    //! \brief Constructor
    DuiDockWidget(QGraphicsWidget *parent = 0);
    //! \brief Destructor
    virtual ~DuiDockWidget();

    //! \brief Undocks the currently docked toolbar
    //! \return Pointer to the toolbar just undocked or zero if none docked
    void undockToolBar();

    DuiToolBar *toolBar();

public Q_SLOTS:
    //! \brief Docks the given toolbar. Ownership is NOT transferred (i.e. DuiDockWidget does not delete ToolBar on destruction
    //! \param toolbar The toolbar to dock
    void dockToolBar(DuiToolBar *toolbar);

private:
    Q_DISABLE_COPY(DuiDockWidget)
    Q_DECLARE_PRIVATE(DuiDockWidget)

#ifdef UNIT_TEST
    friend class Ut_DuiDockWidget;
#endif // UNIT_TEST
    friend class DuiApplicationWindow;
};

//! \internal_end

#endif // DUIDOCKWIDGET
