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

#ifndef DUIGRIDITEMMODEL_H
#define DUIGRIDITEMMODEL_H

#include <duiwidgetmodel.h>

/*!
    \class DuiGridItemModel
    \brief Data model class for DuiGridItem.

    \ingroup models
    \sa DuiGridItem
*/
class DUI_EXPORT DuiGridItemModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiGridItemModel)

public:
    /*!
        \property DuiGridItemModel::image
        \brief Id of the image.
    */
    DUI_MODEL_PROPERTY(QString, image, Image, true, QString())

    /*!
        \property DuiGridItemModel::title
        \brief Text of title label.
    */
    DUI_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property DuiGridItemModel::subtitle
        \brief Text of subtitle label.
    */
    DUI_MODEL_PROPERTY(QString, subtitle, Subtitle, true, QString())

    /*!
        \property DuiGridItemModel::imageVisible
        \brief Boolean value that defines whether image is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, imageVisible, ImageVisible, true, true)

    /*!
        \property DuiGridItemModel::titleVisible
        \brief Boolean value that defines whether title is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, titleVisible, TitleVisible, true, true)

    /*!
        \property DuiGridItemModel::subtitleVisible
        \brief Boolean value that defines whether subtitle is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, subtitleVisible, SubtitleVisible, true, true)

};

#endif
