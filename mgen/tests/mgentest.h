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

class M_EXPORT MGenTest : public MWidgetModel
{
    M_MODEL_INTERNAL(MGenTest)
    M_MODEL_PTR_PROPERTY(QList<QString *>, test1, Test, true, NULL)
    M_MODEL_PTR_PROPERTY(QList<QString *>, test2, Test, true, NULL)
    M_MODEL_PTR_PROPERTY(QString *, test3, Test, true, NULL)
    M_MODEL_PROPERTY(MTextEditModel::EditMode, edit, Edit, true, MTextEditModel::EditModeBasic)
    M_MODEL_PROPERTY(MTextEditModel::LineMode, line, Line, true, MTextEditModel::SingleLine)
    M_MODEL_PTR_PROPERTY(QList<QTextDocument *>, document, Document, true, NULL)
    M_MODEL_PTR_PROPERTY(QTextCursor *, cursor, Cursor, true, NULL)
    M_MODEL_PROPERTY(MTextEditModel::ContentType, type, Type, true, MTextEditModel::FreeText)
    M_MODEL_PROPERTY(bool, maskedInput, MaskedInput, true, false)
    M_MODEL_PROPERTY(Qt::TextInteractionFlags, textInteractionFlags, TextInteractionFlags, true, Qt::TextEditorInteraction)
    M_MODEL_PROPERTY(bool, autoSelectionEnabled, AutoSelectionEnabled, true, false)
    M_MODEL_PROPERTY(bool, imCorrectionEnabled, ImCorrectionEnabled, true, true)
    M_MODEL_PROPERTY(bool, imPredictionEnabled, ImPredictionEnabled, true, false)
}
