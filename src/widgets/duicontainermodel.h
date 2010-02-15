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

#ifndef DUICONTAINERMODEL_H
#define DUICONTAINERMODEL_H

#include <duiwidgetmodel.h>
#include <duiwidget.h>

class DuiWidget;

/*!
 * \class DuiContainerModel
 * \brief The model class of DuiContainer.
 *
 * \sa DuiContainer, DuiContainerView
 */

class DUI_EXPORT DuiContainerModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiContainerModel)

private:
    DUI_MODEL_PROPERTY(QString, title, Title, true, QString())
    DUI_MODEL_PROPERTY(QString, text, Text, true, QString())
    DUI_MODEL_PROPERTY(QString, icon, Icon, true, QString())
    DUI_MODEL_PROPERTY(bool, headerVisible, HeaderVisible, true, true)
    DUI_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)
    DUI_MODEL_PTR_PROPERTY(DuiWidget *, centralWidget, CentralWidget, true, NULL)
};

#endif

