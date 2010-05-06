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

#ifndef MDOCKWIDGET_H
#define MDOCKWIDGET_H

#include "mscenewindow.h"

class MDockWidgetPrivate;
class MToolBar;

//! \internal
/*!
 * \class MDockWidget
 * \brief MDockWidget implements a dock widget
 *
 * MDockWidget doesn't have that many parameters which are read from the
 * theme. The theming is done changing the items which lay inside the
 * dockwidget.
 */

// TODO: Get rid of this class altogether.
class MDockWidget : public MSceneWindow
{
    Q_OBJECT

public:
    //! \brief Constructor
    MDockWidget(QGraphicsWidget *parent = 0);
    //! \brief Destructor
    virtual ~MDockWidget();

    //! \brief Undocks the currently docked toolbar
    //! \return Pointer to the toolbar just undocked or zero if none docked
    void undockToolBar();

    MToolBar *toolBar();

public Q_SLOTS:
    //! \brief Docks the given toolbar. Ownership is NOT transferred (i.e. MDockWidget does not delete ToolBar on destruction
    //! \param toolbar The toolbar to dock
    void dockToolBar(MToolBar *toolbar);

private:
    Q_DISABLE_COPY(MDockWidget)
    Q_DECLARE_PRIVATE(MDockWidget)

#ifdef UNIT_TEST
    friend class Ut_MDockWidget;
#endif // UNIT_TEST
    friend class MApplicationWindow;
};

//! \internal_end

#endif // MDOCKWIDGET
