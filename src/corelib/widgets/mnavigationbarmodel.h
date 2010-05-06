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

#ifndef MNAVIGATIONBARMODEL_H
#define MNAVIGATIONBARMODEL_H

#include <mscenewindowmodel.h>

class MToolBar;

class M_EXPORT MNavigationBarModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MNavigationBarModel)

    M_MODEL_PROPERTY(bool, notifyUser, NotifyUser, true, false)
    M_MODEL_PROPERTY(QString, viewMenuDescription, ViewMenuDescription, true, "Menu")
    M_MODEL_PROPERTY(QString, viewMenuIconID, ViewMenuIconID, true, QString())
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)
    M_MODEL_PROPERTY(bool, arrowIconVisible, ArrowIconVisible, true, false)
    M_MODEL_PTR_PROPERTY(MToolBar *, toolBar, ToolBar, true, 0)
};

#endif
