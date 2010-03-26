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

#ifndef DUIAPPLETINVENTORYMODEL_H_
#define DUIAPPLETINVENTORYMODEL_H_

#include "duiwidgetmodel.h"

class DuiWidget;

//! \internal
typedef QList<DuiWidget *> WidgetList;

class DuiAppletInventoryModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiAppletInventoryModel)

private:
    //! The list of widgets in the applet inventory
    DUI_MODEL_PROPERTY(WidgetList, widgets, Widgets, true, WidgetList())

    //! Whether the close button should be visible or not
    DUI_MODEL_PROPERTY(bool, closeButtonVisible, CloseButtonVisible, true, false)
};
//! \internal_end

#endif /* DUIAPPLETINVENTORYMODEL_H_ */
