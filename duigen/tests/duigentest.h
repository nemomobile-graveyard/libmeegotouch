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

class DUI_EXPORT DuiGenTest : public DuiWidgetModel
{
    DUI_MODEL_INTERNAL(DuiGenTest)
    DUI_MODEL_PTR_PROPERTY(QList<QString *>, test1, Test, true, NULL)
    DUI_MODEL_PTR_PROPERTY(QList<QString *>, test2, Test, true, NULL)
    DUI_MODEL_PTR_PROPERTY(QString *, test3, Test, true, NULL)
    DUI_MODEL_PROPERTY(DuiTextEditModel::EditMode, edit, Edit, true, DuiTextEditModel::EditModeBasic)
    DUI_MODEL_PROPERTY(DuiTextEditModel::LineMode, line, Line, true, DuiTextEditModel::SingleLine)
    DUI_MODEL_PTR_PROPERTY(QList<QTextDocument *>, document, Document, true, NULL)
    DUI_MODEL_PTR_PROPERTY(QTextCursor *, cursor, Cursor, true, NULL)
    DUI_MODEL_PROPERTY(DuiTextEditModel::ContentType, type, Type, true, DuiTextEditModel::FreeText)
    DUI_MODEL_PROPERTY(bool, maskedInput, MaskedInput, true, false)
    DUI_MODEL_PROPERTY(Qt::TextInteractionFlags, textInteractionFlags, TextInteractionFlags, true, Qt::TextEditorInteraction)
    DUI_MODEL_PROPERTY(bool, autoSelectionEnabled, AutoSelectionEnabled, true, false)
    DUI_MODEL_PROPERTY(bool, imCorrectionEnabled, ImCorrectionEnabled, true, true)
    DUI_MODEL_PROPERTY(bool, imPredictionEnabled, ImPredictionEnabled, true, false)
}
