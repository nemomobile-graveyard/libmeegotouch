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

#ifndef DUIINFOBANNERMODEL_H
#define DUIINFOBANNERMODEL_H

#include "duiscenewindowmodel.h"

class DUI_EXPORT DuiInfoBannerModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiInfoBannerModel)

private:
    /*!
        \property Icon in banner.
    */
    DUI_MODEL_PROPERTY(QString, iconID, IconID, true, QString())

    /*!
        \property Body text in banner.
    */
    DUI_MODEL_PROPERTY(QString, bodyText, BodyText, true, QString())

    /*!
        \property Image in banner.
        \note This will reset the pixmap property, if it was set.
    */
    DUI_MODEL_PROPERTY(QString, imageID, ImageID, false, QString())

    /*!
        \property Text for button in interactive banner.
    */
    DUI_MODEL_PROPERTY(QString, buttonText, ButtonText, true, QString())

    /*!
        \property Image in banner.
        \note This will override the image property, if it was set.
    */
    DUI_MODEL_PROPERTY(QPixmap, pixmap, Pixmap, false, QPixmap())
};

#endif

