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

#include <DuiWidgetModel>
#include <QStringList>

class DuiWidget;
class DuiDataStore;

typedef QMap<DuiWidget *, DuiDataStore *> DataStoreMap;

/*!
 * DuiMashupCanvasModel is the model class for DuiMashupCanvas.
 */
class DUI_EXPORT DuiMashupCanvasModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiMashupCanvasModel)

public:
    //! A list of widgets
    DUI_MODEL_PTR_PROPERTY(DataStoreMap *, dataStores, DataStores, true, NULL)

    //! A list of names of the applet categories to show in this mashup canvas
    DUI_MODEL_PROPERTY(QStringList, categories, Categories, true, QStringList())

public:
    /*!
     * Emits the memberModified() signal for dataStores. Can be used
     * when the data pointed to by dataStores has changed.
     */
    void dataStoresModified();
};

#endif /* DUIMASHUPCANVASMODEL_H_ */
