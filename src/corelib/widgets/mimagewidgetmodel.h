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

#ifndef MIMAGEWIDGETMODEL_H
#define MIMAGEWIDGETMODEL_H

#include <mwidgetmodel.h>

/*!
    \class MImageWidgetModel
    \brief Data model class for MImageWidget.

    \ingroup models
    \sa MImageWidget
*/

class M_CORE_EXPORT MImageWidgetModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MImageWidgetModel)

    /*!
        \property MImageWidgetModel::zoomFactorX
        \brief image zoom factor in width
    */
    M_MODEL_PROPERTY(qreal, zoomFactorX, ZoomFactorX, true, 0.0)

    /*!
        \property MImageWidgetModel::zoomFactorY
        \brief image zoom factor in height
    */
    M_MODEL_PROPERTY(qreal, zoomFactorY, ZoomFactorY, true, 0.0)

    /*!
        \property MImageWidgetModel::crop
        \brief crop section rectangle
    */
    M_MODEL_PROPERTY(QRectF, crop, Crop, true, QRectF())

    /*!
        \property MImageWidgetModel::aspectRatioMode
        \brief image zoom AspectRatioMode
    */
    M_MODEL_PROPERTY(Qt::AspectRatioMode, aspectRatioMode, AspectRatioMode, true, Qt::KeepAspectRatio)

    /*!
        \property MImageWidgetModel::imageId
        \brief Image logical id.
    */
    M_MODEL_PROPERTY(QString, imageId, ImageId, true, QString())

    /*!
        \property MImageWidgetModel::imageSize
        \brief Preferred image size.
    */
    M_MODEL_PROPERTY(QSize, imageSize, ImageSize, true, QSize())
};

#endif
