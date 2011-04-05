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

#ifndef TEXTENTRYPAGE_H
#define TEXTENTRYPAGE_H

#include <QList>

#include "templatepage.h"
#include <MTextEdit>
#include <QPointer>

class MWidget;
class MLabel;
class MButton;
class MCompleter;

class MGridLayoutPolicy;
class CustomDirectIMWidget;

class CustomTextEdit: public MTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(const QString &text = QString(), QGraphicsItem *parent = 0);
public slots:
    void changeLabel();
    void changeButton();
protected:
    bool event(QEvent *event);
};


/** /brief Page that shows the features of the virtual keyboard and the MtextEdit
*/
class TextEntryPage : public TemplatePage
{
    Q_OBJECT

public:
    TextEntryPage();
    virtual ~TextEntryPage();
    virtual QString timedemoTitle();

    virtual void createContent();
    MLinearLayoutPolicy *createAndSetupLinearPolicy(QGraphicsWidget *panel);

public slots:
    //! switch Autocapitalisation on/off in every MTextEdit
    void changeAutoCapitalisation(bool val);

    //! switch error correction on/off in every MTextEdit
    void changeErrorCorrection(bool val);

    //! Check if the email text in errorHighlightingTextEdit is valid.
    //! If not, set error highlight
    void checkEmailValidity();

protected:
    virtual void retranslateUi();

private:
    QPointer<MLabel> pageTitleLabel;
    QPointer<MLabel> label0;
    QPointer<MLabel> label1;
    QPointer<MLabel> label2;
    QPointer<MLabel> label3;
    QPointer<MLabel> label4;
    QPointer<MLabel> label5;
    QPointer<MLabel> label6;
    QPointer<MLabel> label7;
    QPointer<MLabel> label8;
    QPointer<MLabel> label9;
    QPointer<MLabel> label10;
    QPointer<MLabel> labelNoEcho;
    QPointer<MLabel> labelEchoOnEdit;
    QPointer<MLabel> labelDirectIM;
    QPointer<MLabel> labelCustomToolbar1;
    QPointer<MLabel> labelCustomToolbar2;
    QPointer<MLabel> labelErrorHighlighting;
    QPointer<MLabel> uneditableTextEditLabel;

    QPointer<MLabel> labelHeader1;
    QPointer<MButton> button1;
    QPointer<MButton> button2;
    QPointer<MTextEdit> errorHighlightingTextEdit;

    QList<MLabel *> labels;
    QList<MTextEdit *> Entries;

    bool m_autoCapitalisation;
    bool m_errorCorrection;

    QScopedPointer<MCompleter> m_completer;
    QPointer<CustomDirectIMWidget> directIMWidget;
};

#endif
