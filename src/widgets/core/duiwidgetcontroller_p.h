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

#ifndef DUIWIDGETCONTROLLER_P_H
#define DUIWIDGETCONTROLLER_P_H

#include <QHash>
#include "duiwidget_p.h"
#include "duiwidgetcontroller.h"

class DuiWidgetControllerPrivate : public DuiWidgetPrivate
{
    Q_DECLARE_PUBLIC(DuiWidgetController)

public:
    DuiWidgetControllerPrivate();
    virtual ~DuiWidgetControllerPrivate();

private: // only for DuiWidgetController
    /*!
     Sets the \a view state to match the state of the controller. This is used to configure new
     views whether they were assigned manually by user or created automatically
     from DuiTheme.
     */
    void configureView(DuiWidgetView *view);

    /*!
      Releases current view if it is not manually set by application developer.

      After calling this the next view() call will cause creation of a new view unless the view
      was manually set. This method should be called only from DuiTheme.
     */
    void deprecateView();

    /*!
     Creates a new view associated with this controller using DuiTheme.
     */
    void createView();

    DuiWidgetModel *model;
    DuiWidgetView *view;
    bool viewSetManually;
    bool active;
    // Safeguard variable to prevent widget controller from creating multiple views
    // for one controller.
    bool constructingView;
    bool modelSetup;

    // contains a pointer to every widget instance in the system.
    static QSet<DuiWidgetController *> allSystemWidgets;

    friend class DuiTheme;
};

#endif // DUIWIDGETCONTROLLER_P_H
