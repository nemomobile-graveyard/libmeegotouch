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

#ifndef DUIPOPUPLISTMODEL_H
#define DUIPOPUPLISTMODEL_H

#include "duidialogmodel.h"

/*!
    \class DuiPopupListModel
    \brief Data model class for DuiPopupList.

    \ingroup models
    \sa DuiPopupListModel
*/

class DUI_EXPORT DuiPopupListModel : public DuiDialogModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiPopupListModel)

    /*!
        \property DuiPopupListModel::batchSize
        \brief This property holds the number of items laid out in each batch.
        The default value is 20.
        \deprecated Since 0.19
    */
    DUI_MODEL_PROPERTY(int, batchSize, BatchSize, true, 20)

};

#endif
