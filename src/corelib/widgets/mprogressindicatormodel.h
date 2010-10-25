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

#ifndef MPROGRESSINDICATORMODEL_H
#define MPROGRESSINDICATORMODEL_H

#include <mwidgetmodel.h>

/*!
    \class MProgressIndicatorModel
    \brief This is the data model class for progress indicator.

    \ingroup models
    \sa MProgressIndicator
*/

class M_CORE_EXPORT MProgressIndicatorModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MProgressIndicatorModel)

    /*!
    * \property MProgressIndicatorModel::value
    * \brief This property holds the progress indicator's current value.
    */
    M_MODEL_PROPERTY(int, value, Value, true, 0)

    /*!
    * \property MProgressIndicatorModel::minimum
    * \brief This property holds the progress indicator's minimum value.
    */
    M_MODEL_PROPERTY(int, minimum, Minimum, true, 0)

    /*!
    * \property MProgressIndicatorModel::maximum
    * \brief This property holds the progress indicator's maximum value.
    */
    M_MODEL_PROPERTY(int, maximum, Maximum, true, 100)

    /*!
    * \property MProgressIndicatorModel::unknownDuration
    * \brief This property holds whether the progress indicator is type of known duration / unknown duration.
    */
    M_MODEL_PROPERTY(bool, unknownDuration, UnknownDuration, true, false)
};

#endif

