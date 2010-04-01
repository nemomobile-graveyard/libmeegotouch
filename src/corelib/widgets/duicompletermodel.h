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

#ifndef DUICOMPLETERMODEL_H
#define DUICOMPLETERMODEL_H

#include <QStringList>
#include "duiscenewindowmodel.h"
class QAbstractItemModel;
class DuiWidget;

class DUI_EXPORT DuiCompleterModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiCompleterModel)

private:
    DUI_MODEL_PTR_PROPERTY(DuiWidget *, textWidget, TextWidget, true, NULL)
    DUI_MODEL_PROPERTY(QString, completionPrefix, CompletionPrefix, true, QString())
    DUI_MODEL_PTR_PROPERTY(QAbstractItemModel *, matchedModel, MatchedModel, true, NULL)
    DUI_MODEL_PROPERTY(int, valueColumnIndex, ValueColumnIndex, true, 0)
    DUI_MODEL_PROPERTY(int, matchedIndex, MatchedIndex, true, -1)
    DUI_MODEL_PROPERTY(bool, active, Active, true, false)
    DUI_MODEL_PROPERTY(bool, popupActive, PopupActive, true, false)
    DUI_MODEL_PROPERTY(bool, acceptMultipleEntries, AcceptMultipleEntries, true, true)
    DUI_MODEL_PROPERTY(QString, charactersToTrim, CharactersToTrim, true, QString())

};

#endif

