/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSHEETMODEL_H
#define MSHEETMODEL_H

#include <mscenewindowmodel.h>

class M_CORE_EXPORT MSheetModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MSheetModel)

    /*!
        \property MSheetModel::headerWidget
        \brief Widget to be placed on sheet's header.
    */
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, headerWidget, HeaderWidget, true, new QGraphicsWidget)

    /*!
        \property MSheetModel::centralWidget
        \brief Central widget of the sheet.
    */
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, centralWidget, CentralWidget, true, new QGraphicsWidget)

    M_MODEL_PROPERTY(bool, headerVisible, HeaderVisible, true, true)

    /*!
      \property MSheetModel::centralWidgetSizePolicyRespected
      \brief Same as MSheet::centralWidgetSizePolicyRespected

      \sa MSheet::centralWidgetSizePolicyRespected
     */
    M_MODEL_PROPERTY(bool, centralWidgetSizePolicyRespected, CentralWidgetSizePolicyRespected, true, false)

    /*!
        \property MSheetModel::headerFloating
        \brief Flag to show if the header should overlap the central widget or should adjust central widget size.
    */
    M_MODEL_PROPERTY(bool, headerFloating, HeaderFloating, true, false)
};

#endif // MSHEETMODEL_H
