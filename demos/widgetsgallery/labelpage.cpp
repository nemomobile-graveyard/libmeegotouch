/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "labelpage.h"
#include "customselectablelabel.h"

#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MApplicationPage>
#include <MLinearLayoutPolicy>
#include <mdebug.h>

#include <mtextedit.h>
#include <mbutton.h>
#include <mlabelhighlighter.h>

LabelPage::LabelPage()
    : TemplatePage(TemplatePage::SimpleWidgets),
      simpleLabel(0),
      richLabel(0),
      ignoreClickAndLongPressCheckbox(0),
      ignoreClickAndLongPressLabel(0),
      selectableLabel(0),
      phoneHighlighter(0),
      urlHighlighter(0),
      emailHighlighter(0),
      commonHighlighter(0),
      selectableUrlHighlighter(0)
{
}

LabelPage::~LabelPage()
{
    delete phoneHighlighter;
    delete emailHighlighter;
    delete urlHighlighter;
    delete commonHighlighter;
    delete selectableUrlHighlighter;
}

QString LabelPage::timedemoTitle()
{
    return "Label";
}

void LabelPage::createContent()
{
    TemplatePage::createContent();

    simpleLabel = new MLabel();
    simpleLabel->setObjectName("simpleLabel");
    simpleLabel->setStyleName(inv("CommonBodyText"));
    simpleLabel->setAlignment(Qt::AlignVCenter);
    containerPolicy->addItem(simpleLabel);

    richLabel = new MLabel();
    richLabel->setObjectName("richLabel");
    richLabel->setStyleName(inv("CommonBodyText"));

    richLabel->setAlignment(Qt::AlignTop);
    richLabel->setWordWrap(true);
    containerPolicy->addItem(richLabel);

    ignoreClickAndLongPressLabel = new MLabel;
    ignoreClickAndLongPressLabel->setObjectName("ignoreClickAndLongPressLabel");
    ignoreClickAndLongPressLabel->setStyleName(inv("CommonBodyText"));
    ignoreClickAndLongPressLabel->setWordWrap(true);
    containerPolicy->addItem(ignoreClickAndLongPressLabel);

    ignoreClickAndLongPressCheckbox = new MButton;
    ignoreClickAndLongPressCheckbox->setObjectName("ignoreClickAndLongPressCheckbox");
    ignoreClickAndLongPressCheckbox->setStyleName(inv("CommonCheckBox"));
    ignoreClickAndLongPressCheckbox->setViewType(MButton::checkboxType);
    ignoreClickAndLongPressCheckbox->setCheckable(true);
    ignoreClickAndLongPressCheckbox->setChecked(true);
    containerPolicy->addItem(ignoreClickAndLongPressCheckbox);

    QRegExp phoneregexp("[\\+]{0,1}(\\d{8,13}|[\\(][\\+]{0,1}\\d{2,}[\\)]*\\d{5,13}|\\d{2,6}[\\-]{1}\\d{2,13}[\\-]*\\d{3,13})",
                        Qt::CaseInsensitive);
    QRegExp emailregexp("([0-9A-Z]([-\\.\\w]*[0-9A-Z])*@([0-9A-Z][-\\w]*[0-9A-Z]\\.)+[A-Z]{2,9})",
                        Qt::CaseInsensitive);
    QRegExp urlregexp("[-\\w\\.]+://([-\\w\\.]+)+(:\\d+)?(:\\w+)?(@\\d+)?(@\\w+)?([-\\w\\.]+)(/([\\w/_\\.]*(\\?\\S+)?)?)?",
                      Qt::CaseInsensitive);
//    QRegExp url("https?://([-\\w\\.]+)+(:\\d+)?(/([\\w/_\\.]*(\\?\\S+)?)?)?",
//                Qt::CaseInsensitive);


    MCommonLabelHighlighter *phone = new MCommonLabelHighlighter(phoneregexp);
    phone->setObjectName("phoneLabelHighlighter");
    MCommonLabelHighlighter *email = new MCommonLabelHighlighter(emailregexp);
    email->setObjectName("emailLabelHighlighter");
    MCommonLabelHighlighter *url = new MCommonLabelHighlighter(urlregexp);
    url->setObjectName("urlLabelHighlighter");
    MCommonLabelHighlighter *selectableUrl = new MCommonLabelHighlighter(urlregexp);
    selectableUrl->setObjectName("selectableUrlLabelHighlighter");
    selectableUrl->setIgnoreLongPressEvents(true);

    connect(phone, SIGNAL(longPressed(QString)), this, SLOT(phoneNumberLongPressed(QString)));
    connect(phone, SIGNAL(clicked(QString)), this, SLOT(phoneNumberClicked(QString)));

    connect(email, SIGNAL(longPressed(QString)), this, SLOT(emailLongPressed(QString)));
    connect(email, SIGNAL(clicked(QString)), this, SLOT(emailClicked(QString)));

    connect(url, SIGNAL(longPressed(QString)), this, SLOT(urlLongPressed(QString)));
    connect(url, SIGNAL(clicked(QString)), this, SLOT(urlClicked(QString)));

    connect(selectableUrl, SIGNAL(clicked(QString)), this, SLOT(urlClicked(QString)));

    connect(ignoreClickAndLongPressCheckbox, SIGNAL(toggled(bool)), this, SLOT(enableOrDisableClickAndLongPressHandling(bool)));

    phoneHighlighter = phone;
    emailHighlighter = email;
    urlHighlighter = url;
    selectableUrlHighlighter = selectableUrl;

    richLabel->addHighlighter(emailHighlighter);
    richLabel->addHighlighter(phoneHighlighter);
    richLabel->addHighlighter(urlHighlighter);

    selectableLabel = new CustomSelectableLabel;
    selectableLabel->setObjectName("customselectablelabel");
    selectableLabel->setStyleName(inv("CommonBodyText"));
    containerPolicy->addItem(selectableLabel);

    selectableLabel->addHighlighter(selectableUrlHighlighter);

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
    infoLabel->setText("<a></a>" + qtTrId("xx_label_page_info_label").replace('\n', "<br>"));
    //% "Simple Label"
    simpleLabel->setText(qtTrId("xx_simple_label"));
    //% "Rich Label <b>bold</b>, <i>italic</i>, "
    //% "http://www.nokia.com, +358401234567, fors.fagerstrom@email.com"
    richLabel->setText(qtTrId("xx_rich_label"));

    //% "Handle click and long press events:"
    ignoreClickAndLongPressLabel->setText(qtTrId("xx_label_page_handle_click_and_long_press_events"));

    //% "This is a custom label that combines performance and highlighters (like links: http://www.nokia.com) "
    //% "of <i>MLabel</i> and selection & copy support of <i>MTextEdit</i>.\n"
    //% "Selection is started by long tapping a word and the word will be preselected and selection adjustment handles and "
    //% "Copy-button appears."
    selectableLabel->setText(qtTrId("xx_label_page_selectablelabel"));

    update();
}

