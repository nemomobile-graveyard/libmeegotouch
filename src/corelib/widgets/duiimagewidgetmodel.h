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

#ifndef DUIIMAGEWIDGETMODEL_H
#define DUIIMAGEWIDGETMODEL_H

#include <duiwidgetmodel.h>

/*!
    \class DuiImageWidgetModel
    \brief Data model class for DuiImageWidget.

    \ingroup models
    \sa DuiImageWidget
*/

class DUI_EXPORT DuiImageWidgetModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiImageWidgetModel)

    /*!
        \property DuiImageWidgetModel::zoomFactorX
        \brief image zoom factor in width
    */
    DUI_MODEL_PROPERTY(float, zoomFactorX, ZoomFactorX, true, 0.0)

    /*!
        \property DuiImageWidgetModel::zoomFactorY
        \brief image zoom factor in height
    */
    DUI_MODEL_PROPERTY(float, zoomFactorY, ZoomFactorY, true, 0.0)

    /*!
        \property DuiImageWidgetModel::crop
        \brief crop section rectangle
    */
    DUI_MODEL_PROPERTY(QRectF, crop, Crop, true, QRectF())

    /*!
        \property DuiImageWidgetModel::aspectRatioMode
        \brief image zoom AspectRatioMode
    */
    DUI_MODEL_PROPERTY(Qt::AspectRatioMode, aspectRatioMode, AspectRatioMode, true, Qt::KeepAspectRatio)
};

#endif
