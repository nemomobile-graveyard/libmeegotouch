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

class MDialog;
class MComboBox;

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
    void showTextStylingDialog(const QString &fontfamily = "");

Q_SIGNALS:

    /**
     *  \brief Emitted whenever a font family is selected
     */
    void fontFamilySelected(const QString &fontFamily);

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
     * \brief updates the ui components of the TextStyles dialog to show the current style
     */
    void setTextStyleValues(const QString &fontfamily);
    /*!
     * \brief launches the dialog of given type
     */
    void execDialog(ActiveDialog *activeDialog);

    static MRichTextEditDialogsManager *dialogsManager;

    struct Dialogs
    {
        ActiveDialog textStyles;
    };
    Dialogs dialogs;

    MComboBox *fontFamilyCombo;
};

#endif
