/***************************************************************************
**
** Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MTEXTEDITMODEL_H
#define MTEXTEDITMODEL_H

#include <mwidgetmodel.h>
#include <limits>
#include <QTextDocument>
#include <QTextCharFormat>
#include "mnamespace.h"

class MTextEditFormatRange
{
 public:
    bool operator ==(const MTextEditFormatRange& other) const
    {
        return (start == other.start && length == other.length && format == other.format);
    }
    
    int start;
    int length;
    QTextCharFormat format;
};

class M_CORE_EXPORT MTextEditModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MTextEditModel)
    M_MODEL_CUSTOM_DESTRUCTOR

public:

    //! Edit mode for MTextEdit
    enum EditMode {
        //! The main mode of operation, no selection or preedit
        EditModeBasic,

        //! preedit editing initiated
        EditModeActive,

        //! Text selected
        EditModeSelect
    };

    //! Line mode for MTextEdit, single line or multiline
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

    M_MODEL_PROPERTY(MTextEditModel::EditMode, edit, Edit, true, MTextEditModel::EditModeBasic)
    M_MODEL_PROPERTY(MTextEditModel::LineMode, line, Line, true, MTextEditModel::SingleLine)
    M_MODEL_PTR_PROPERTY(QTextDocument *, document, Document, true, NULL)
    M_MODEL_PTR_PROPERTY(QTextCursor *, cursor, Cursor, true, NULL)

    M_MODEL_PROPERTY(QString, text, Text, true, QString())
    M_MODEL_PROPERTY(M::TextContentType, type, Type, true, M::FreeTextContentType)
    M_MODEL_PROPERTY(Qt::TextInteractionFlags, textInteractionFlags, TextInteractionFlags, true, Qt::TextEditorInteraction)

    M_MODEL_PROPERTY(bool, errorHighlight, ErrorHighlight, true, false)
    M_MODEL_PROPERTY(bool, inputMethodCorrectionEnabled, InputMethodCorrectionEnabled, true, true)
    M_MODEL_PROPERTY(bool, isReadOnly, ReadOnly, true, false)
    M_MODEL_PROPERTY(bool, inputMethodAutoCapitalizationEnabled, InputMethodAutoCapitalizationEnabled, true, true)
    M_MODEL_PROPERTY(bool, autoSelectionEnabled, AutoSelectionEnabled, true, false)
    M_MODEL_PROPERTY(bool, inputMethodPredictionEnabled, InputMethodPredictionEnabled, true, false)
    M_MODEL_PROPERTY(int,  maxLength, MaxLength, true, std::numeric_limits<int>::max())
    M_MODEL_PROPERTY(QString, prompt, Prompt, true, QString())
    M_MODEL_PROPERTY(QString, toolbar, Toolbar, true, QString())
    M_MODEL_PROPERTY(int, toolbarId, ToolbarId, true, -1)
    M_MODEL_PROPERTY(MTextEditModel::EchoMode, echo, Echo, true, MTextEditModel::Normal)
    M_MODEL_PROPERTY(bool, isAutoSipEnabled, AutoSipEnabled, true, true)
    M_MODEL_PROPERTY(int, preeditCursor, PreeditCursor, true, -1)
    M_MODEL_PROPERTY(QList<MTextEditFormatRange>, additionalFormats, AdditionalFormats, true, QList<MTextEditFormatRange>())
public:
    void updateCursor()   {
        memberModified(MTextEditModel::Cursor);
    }
};

#endif
