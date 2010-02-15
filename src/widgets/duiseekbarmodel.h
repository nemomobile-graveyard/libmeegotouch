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

#ifndef DUISEEKBARMODEL_H
#define DUISEEKBARMODEL_H

#include <duislidermodel.h>

/*!
    \class DuiSliderModel
    \brief Model class for DuiSlider.

    \ingroup models
    \sa DuiSlider
*/
class DUI_EXPORT DuiSeekBarModel : public DuiSliderModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiSeekBarModel)

private:
    /*!
        \property DuiSeekBarModel::loadedContentMin
        \brief Minimum value of seekbar loaded content range.
    */
    DUI_MODEL_PROPERTY(int, loadedContentMin, LoadedContentMin, true, 0)
    /*!
        \property DuiSeekBarModel::loadedContentMax
        \brief Maximum value of seekbar loaded content range.
    */
    DUI_MODEL_PROPERTY(int, loadedContentMax, LoadedContentMax, true, 0)
};

#endif

