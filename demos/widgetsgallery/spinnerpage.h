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

#ifndef SPINNERPAGE_H
#define SPINNERPAGE_H

#include "templatepage.h"
#include <QTimer>

class MLabel;
class MContainer;
class MProgressIndicator;
class MButton;
class MFlowLayoutPolicy;

class SpinnerPage : public TemplatePage
{
    Q_OBJECT;

    enum View {
        Unknown,
        ApplicationMainArea,
        ContainerHeader,
        Dialog
    };

public:
    SpinnerPage();
    virtual ~SpinnerPage();
    virtual QString timedemoTitle();
    virtual void createContent();
public slots:
    void inApplicationMainArea();
    void inContainerHeader();
    void inDialog();
    void launchDialog();

    void timeout();
protected:
    virtual void retranslateUi();

private:
    void reset();

    MContainer *container;
    MProgressIndicator *spinner;
    MLayout *spinnerLayout1;
    MLayout *spinnerLayout2;
    MLabel *header;
    MLabel *description;
    View view;
    QTimer timer;
    MFlowLayoutPolicy *imageContainerPolicy;
    MAction *actionInMainArea;
    MAction *actionInContainerHeader;
    MAction *actionInDialog;
};

#endif // SPINNERPAGE_H
