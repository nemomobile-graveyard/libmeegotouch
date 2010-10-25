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

#ifndef MCONTAINERMODEL_H
#define MCONTAINERMODEL_H

#include <mwidgetmodel.h>

class QGraphicsWidget;

/*!
 * \class MContainerModel
 * \brief The model class of MContainer.
 *
 * \sa MContainer, MContainerView
 */

class M_CORE_EXPORT MContainerModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MContainerModel)

private:
    M_MODEL_PROPERTY(QString, title, Title, true, QString())
    M_MODEL_PROPERTY(QString, text, Text, true, QString())
    M_MODEL_PROPERTY(QString, icon, Icon, true, QString())
    M_MODEL_PROPERTY(bool, headerVisible, HeaderVisible, true, true)
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, centralWidget, CentralWidget, true, NULL)
};

#endif

