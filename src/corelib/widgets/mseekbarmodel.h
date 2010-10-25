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

#ifndef MSEEKBARMODEL_H
#define MSEEKBARMODEL_H

#include <mslidermodel.h>

/*!
    \class MSliderModel
    \brief Model class for MSlider.

    \ingroup models
    \sa MSlider
*/
class M_CORE_EXPORT MSeekBarModel : public MSliderModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MSeekBarModel)

private:
    /*!
        \property MSeekBarModel::loadedContentMin
        \brief Minimum value of seekbar loaded content range.
    */
    M_MODEL_PROPERTY(int, loadedContentMin, LoadedContentMin, true, 0)
    /*!
        \property MSeekBarModel::loadedContentMax
        \brief Maximum value of seekbar loaded content range.
    */
    M_MODEL_PROPERTY(int, loadedContentMax, LoadedContentMax, true, 0)
};

#endif

