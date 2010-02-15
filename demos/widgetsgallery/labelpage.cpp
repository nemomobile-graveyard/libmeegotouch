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

#include "labelpage.h"

#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiApplicationPage>
#include <DuiLinearLayoutPolicy>
#include <duidebug.h>

#include <duitextedit.h>
#include <duibutton.h>
#include <duilabelhighlighter.h>

LabelPage::LabelPage()
    : TemplatePage(),
      simpleLabel(0),
      richLabel(0)
{
    gid = TemplatePage::LayoutsAndVisuals;

    phoneHighlighter = NULL;
    emailHighlighter = NULL;
    urlHighlighter = NULL;
    commonHighlighter = NULL;
}

LabelPage::~LabelPage()
{
    delete phoneHighlighter;
    delete emailHighlighter;
    delete urlHighlighter;
    delete commonHighlighter;
}

void LabelPage::createContent()
{
    TemplatePage::createContent();

    simpleLabel = new DuiLabel();
    simpleLabel->setAlignment(Qt::AlignVCenter);
    containerPolicy->addItem(simpleLabel);

    richLabel = new DuiLabel();

    richLabel->setAlignment(Qt::AlignTop);
    richLabel->setWordWrap(true);
    containerPolicy->addItem(richLabel);

    textEdit = new DuiTextEdit(DuiTextEditModel::MultiLine);
    containerPolicy->addItem(textEdit);

    DuiButton *b = new DuiButton("Highlight");
    containerPolicy->addItem(b);

    connect(b, SIGNAL(clicked()), this, SLOT(applySearch()));


    QRegExp phoneregexp("[\\+]{0,1}(\\d{8,13}|[\\(][\\+]{0,1}\\d{2,}[\\)]*\\d{5,13}|\\d{2,6}[\\-]{1}\\d{2,13}[\\-]*\\d{3,13})",
                        Qt::CaseInsensitive);
    QRegExp emailregexp("([0-9A-Z]([-\\.\\w]*[0-9A-Z])*@([0-9A-Z][-\\w]*[0-9A-Z]\\.)+[A-Z]{2,9})",
                        Qt::CaseInsensitive);
    QRegExp urlregexp("[-\\w\\.]+://([-\\w\\.]+)+(:\\d+)?(:\\w+)?(@\\d+)?(@\\w+)?([-\\w\\.]+)(/([\\w/_\\.]*(\\?\\S+)?)?)?",
                      Qt::CaseInsensitive);
//    QRegExp url("https?://([-\\w\\.]+)+(:\\d+)?(/([\\w/_\\.]*(\\?\\S+)?)?)?",
//                Qt::CaseInsensitive);


    DuiCommonLabelHighlighter *phone = new DuiCommonLabelHighlighter(phoneregexp);
    DuiCommonLabelHighlighter *email = new DuiCommonLabelHighlighter(emailregexp);
    DuiCommonLabelHighlighter *url = new DuiCommonLabelHighlighter(urlregexp);

    connect(phone, SIGNAL(longPressed(QString)), this, SLOT(phoneNumberLongPressed(QString)));
    connect(phone, SIGNAL(clicked(QString)), this, SLOT(phoneNumberClicked(QString)));

    connect(email, SIGNAL(longPressed(QString)), this, SLOT(emailLongPressed(QString)));
    connect(email, SIGNAL(clicked(QString)), this, SLOT(emailClicked(QString)));

    connect(url, SIGNAL(longPressed(QString)), this, SLOT(urlLongPressed(QString)));
    connect(url, SIGNAL(clicked(QString)), this, SLOT(urlClicked(QString)));

    phoneHighlighter = phone;
    emailHighlighter = email;
    urlHighlighter = url;

    richLabel->addHighlighter(emailHighlighter);
    richLabel->addHighlighter(phoneHighlighter);
    richLabel->addHighlighter(urlHighlighter);

    retranslateUi();
}

void LabelPage::retranslateUi()
{
    //% "Labels"
    setTitle(qtTrId("xx_label_page_title"));
    if (!isContentCreated())
        return;
    //% "Simple Label is used mainly by other components, and it does "
    //% "not support any formatting or interaction.\n\n"
    //% "Rich Label supports multiple lines, HTML formatting and "
    //% "highlighting for links.\n\n"
    //% "All Labels can be aligned left, right or center and will be "
    //% "automatically truncated if the text does not fit inside "
    //% "the given area."
    infoLabel->setText("<a></a>" + qtTrId("xx_label_page_info_label"));
    //% "Simple Label"
    simpleLabel->setText(qtTrId("xx_simple_label"));
    //% "Rich Label <b>bold</b>, <i>italic</i>, "
    //% "http://www.nokia.com, +358401234567, fors.fagerstrom@email.com"
    richLabel->setText(qtTrId("xx_rich_label"));

    update();
}

void LabelPage::phoneNumberLongPressed(const QString &link)
{
    duiDebug("LabelPage::phoneNumberLongPressed()") << link;
}

void LabelPage::phoneNumberClicked(const QString &link)
{
    duiDebug("LabelPage::phoneNumberClicked()") << link;
}

void LabelPage::emailLongPressed(const QString &link)
{
    duiDebug("LabelPage::emailLongPressed()") << link;
}

void LabelPage::emailClicked(const QString &link)
{
    duiDebug("LabelPage::emailClicked()") << link;
}

void LabelPage::urlLongPressed(const QString &link)
{
    duiDebug("LabelPage::urlLongPressed()") << link;
}

void LabelPage::urlClicked(const QString &link)
{
    duiDebug("LabelPage::urlClicked()") << link;
}


void LabelPage::applySearch()
{
    duiDebug("LabelPage::applySearch()") << textEdit->text();

    richLabel->removeHighlighter(commonHighlighter);
    delete commonHighlighter;

    commonHighlighter = new DuiCommonLabelHighlighter(QRegExp(textEdit->text()));

    richLabel->addHighlighter(commonHighlighter);

}
