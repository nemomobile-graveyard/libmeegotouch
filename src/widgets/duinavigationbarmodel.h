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

#ifndef DUINAVIGATIONBARMODEL_H
#define DUINAVIGATIONBARMODEL_H

#include <duiscenewindowmodel.h>

class DuiToolBar;

class DUI_EXPORT DuiNavigationBarModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiNavigationBarModel)

    DUI_MODEL_PROPERTY(bool, notifyUser, NotifyUser, true, false)
    DUI_MODEL_PROPERTY(QString, viewMenuDescription, ViewMenuDescription, true, "Menu")
    DUI_MODEL_PROPERTY(QString, viewMenuIconID, ViewMenuIconID, true, QString())
    DUI_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)
    DUI_MODEL_PROPERTY(bool, arrowIconVisible, ArrowIconVisible, true, false)
    DUI_MODEL_PTR_PROPERTY(DuiToolBar *, toolBar, ToolBar, true, 0)
};

#endif
