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

#ifndef STATUSINDICATORMENUMODEL_H_
#define STATUSINDICATORMENUMODEL_H_

#include <MWidgetModel>
#include "declsettings/parser.h"

// this is a second test case taken from mhome
// it doesn't have the M_EXPORT in the class line

class StatusIndicatorMenuModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(StatusIndicatorMenuModel)

    //! Whether the close button should be visible or not
    M_MODEL_PROPERTY(bool, closeButtonVisible, CloseButtonVisible, true, false)
    //! A list of menu items
    M_MODEL_PROPERTY(QList<SettingsItem *>, settingsItems, SettingsItems, true, QList<SettingsItem *>())
};

#endif /* STATUSINDICATORMENUMODEL_H_ */
