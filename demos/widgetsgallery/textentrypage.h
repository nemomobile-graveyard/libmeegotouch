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

class MWidget;
class MLabel;
class MButton;
class MTextEdit;
class MCompleter;

class MGridLayoutPolicy;
class CustomDirectIMWidget;

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
    MGridLayoutPolicy *createAndSetupGridPolicy(QGraphicsWidget *panel);

public slots:
    //! switch Autocapitalisation on/off in every MTextEdit
    void changeAutoCapitalisation(bool val);

    //! switch error correction on/off in every MTextEdit
    void changeErrorCorrection(bool val);

protected:
    virtual void retranslateUi();

private:
    MLabel *label0;
    MLabel *label1;
    MLabel *label2;
    MLabel *label3;
    MLabel *label4;
    MLabel *label5;
    MLabel *label6;
    MLabel *label7;
    MLabel *label8;
    MLabel *labelNoEcho;
    MLabel *labelEchoOnEdit;
    MLabel *labelDirectIM;

    MLabel *labelHeader1;
    MButton *button1;
    MButton *button2;

    QList<MLabel *> labels;
    QList<MTextEdit *> Entries;

    bool m_autoCapitalisation;
    bool m_errorCorrection;

    MCompleter *m_completer;
    CustomDirectIMWidget *directIMWidget;
};

#endif
