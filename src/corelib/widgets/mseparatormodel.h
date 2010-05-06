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

#ifndef MSEPARATORMODEL_H
#define MSEPARATORMODEL_H

#include <mwidgetmodel.h>

/*!
    \class MSeparatorModel
    \brief Data model class for MSeparator

    \ingroup models
    \sa MSeparator
*/

class M_EXPORT MSeparatorModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MSeparatorModel)

    /*!
        \property MSeparatorModel::orientation
        \brief display orientation
    */
    M_MODEL_PROPERTY(Qt::Orientation, orientation, Orientation, true, Qt::Horizontal)

};

#endif

