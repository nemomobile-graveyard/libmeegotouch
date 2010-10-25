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

#ifndef MEXTENSIONAREAMODEL_H_
#define MEXTENSIONAREAMODEL_H_

#include <MWidgetModel>

class MDataStore;

typedef QMap<QGraphicsWidget *, MDataStore *> DataStoreMap;

/*!
 * MExtensionAreaModel is the model class for MExtensionArea.
 */
class M_EXTENSIONS_EXPORT MExtensionAreaModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MExtensionAreaModel)

public:
    //! A map of widgets and the associated datastores
    M_MODEL_PTR_PROPERTY(DataStoreMap *, dataStores, DataStores, true, NULL)

public:
    /*!
     * Emits the memberModified() signal for dataStores. Can be used
     * when the data pointed to by dataStores has changed.
     */
    void dataStoresModified();
};

#endif /* MEXTENSIONAREAMODEL_H_ */
