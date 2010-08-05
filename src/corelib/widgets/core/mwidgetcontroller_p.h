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

#ifndef MWIDGETCONTROLLER_P_H
#define MWIDGETCONTROLLER_P_H

#include <QHash>
#include "mwidget_p.h"
#include "mwidgetcontroller.h"

class MWidgetControllerPrivate : public MWidgetPrivate
{
    Q_DECLARE_PUBLIC(MWidgetController)

public:
    MWidgetControllerPrivate();
    virtual ~MWidgetControllerPrivate();

private: // only for MWidgetController
    /*!
     Sets the \a view state to match the state of the controller. This is used to configure new
     views whether they were assigned manually by user or created automatically
     from MTheme.
     */
    void configureView(MWidgetView *view);

    /*!
      Releases current view if it is not manually set by application developer.

      After calling this the next view() call will cause creation of a new view unless the view
      was manually set. This method should be called only from MTheme.
     */
    void deprecateView();

    /*!
     Creates a new view associated with this controller using MTheme.
     */
    void createView();

    /*! Set the model to the given model */
    void setModel(MWidgetModel *model);

    MWidgetModel *model;
    MWidgetView *view;
    bool viewSetManually;
    bool active;
    // Safeguard variable to prevent widget controller from creating multiple views
    // for one controller.
    bool constructingView;
    bool modelSetup;

    // contains a pointer to every widget instance in the system.
    static QSet<MWidgetController *> allSystemWidgets;

    friend class MTheme;
};

#endif // MWIDGETCONTROLLER_P_H
