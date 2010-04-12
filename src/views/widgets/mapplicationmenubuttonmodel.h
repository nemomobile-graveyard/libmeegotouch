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

#ifndef MAPPLICATIONMENUBUTTONMODEL_H
#define MAPPLICATIONMENUBUTTONMODEL_H

//! \internal

#include "mbuttonmodel.h"

/*!
    \class MApplicationMenuButtonModel
    \brief Data model class for MApplicationMenuButton.

    \ingroup models
    \sa MApplicationMenuButton
*/
class MApplicationMenuButtonModel : public MButtonModel
{
    Q_OBJECT
    M_MODEL(MApplicationMenuButtonModel)

    /*!
        \property MApplicationMenuButtonModel::progressIndicatorVisible
        \brief Boolean value that defines whether progress Indicator is displayed or not.
    */
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property MApplicationMenuButtonModel::arrowIconVisible
        \brief Boolean value that defines whether arrow icon is displayed or not.
    */
    M_MODEL_PROPERTY(bool, arrowIconVisible, ArrowIconVisible, true, false)
};

//! \internal_end

#endif

