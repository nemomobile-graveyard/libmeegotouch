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

#ifndef MLABELMODEL_H
#define MLABELMODEL_H

#include <QApplication>
#include <QList>
#include <QTextOption>
#include <mwidgetmodel.h>
#include <mnamespace.h>

class MLabelHighlighter;

/*!
    \class MLabelModel
    \brief Model class for MLabel.

    \ingroup models
    \sa MLabel
*/
class M_CORE_EXPORT MLabelModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MLabelModel)

    /*!
        \property MLabelModel::text
        \brief The text of the label.
    */
    M_MODEL_PROPERTY(QString, text, Text, true, QString())

    /*!
        \property MLabelModel::wordWrap
        \brief Is Word wrapping active.

        When set to true, the label wraps the text into multiple 
        lines. This is done depending on which wrap mode is active.
    */

    M_MODEL_PROPERTY(bool, wordWrap, WordWrap, true, false)

    /*!
      \property MLabelModel::wrapMode
      \brief If 'wordwrap' is set, describes how text is wrapped in the label.
    */
    M_MODEL_PROPERTY(QTextOption::WrapMode, wrapMode, WrapMode, true, QTextOption::WordWrap)

    /*!
        \property MLabelModel::alignment
        \brief Alignmentation of the label.
    */
    M_MODEL_PROPERTY(Qt::Alignment, alignment, Alignment, true, Qt::AlignLeft | Qt::AlignVCenter)

    /*!
      \property MLabelModel::alignmentFromStyle
      \brief Alignment of the label is done by style.
    */
    M_MODEL_PROPERTY(bool, alignmentFromStyle, AlignmentFromStyle, true, true)

    /*!
        \property MLabelModel::textDirection
        \brief Text direction of the label.

        The default text direction is autodetected from the text contents
    */
    M_MODEL_PROPERTY(Qt::LayoutDirection, textDirection, TextDirection, true, Qt::LayoutDirectionAuto)

    /*!
        \property MLabelModel::textElide
        \brief Text eliding mode of the label.

        When set to true, the label automatically elides text with three dots if
        the whole text does not fit into one line.
    */
    M_MODEL_PROPERTY(bool, textElide, TextElide, true, false)

    /*!
        \property MLabelModel::useModelFont
        \brief Boolean value which defines whether to use the font from model or CSS.

        If true the font defined by MLabelModel::font property is used, if
        false font defined in CSS is used.
    */
    M_MODEL_PROPERTY(bool, useModelFont, UseModelFont, true, false)

    /*!
        \property MLabelModel::font
        \brief Default font for the label.

        Defines the default font to be used in label. This font is used only if
        MLabelModel::useModelFont is set to true otherwise the font defined
        in CSS is used.
    */
    M_MODEL_PROPERTY(QFont, font, Font, true, QFont())

    /*!
        \property MLabelModel::color
        \brief Default color for the label.

        Defines the default color to be used in label. This coor is used only if
        it has been set set to a valid value (color.isValid() == true) otherwise
        the color defined in CSS is used.
    */
    M_MODEL_PROPERTY(QColor, color, Color, true, QColor())

    /*!
        \property MLabelModel::highlighters
        \brief List of active text highlighter objects for the label.

        Highlighter objects are used for highlighting different types of text
        fragments like urls, email addresses etc. Objects also receive callbacks
        when the highlighted items are interacted by the user.
    */
    M_MODEL_PROPERTY(QList<MLabelHighlighter *>, highlighters, Highlighters, true, QList<MLabelHighlighter *>())

    M_MODEL_PROPERTY(Qt::TextFormat, textFormat, TextFormat, true, Qt::AutoText)

public:
    void emitLinkActivated(const QString &link) const {
        Q_EMIT linkActivated(link);
    }

    /*!
        \brief Add highlighter to list.
    */
    void addHighlighter(MLabelHighlighter *highlighter);

    /*!
        \brief Remove highlighter from list.
    */
    void removeHighlighter(MLabelHighlighter *highlighter);

Q_SIGNALS:
    /*!
     *  \brief A signal which is emitted when URL in this widget is clicked.
     */
    void linkActivated(const QString &link) const;
};

#endif

