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

#ifndef DUIAPPLICATIONMENUBUTTONMODEL_H
#define DUIAPPLICATIONMENUBUTTONMODEL_H

//! \internal

#include "duibuttonmodel.h"

/*!
    \class DuiApplicationMenuButtonModel
    \brief Data model class for DuiApplicationMenuButton.

    \ingroup models
    \sa DuiApplicationMenuButton
*/
class DuiApplicationMenuButtonModel : public DuiButtonModel
{
    Q_OBJECT
    DUI_MODEL(DuiApplicationMenuButtonModel)

    /*!
        \property DuiApplicationMenuButtonModel::progressIndicatorVisible
        \brief Boolean value that defines whether progress Indicator is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property DuiApplicationMenuButtonModel::arrowIconVisible
        \brief Boolean value that defines whether arrow icon is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, arrowIconVisible, ArrowIconVisible, true, false)
};

//! \internal_end

#endif

