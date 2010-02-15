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

#ifndef DUITEXTEDITMODEL_H
#define DUITEXTEDITMODEL_H

#include <duiwidgetmodel.h>
#include <limits>
#include <QTextDocument>
#include "duinamespace.h"

class DUI_EXPORT DuiTextEditModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiTextEditModel)

public:

    //! Edit mode for DuiTextEdit
    enum EditMode {
        //! The main mode of operation, no selection or preedit
        EditModeBasic,

        //! preedit editing initiated
        EditModeActive,

        //! Text selected
        EditModeSelect
    };

    //! Line mode for DuiTextEdit, single line or multiline
    enum LineMode {
        //! Only one line allowed, no more can be created
        SingleLine,

        //! May have arbitrary number of lines
        MultiLine
    };

    enum EchoMode {
        //! Display characters as they are entered, default.
        Normal,

        //! Do not display entered characters.
        NoEcho,

        //! Entered characters are replaced by mask characters.
        Password,

        //! Input is displayed clear text while doing input, replaced by mask characters afterwards.
        PasswordEchoOnEdit
    };

private:

    DUI_MODEL_PROPERTY(DuiTextEditModel::EditMode, edit, Edit, true, DuiTextEditModel::EditModeBasic)
    DUI_MODEL_PROPERTY(DuiTextEditModel::LineMode, line, Line, true, DuiTextEditModel::SingleLine)
    DUI_MODEL_PTR_PROPERTY(QTextDocument *, document, Document, true, NULL)
    DUI_MODEL_PTR_PROPERTY(QTextCursor *, cursor, Cursor, true, NULL)

    DUI_MODEL_PROPERTY(QString, text, Text, true, QString())
    DUI_MODEL_PROPERTY(Dui::TextContentType, type, Type, true, Dui::FreeTextContentType)
    DUI_MODEL_PROPERTY(Qt::TextInteractionFlags, textInteractionFlags, TextInteractionFlags, true, Qt::TextEditorInteraction)

    DUI_MODEL_PROPERTY(bool, inputMethodCorrectionEnabled, InputMethodCorrectionEnabled, true, true)
    DUI_MODEL_PROPERTY(bool, isReadOnly, ReadOnly, true, false)
    DUI_MODEL_PROPERTY(bool, inputMethodAutoCapitalizationEnabled, InputMethodAutoCapitalizationEnabled, true, true)
    DUI_MODEL_PROPERTY(bool, autoSelectionEnabled, AutoSelectionEnabled, true, false)
    DUI_MODEL_PROPERTY(bool, inputMethodPredictionEnabled, InputMethodPredictionEnabled, true, false)
    DUI_MODEL_PROPERTY(int,  maxLength, MaxLength, true, std::numeric_limits<int>::max())
    DUI_MODEL_PROPERTY(QString, prompt, Prompt, true, QString())
    DUI_MODEL_PROPERTY(QString, toolbar, Toolbar, true, QString())
    DUI_MODEL_PROPERTY(DuiTextEditModel::EchoMode, echo, Echo, true, DuiTextEditModel::Normal)

public:
    void updateCursor()   {
        memberModified(DuiTextEditModel::Cursor);
    }
};

#endif
