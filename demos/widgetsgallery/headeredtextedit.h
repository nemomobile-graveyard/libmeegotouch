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


#ifndef HEADEREDTEXTEDIT_H
#define HEADEREDTEXTEDIT_H

#include <MTextEdit>
#include <MLabel>
#include <QTextCursor>

class MLabel;

/*!
 *\brief: Multiline TextInput widget with label inside. 
 * Layout of the widget would be as follows:
 *  ______________________
 * |SomeLabel:            |
 *  ______________________
 * 
 * Accepts all scripts, as its a M::FreeTextContentType .
 */

class HeaderedTextEdit : public MTextEdit
{
    Q_OBJECT

public:
    /*! 
     *\brief: our explicit constructor.
     */
    explicit HeaderedTextEdit(QGraphicsItem *parent = 0);
    virtual ~HeaderedTextEdit();

public:
    /*!
     *\brief: Public method to set label text
     */
    void setHeaderText(const QString &text);
   
    /*!
     *\brief: Returns label text. In case if label text is 
     * not set, returns an empty QString.
     */ 
    QString staticHeaderText() const;

protected Q_SLOTS:

    /*!
     *\brief: protected slot. Called after creating label and 
     * setting up its text.
     * It sets the cursor at an indented position equal to the width
     * of label size.
     */
    void _q_updateTextLeftMargin();

    /*!
     *\brief: Protected slot is called, whenever a user presses return key.
     * It removes the indentation which was set in first block only and only 
     * if its a new block other wise sets the indented block again (means using
     * backspace to go to the first block.)
     */
    void _q_resetNewBlockMargin();
    
private:
    /*!
     *\brief: Label inside text input
     */
    MLabel *headerLabel;
};


#endif
