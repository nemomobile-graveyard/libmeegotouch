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

#ifndef MCOMPLETERMODEL_H
#define MCOMPLETERMODEL_H

#include <QStringList>
#include "mscenewindowmodel.h"
class QAbstractItemModel;
class MWidget;

class M_CORE_EXPORT MCompleterModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MCompleterModel)

private:
    M_MODEL_PTR_PROPERTY(MWidget *, textWidget, TextWidget, true, NULL)
    M_MODEL_PROPERTY(QString, completionPrefix, CompletionPrefix, true, QString())
    M_MODEL_PTR_PROPERTY(QAbstractItemModel *, matchedModel, MatchedModel, true, NULL)
    M_MODEL_PROPERTY(int, valueColumnIndex, ValueColumnIndex, true, 0)
    M_MODEL_PROPERTY(int, matchedIndex, MatchedIndex, true, -1)
    M_MODEL_PROPERTY(bool, active, Active, true, false)
    M_MODEL_PROPERTY(bool, popupActive, PopupActive, true, false)
    M_MODEL_PROPERTY(bool, acceptMultipleEntries, AcceptMultipleEntries, true, true)
    M_MODEL_PROPERTY(QString, charactersToTrim, CharactersToTrim, true, QString(" "))
    M_MODEL_PROPERTY(QString, charactersToTrimForCompletionPrefix, CharactersToTrimForCompletionPrefix, true, QString())
    M_MODEL_PROPERTY(QString, completionTitle, CompletionTitle, true, QString())
};

#endif

