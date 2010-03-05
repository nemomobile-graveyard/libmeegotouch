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

#ifndef DUILABEL_H
#define DUILABEL_H

#include <duiwidgetcontroller.h>
#include <duilabelmodel.h>

class DuiLabelPrivate;
class DuiLabelHighlighter;
class QString;

Q_DECLARE_METATYPE(QTextOption::WrapMode)

/*!
    \class DuiLabel
    \brief DuiLabel provides functionality for displaying text.

    \ingroup widgets

    \section DuiLabelOverview Overview
        DuiLabel provides functionality for displaying text. It can display
        plain text as well as rich text. The positioning of the text can be
        changed using setAlignment(). The appearance of the text can be tweaked
        using setTextDirection(), setWordWrap() and setTextElide().

        Following list summarizes the functionality of DuiLabel:

        - Text direction can be set.
        - Support for auto wrapping and manual line changes for rich text.
          Use "<br>" to do a manual line change.
        - Support for automatic text eliding. If the label is not fitting into
          the given area, the label is truncated, and elision marks "..." are
          added in the end of the label. For truncation, the label has to be at
          least 4 characters long.
        - Rich label supports HTML tags and CSS styling for text formatting
          (such as bold, color, font, italics). See <a href="http://doc.trolltech.com/richtext-html-subset.html">
          Supported HTML Subset</a> for more details.
        - Rich label supports highlighting and execution of links/anchors.
          linkActivated() signal is emitted when a link is clicked.

    \section DuiLabelUseGuidelines Usage guidelines
        - Simple label is used for plain text on one line such as device buttons.
        - Rich label is used when text styling and formatting are needed.
        - Application decides:
            - Position of the label in the view.
            - Content of the label.
            - Available area for the label.
            - Check that left alignment is OK for the your design, otherwise
              override the alignment. For instance in button the text needs to
              be center aligned.

    \section DuiLabelVariants Variants
        \li \link DuiLabelView Default view. \endlink Supports both simple and
            rich text.

    \section DuiLabelOpenIssues Open issues
        - requirements of localizations:
            - bi-directional languages - do we need to swap the label order in
              the same component?
            - bi-directional languages - aligning from left edge to right edge.
            - bi-directional languages - role of wrapping.
            - different label sub variations required by the localization - do
              we need to have two sub label types per language, one which is
              optimized for the space and another one that uses the whole
              available space.
            - Layouting in whole UI - to which extent our navigation and
              transitions do take into account the localization.

    \section DuiLabelExamples Examples
        Simple label:
        \code
            //simple label with text
            DuiLabel* label = new DuiLabel("Simple label");

            //set/change the text of label
            label->setText("Text");

        \endcode

        Rich label:
        \code
            //rich label with bold text.
            DuiLabel* label = new DuiLabel("Rich <b>bold</b> label");

            //manual line changes in rich label
            label->setText("row1<br><i>row2</i><br>row3");
        \endcode

        Rich label with custom text color:
        \code
            QString styledText = "<style>red{color:#FF0000;}</style> <red>Rich</red> label can be <red>styled</red>.";
            DuiLabel* label = new DuiLabel(styledText);
        \endcode

        Links in rich label:
        \code
            //create label with a link
            QString text = "Rich label can contain <a href=\"http://www.nokia.com\"> links </a>.";
            DuiLabel* label = new DuiLabel(styledText);

            //connect to signal to receive notification when user clicks a link in label
            connect(label, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));
        \endcode
    \sa DuiLabelModel DuiLabelStyle <a href="http://doc.trolltech.com/richtext-html-subset.html"> Supported HTML Subset</a>
*/
class DUI_EXPORT DuiLabel : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiLabel)

    /*!
        \property DuiLabel::alignment
        \brief Alignmentation of the label.

        See DuiLabelModel::alignment for details.
    */
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)

    /*!
        \property DuiLabel::wordWrap
        \brief Is word wrapping done or not
    */
    Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
        
    /*!
        \property DuiLabel::WrapMode
        \brief Word wrapping mode of the label.

        Used only if 'wordWrap' is true.
    */
    Q_PROPERTY(QTextOption::WrapMode wrapMode READ wrapMode WRITE setWrapMode)

    /*!
        \property DuiLabel::textElide
        \brief Text eliding mode of the label.

        See DuiLabelModel::textElide for details.
    */
    Q_PROPERTY(bool textElide READ textElide WRITE setTextElide)

    /*!
        \property DuiLabel::text
        \brief Text of the label.

        See DuiLabelModel::text for details.
    */
    Q_PROPERTY(QString text READ text WRITE setText)

    /*!
        \property DuiLabel::text
        \brief Color of the label text.

        See DuiLabelModel::color for details.
    */
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    
    /*!
        \brief Constructs label widget.
        \param parent optional parent.
        \param model optional model.
     */
    DuiLabel(QGraphicsItem *parent = 0, DuiLabelModel *model = 0);

    /*!
        \brief Constructs a label with a text.
        \param text Label text.
        \param parent Optional parent.
     */
    explicit DuiLabel(QString const &text, QGraphicsItem *parent = 0);

    /*!
        \brief Destructs label widget.
     */
    virtual ~DuiLabel();

    /*!
        \brief Set an alignmentation for the text.
        \sa Qt::Alignment
     */
    void setAlignment(Qt::Alignment alignment);

    /*!
        \brief Returns the current alignmentation.
        \return alignment
     */
    Qt::Alignment alignment() const;

    /*!
        \brief Returns wrapping mode for the text in the label.

        If the label doesn't have enough space to show the full text on one 
        line, the text will be wrapped to multiple lines if a mode with
        wrapping behavior is active.        

        \return wrap mode
     */
    
    QTextOption::WrapMode wrapMode() const;

    /*!
        \brief Set word wrapping mode.
        \sa wrapMode
    */
    void setWrapMode(QTextOption::WrapMode wrapMode);

    /*!
        \brief Enable/disable automatic word wrapping.

        If true, wrapping is done according to the mode set with setWrapMode.
     */
    void setWordWrap(bool wrap);

    /*!
        \brief Returns whether the text in label will be wrapped or not.

        If the label doesn't have enough space to show the full text on one
        line, the text will be wrapped to multiple lines.

        \return wrap mode
     */
    bool wordWrap() const;

    /*!
        Returns the current text.
        \return text
     */
    QString text() const;

    /*!
        \brief Enable/disable automatic text eliding.

        If the label doesn't have enough space to show the full text on one
        line, the will be elided with three dots.

        \sa Qt::TextElideMode
     */
    void setTextElide(bool elide);

    /*!
        \brief Returns the elide mode of the label.
        \return elide mode of the label

        \sa Qt::TextElideMode
     */
    bool textElide() const;

    /*!
        \brief Set the font used in DuiLabel.

        By default DuiLabel uses font defined in the css file. This method
        can be used to change the font from code.

        Overrides QGraphicsWidget::setFont(const QFont &font)
     */
    void setFont(const QFont &font);

    /*!
        \brief Return the current font.

        The current font is either a font previously set with setFont() or
        if nothing has been manually set the one that has been defined in css
        for this particular label.

        Overrides QGraphicsWidget::font() const.

        \return current font
     */
    QFont font() const;

    /*!
        \brief Set the text color for the DuiLabel.

        By default DuiLabel uses color defined in the css file. This method
        can be used to change the color from code. Giving invalid color
        (QColor()) takes font in the css back to use.
     */
    void setColor(const QColor &color);

    /*!
        \brief Returns the color previously set with setColor().

        \return current color
     */
    QColor color() const;

    /*!
        \brief Add highlighter object to duilabel.

        \sa DuiLabelHighlighter
    */
    void addHighlighter(DuiLabelHighlighter *highlighter);

    /*!
        \brief Remove highlighter object from duilabel.

        \sa DuiLabelHighlighter
    */
    void removeHighlighter(DuiLabelHighlighter *highlighter);

    /*!
        \brief Remove all highlighter objects from duilabel.

        \sa DuiLabelHighlighter
    */
    void removeAllHighlighters();

public Q_SLOTS:

    /*!
        \brief Set text for the label.
    */
    void setText(const QString &text);

Q_SIGNALS:
    /*!
        \brief A signal which is emitted when an anchor in the label is clicked.

        \a link contains the clicked link. The signal is only emitted when the link
        is defined using anchor "<a href=\"http://www.nokia.com\"> nokia </a>" html
        tag.

     */
    void linkActivated(const QString &link);

protected:

    //! \reimp
    virtual void changeEvent(QEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void setupModel();
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiLabel)
    Q_DECLARE_PRIVATE(DuiLabel)
};
#endif