void LabelPage::phoneNumberLongPressed(const QString &link)
{
    mDebug("LabelPage::phoneNumberLongPressed()") << link;
}

void LabelPage::phoneNumberClicked(const QString &link)
{
    mDebug("LabelPage::phoneNumberClicked()") << link;
}

void LabelPage::emailLongPressed(const QString &link)
{
    mDebug("LabelPage::emailLongPressed()") << link;
}

void LabelPage::emailClicked(const QString &link)
{
    mDebug("LabelPage::emailClicked()") << link;
}

void LabelPage::urlLongPressed(const QString &link)
{
    mDebug("LabelPage::urlLongPressed()") << link;
}

void LabelPage::urlClicked(const QString &link)
{
    mDebug("LabelPage::urlClicked()") << link;
}

void LabelPage::enableOrDisableClickAndLongPressHandling(bool checked)
{
    phoneHighlighter->setIgnoreClickEvents(!checked);
    phoneHighlighter->setIgnoreLongPressEvents(!checked);

    emailHighlighter->setIgnoreClickEvents(!checked);
    emailHighlighter->setIgnoreLongPressEvents(!checked);

    urlHighlighter->setIgnoreClickEvents(!checked);
    urlHighlighter->setIgnoreLongPressEvents(!checked);

    selectableUrlHighlighter->setIgnoreClickEvents(!checked);
}
