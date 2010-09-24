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

#ifndef MBANNERMODEL_H
#define MBANNERMODEL_H

#include "mscenewindowmodel.h"
#include <QDateTime>

class M_EXPORT MBannerModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MBannerModel)

    /*!
        \property Icon in banner.
    */
    M_MODEL_PROPERTY(QString, iconID, IconID, true, QString())

    /*!
        \property Title for event banner
    */

    M_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property Subtitle for event banner
    */

    M_MODEL_PROPERTY(QString, subtitle, Subtitle, true, QString())

    /*!
        \property Datetime
    */

    M_MODEL_PROPERTY(QDateTime, bannerTimeStamp, BannerTimeStamp, true, QDateTime())
};

#endif

