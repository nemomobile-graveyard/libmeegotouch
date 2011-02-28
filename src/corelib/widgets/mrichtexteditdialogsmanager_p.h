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

#ifndef MRICHTEXTEDITDIALOGSMANAGER_P_H
#define MRICHTEXTEDITDIALOGSMANAGER_P_H

#include <QPointer>
#include <QPair>
#include <MNamespace>
#include <MStylableWidget>

class MDialog;
class MComboBox;
class MColorComboBox;
class MBasicListItem;


class MRichTextEditDialogsManager : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief returns the MRichTextEditDialogsManager instance
     */
    static MRichTextEditDialogsManager *instance();
    /*!
     * \brief shows the text styling dialog with the current style
     */
    void showTextStylingDialog(const QString &fontfamily = "", int fontPointSize = -1,
                               const QColor &fontColor = QColor());

Q_SIGNALS:

    /**
     *  \brief Emitted whenever a font family is selected
     */
    void fontFamilySelected(const QString &fontFamily);

    /**
     *  \brief Emitted whenever a font size is selected
     */
    void fontSizeSelected(int fontPointSize);

    /**
     *  \brief Emitted whenever a font color is selected
     */
    void fontColorSelected(const QColor &color);

private Q_SLOTS:

    void setFontSize();

private:
    // QPointer<MDialog> in ActiveDialog will hold a dialog
    // bool in ActiveDialog is used to avoid calling its dialog's exec() when it is active
    typedef QPair<QPointer<MDialog>, bool> ActiveDialog;

    /*!
     * \brief private constructor to make the class singleton
     */
    MRichTextEditDialogsManager();
    /*!
     * \brief private destructor to avoid deleting the singleton object
     */
    virtual ~MRichTextEditDialogsManager();
    /*!
     * \brief initializes the TextStyles dialog
     */
    void initTextStylingDialog();
    /*!
     * \brief initializes the FontColor dialog
     */
    void initFontColorDialog();
    /*!
     * \brief updates the ui components of the TextStyles dialog to show the current style
     */
    void setTextStyleValues(const QString &fontfamily, int fontPointSize, const QColor &fontColor);
    /*!
     * \brief launches the dialog of given type
     */
    void execDialog(ActiveDialog *activeDialog);
    /*!
     * \brief updates font size list
     */
    QStringList generateFontSizeComboValues() const;

    static MRichTextEditDialogsManager *dialogsManager;

    struct Dialogs
    {
        ActiveDialog textStyles;
        ActiveDialog fontColor;
    };
    Dialogs dialogs;

    QList<int> sizeValues;
    MComboBox *fontFamilyCombo;
    MComboBox *fontSizeCombo;
    MColorComboBox *fontColorCombo;
};

#endif
