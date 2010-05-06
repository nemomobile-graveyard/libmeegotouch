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

#ifndef MDOCKWIDGETVIEW_H
#define MDOCKWIDGETVIEW_H

#include "mscenewindowview.h"
#include "mdockwidgetmodel.h"
#include <mdockwidgetstyle.h>

class MDockWidgetViewPrivate;
class MDockWidget;

//! \internal
class MDockWidgetView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MDockWidgetModel, MDockWidgetStyle)
public:
    explicit MDockWidgetView(MDockWidget *controller);
    virtual ~MDockWidgetView();

private:
    Q_DISABLE_COPY(MDockWidgetView)
    Q_DECLARE_PRIVATE(MDockWidgetView)
};

//! \internal_end

#endif // MDOCKWIDGETVIEW
