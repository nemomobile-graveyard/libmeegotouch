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

#ifndef DUIMASHUPCANVASMODEL_H_
#define DUIMASHUPCANVASMODEL_H_

#include "duiextensionareamodel.h"
#include <QStringList>

class DuiWidget;
class DuiDataStore;

/*!
 * DuiMashupCanvasModel is the model class for DuiMashupCanvas.
 */
class DUI_EXPORT DuiMashupCanvasModel : public DuiExtensionAreaModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiMashupCanvasModel)

public:
    //! A list of names of the applet categories to show in this mashup canvas
    DUI_MODEL_PROPERTY(QStringList, categories, Categories, true, QStringList())
};

#endif /* DUIMASHUPCANVASMODEL_H_ */
