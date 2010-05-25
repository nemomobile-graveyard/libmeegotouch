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

#ifndef MRICHTEXTEDIT_H
#define MRICHTEXTEDIT_H

#include <MTextEdit>
#include <QTextCharFormat>

class MRichTextEditPrivate;

class M_EXPORT MRichTextEdit : public MTextEdit
{
    Q_OBJECT

public:
    /*!
     * \brief Default constructor. Creates a textEdit field containing the specified text and line mode with rich text support.
     * \param type widget type (single line or multiline).
     * \param text optional text.
     * \param parent optional parent.
     */
    explicit MRichTextEdit(MTextEditModel::LineMode type = MTextEditModel::SingleLine,
                             const QString &text = QString(), QGraphicsItem *parent = 0);
    /*!
     * \brief Creates a textEdit field with a specified line mode, model and parent with rich text support.
     * \param model the model to be used.
     * \param parent the parent of the text edit.
     */
    MRichTextEdit(MTextEditModel *model, QGraphicsItem *parent);

    /*!
     * \brief - Destructor
     */
    virtual ~MRichTextEdit();

    /*! \reimp */
    virtual void copy();

    virtual void paste();

    virtual void cut();
    /*! \reimp_end */

    /*!
     * \brief sets the Underline style and apply the style to the current selection if any
     */
    void setFontUnderline(bool underline);

    /*!
     * \brief sets the italic style and apply the style to the current selection if any
     */
    void setFontItalic(bool italic);

    /*!
     * \brief sets the bold style and apply the style to the current selection if any
     */
    void setFontBold(bool bold);

    /*!
     * \brief Retrieves the font information of current selection
     * Use the bold(),italic(),overline(), etc methods of font
     * object to get more info about the selection.
     * \return - Font properties of selection
     */
    QFont currentFont();

protected:
    /*! \reimp */
    virtual void keyPressEvent(QKeyEvent *event);
    /*! \reimp_end */

private:
    Q_DECLARE_PRIVATE(MRichTextEdit)
    Q_DISABLE_COPY(MRichTextEdit)

#ifdef UNIT_TEST
    friend class Ut_MRichTextEdit;
#endif
};

#endif //MRichTEXTEDIT_H
