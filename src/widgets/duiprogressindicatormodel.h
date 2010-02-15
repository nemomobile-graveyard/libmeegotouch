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

#ifndef DUIPROGRESSINDICATORMODEL_H
#define DUIPROGRESSINDICATORMODEL_H

#include <duiwidgetmodel.h>

/*!
    \class DuiProgressIndicatorModel
    \brief This is the data model class for progress indicator.

    \ingroup models
    \sa DuiProgressIndicator
*/

class DUI_EXPORT DuiProgressIndicatorModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiProgressIndicatorModel)

    /*!
    * \property DuiProgressIndicatorModel::value
    * \brief This property holds the progress indicator's current value.
    */
    DUI_MODEL_PROPERTY(int, value, Value, true, 0)

    /*!
    * \property DuiProgressIndicatorModel::minimum
    * \brief This property holds the progress indicator's minimum value.
    */
    DUI_MODEL_PROPERTY(int, minimum, Minimum, true, 0)

    /*!
    * \property DuiProgressIndicatorModel::maximum
    * \brief This property holds the progress indicator's maximum value.
    */
    DUI_MODEL_PROPERTY(int, maximum, Maximum, true, 100)

    /*!
    * \property DuiProgressIndicatorModel::unknownDuration
    * \brief This property holds whether the progress indicator is type of known duration / unknown duration.
    */
    DUI_MODEL_PROPERTY(bool, unknownDuration, UnknownDuration, true, false)
};

#endif

