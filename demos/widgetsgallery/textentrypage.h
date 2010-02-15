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

#ifndef TEXTENTRYPAGE_H
#define TEXTENTRYPAGE_H

#include <QList>

#include "templatepage.h"

class DuiWidget;
class DuiLabel;
class DuiButton;
class DuiTextEdit;
class DuiCompleter;

class DuiGridLayoutPolicy;
class CustomDirectIMWidget;

/** /brief Page that shows the features of the virtual keyboard and the DuitextEdit
*/
class TextEntryPage : public TemplatePage
{
    Q_OBJECT

public:
    TextEntryPage();
    virtual ~TextEntryPage();

    virtual void createContent();
    DuiGridLayoutPolicy *createAndSetupGridPolicy(DuiWidget *panel);

public slots:
    //! switch Autocapitalisation on/off in every DuiTextEdit
    void changeAutoCapitalisation(bool val);

    //! switch error correction on/off in every DuiTextEdit
    void changeErrorCorrection(bool val);

protected:
    virtual void retranslateUi();

private:
    DuiLabel *label0;
    DuiLabel *label1;
    DuiLabel *label2;
    DuiLabel *label3;
    DuiLabel *label4;
    DuiLabel *label5;
    DuiLabel *label6;
    DuiLabel *label7;
    DuiLabel *label8;
    DuiLabel *labelNoEcho;
    DuiLabel *labelEchoOnEdit;
    DuiLabel *labelDirectIM;

    DuiLabel *labelHeader1;
    DuiButton *button1;
    DuiButton *button2;

    QList<DuiLabel *> labels;
    QList<DuiTextEdit *> Entries;

    bool m_autoCapitalisation;
    bool m_errorCorrection;

    DuiCompleter *m_completer;
    CustomDirectIMWidget *directIMWidget;
};

#endif
