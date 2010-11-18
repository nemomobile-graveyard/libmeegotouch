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

#ifndef MMASHUPCANVASMODEL_H_
#define MMASHUPCANVASMODEL_H_

#include "mextensionareamodel.h"
#include <QStringList>

class MWidget;
class MDataStore;

//! \internal
/*!
 * MMashupCanvasModel is the model class for MMashupCanvas.
 */
class M_EXTENSIONS_EXPORT MMashupCanvasModel : public MExtensionAreaModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MMashupCanvasModel)

public:
    //! A list of names of the applet categories to show in this mashup canvas
    M_MODEL_PROPERTY(QStringList, categories, Categories, true, QStringList())
};
//! \internal_end

#endif /* MMASHUPCANVASMODEL_H_ */
