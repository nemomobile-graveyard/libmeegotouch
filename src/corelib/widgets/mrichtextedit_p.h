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

#ifndef MRICHTEXTEDIT_P_H
#define MRICHTEXTEDIT_P_H

#include <QList>
#include <QAction>
#include <QMimeData>
#include <QTextDocumentFragment>
#include "mtextedit_p.h"
#include "mrichtextedit.h"

#include <QObject>

class MActionGroup: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MActionGroup)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

public:

    explicit MActionGroup(QObject *parent);
    ~MActionGroup();

    QAction *addAction(QAction *a);
    void removeAction(QAction *a);
    QList<QAction*> actions() const;

    void setVisible(bool);
    bool isVisible() const;

    int visibleItemCount();

Q_SIGNALS:

    void itemShown();
    void itemHidden();

private Q_SLOTS:

    void onItemChanged();

private:

    void setVisibleItemCount(int value);

    int readVisibleItemCount();

    QActionGroup group;

    int _visibleItemCount;

    bool internalUpdate;
};


class MRichTextEditPrivate : public MTextEditPrivate
{
    Q_DECLARE_PUBLIC(MRichTextEdit)

public:
    /*!
     * \brief constructor
     */
    MRichTextEditPrivate();
    /*!
     * \brief destructor
     */
    virtual ~MRichTextEditPrivate();

    /*!
     * \brief Initialize object
     */
    void init();

    /*!
     * \brief Return the mime information about the selection
     * The mime will contain both text and html formats
     * \return MimeData - Caller has to release memory when done
     */
    QMimeData *createMimeDataFromSelection();

    /*!
     * \brief Paste the mimedata to the selection.
     * Paste richtext if available, otherwise paste plain text.
     * \param source - Input mime data
     */
    bool insertFromMimeData(const QMimeData *source);

    /*!
     * \brief copy the text cursor's selection to the clipbaord.
     * \return True if operation compeled successfully.
     */
    virtual bool copy();

    /*!
     * \brief Update the style button(Bold, Italics, Underline) depending on
     * the current cursor position
     */
    void _q_updateStyle();

    /*!
     * \brief shows the text styling options
     */
    void _q_showTextStylingOptions();

    /*!
     * \brief gets the current text style values to be shown in the text styling ui
     */
    void  textStyleValues(QString *fontfamily, int *fontPointSize, QColor *fontColor);

    /*!
     * \brief removes line breakes in rich text document
     */
    QTextDocumentFragment replaceLineBreaks(const QTextDocumentFragment &fragment,const QString &replacement);

     /*!
     * \brief Set new font family
     * \param fontFamily - new font family name
     */
    void _q_setFontFamily(const QString &fontFamily);

    /*!
     * \brief Set new font size
     * \param fontPointSize - new font size
     */
    void _q_setFontSize(int fontPointSize);

    /*!
     * \brief Set new font color
     * \param fontColor - new font color
     */
    void _q_setFontColor(const QColor &fontColor);

    /*!
     * \brief Updates when MRichTextEdit buttons state
     */
    void _q_updateToolbarButtons();

    /*!
     * \brief Show RTE buttons on popup toolbar
     * \param show - true to show RTE buttons, false to hide them
     */
    void _q_showRichTextToolbar(bool show);

    /*!
     * \brief Toggle font Bold state of current cursor
     */
    void _q_toggleFontBold();

    /*!
     * \brief Toggle font Italic state of current cursor
     */
    void _q_toggleFontItalic();

    /*!
     * \brief Toggle font Underline state of current cursor
     */
    void _q_toggleFontUnderline();


private:

    // Cut, Copy and Paste buttons of MTextEdit
    MActionGroup textEditButtons;

    // B, F, U, F and T buttons of MRichTextEdit
    QActionGroup richTextEditButtons;

    // Bold button on toolbar
    QAction boldAction;

    // Italic button on toolbar
    QAction italicAction;

    // Underline button on toolbar
    QAction underlineAction;

    // Format button on toolbar(open color,size,face color dialog)
    QAction formatAction;

    // Switch mode button on toolbar
    QAction modeAction;
};

#endif
