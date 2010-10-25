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

#ifndef MGRIDITEMMODEL_H
#define MGRIDITEMMODEL_H

#include <mwidgetmodel.h>

/*!
    \class MGridItemModel
    \brief Data model class for MGridItem.

    \ingroup models
    \sa MGridItem
*/
class M_CORE_EXPORT MGridItemModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MGridItemModel)

public:
    /*!
        \property MGridItemModel::image
        \brief Id of the image.
    */
    M_MODEL_PROPERTY(QString, image, Image, true, QString())

    /*!
        \property MGridItemModel::title
        \brief Text of title label.
    */
    M_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property MGridItemModel::subtitle
        \brief Text of subtitle label.
    */
    M_MODEL_PROPERTY(QString, subtitle, Subtitle, true, QString())

    /*!
        \property MGridItemModel::imageVisible
        \brief Boolean value that defines whether image is displayed or not.
    */
    M_MODEL_PROPERTY(bool, imageVisible, ImageVisible, true, true)

    /*!
        \property MGridItemModel::titleVisible
        \brief Boolean value that defines whether title is displayed or not.
    */
    M_MODEL_PROPERTY(bool, titleVisible, TitleVisible, true, true)

    /*!
        \property MGridItemModel::subtitleVisible
        \brief Boolean value that defines whether subtitle is displayed or not.
    */
    M_MODEL_PROPERTY(bool, subtitleVisible, SubtitleVisible, true, true)

};

#endif
