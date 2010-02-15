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

#ifndef DUILABELMODEL_H
#define DUILABELMODEL_H

#include <duiwidgetmodel.h>
#include <QApplication>

class DuiLabelHighlighter;
typedef QList<DuiLabelHighlighter *> DuiLabelHighlighterList;

/*!
    \class DuiLabelModel
    \brief Model class for DuiLabel.

    \ingroup models
    \sa DuiLabel
*/
class DUI_EXPORT DuiLabelModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiLabelModel)

    /*!
        \property DuiLabelModel::text
        \brief The text of the label.
    */
    DUI_MODEL_PROPERTY(QString, text, Text, true, QString())

    /*!
        \property DuiLabelModel::wordWrap
        \brief Word wrapping mode of the label.

        When set to true, the label automatically wraps the text into multiple
        lines, if whole text does not fit into one line.
    */
    DUI_MODEL_PROPERTY(bool, wordWrap, WordWrap, true, false)

    /*!
        \property DuiLabelModel::alignment
        \brief Alignmentation of the label.
    */
    DUI_MODEL_PROPERTY(Qt::Alignment, alignment, Alignment, true, Qt::AlignLeft | Qt::AlignVCenter)

    /*!
        \property DuiLabelModel::textDirection
        \brief Text direction of the label.
    */
    DUI_MODEL_PROPERTY(Qt::LayoutDirection, textDirection, TextDirection, true, qApp->layoutDirection())

    /*!
        \property DuiLabelModel::textElide
        \brief Text eliding mode of the label.

        When set to true, the label automatically elides text with three dots if
        the whole text does not fit into one line.
    */
    DUI_MODEL_PROPERTY(bool, textElide, TextElide, true, false)

    /*!
        \property DuiLabelModel::useModelFont
        \brief Boolean value which defines whether to use the font from model or CSS.

        If true the font defined by DuiLabelModel::font property is used, if
        false font defined in CSS is used.
    */
    DUI_MODEL_PROPERTY(bool, useModelFont, UseModelFont, true, false)

    /*!
        \property DuiLabelModel::font
        \brief Default font for the label.

        Defines the default font to be used in label. This font is used only if
        DuiLabelModel::useModelFont is set to true otherwise the font defined
        in CSS is used.
    */
    DUI_MODEL_PROPERTY(QFont, font, Font, true, QFont())

    /*!
        \property DuiLabelModel::color
        \brief Default color for the label.

        Defines the default color to be used in label. This coor is used only if
        it has been set set to a valid value (color.isValid() == true) otherwise
        the color defined in CSS is used.
    */
    DUI_MODEL_PROPERTY(QColor, color, Color, true, QColor())

    /*!
        \property DuiLabelModel::highlighters
        \brief List of active text highlighter objects for the label.

        Highlighter objects are used for highlighting different types of text
        fragments like urls, email addresses etc. Objects also receive callbacks
        when the highlighted items are interacted by the user.
    */
    DUI_MODEL_PROPERTY(DuiLabelHighlighterList, highlighters, Highlighters, true, DuiLabelHighlighterList())

public:
    void emitLinkActivated(const QString &link) const {
        Q_EMIT linkActivated(link);
    }

    /*!
        \brief Add highlighter to list.
    */
    void addHighlighter(DuiLabelHighlighter *highlighter);

    /*!
        \brief Remove highlighter from list.
    */
    void removeHighlighter(DuiLabelHighlighter *highlighter);

Q_SIGNALS:
    /*!
     *  \brief A signal which is emitted when URL in this widget is clicked.
     */
    void linkActivated(const QString &link) const;
};

#endif

