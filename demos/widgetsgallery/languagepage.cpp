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

#include "languagepage.h"

#include <MLayout>
#include <MLocale>
#include <MPopupList>
#include <MLabel>
#include <MGridLayoutPolicy>
#include <MButton>
#include <MComboBox>
#include <MTheme>
#ifdef HAVE_GCONF
#include <MGConfItem>
#endif
#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QStringList>
#include <QDebug>
#include <QDateTime>

LanguagePage::LanguagePage()
    : TemplatePage(TemplatePage::ApplicationView),
    comboBoxLanguage(0),
      modelLanguage(0),
      comboBoxLcTime(0),
      modelLcTime(0),
      comboBoxLcCollate(0),
      modelLcCollate(0),
      comboBoxLcNumeric(0),
      modelLcNumeric(0),
      comboBoxLcMonetary(0),
      modelLcMonetary(0),
      labelHaveGconf(0),
      labelHaveIcu(0),
      labelExampleNumber(0),
      labelExampleDateTime(0),
      labelExampleCurrency(0),
      labelExampleTranslation1(0),
      labelExampleTranslation2(0),
      labelExampleTranslation3(0),
      labelFontTest(0)
{
}

LanguagePage::~LanguagePage()
{
}

QString LanguagePage::timedemoTitle()
{
    return "Language";
}

void LanguagePage::createContent()
{
    MApplicationPage::createContent();
    QGraphicsWidget *panel = centralWidget();
    MLayout *layout = new MLayout(panel);
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    comboBoxLanguage = new MComboBox;
    policy->addItem(comboBoxLanguage, 1, 1);
    comboBoxLcTime = new MComboBox;
    comboBoxLcCollate = new MComboBox;
    comboBoxLcNumeric = new MComboBox;
    comboBoxLcMonetary = new MComboBox;
    labelHaveGconf = new MLabel;
    labelHaveIcu = new MLabel;
    labelExampleNumber = new MLabel;
    labelExampleDateTime = new MLabel;
    labelExampleCurrency = new MLabel;
    labelExampleTranslation1 = new MLabel;
    labelExampleTranslation2 = new MLabel;
    labelExampleTranslation3 = new MLabel;
    labelFontTest = new MLabel;
    labelFontTest->setWordWrap(true);

    policy->addItem(comboBoxLcTime, 2, 1);
    policy->addItem(comboBoxLcCollate, 3, 1);
    policy->addItem(comboBoxLcNumeric, 4, 1);
    policy->addItem(comboBoxLcMonetary, 5, 1);
    policy->addItem(labelHaveGconf, 6, 1);
    policy->addItem(labelHaveIcu, 7, 1);
    policy->addItem(labelExampleNumber, 8, 1);
    policy->addItem(labelExampleDateTime, 9, 1);
    policy->addItem(labelExampleCurrency, 10, 1);
    policy->addItem(labelExampleTranslation1, 11, 1);
    policy->addItem(labelExampleTranslation2, 12, 1);
    policy->addItem(labelExampleTranslation3, 13, 1);
    policy->addItem(labelFontTest, 14, 1);

    retranslateUi();
}

void LanguagePage::retranslateUi()
{
    //% "Change Language"
    setTitle(qtTrId("xx_language_title"));
    if (!isContentCreated())
        return;

    disconnect(comboBoxLanguage, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLanguage(int)));
    disconnect(comboBoxLcTime, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcTime(int)));
    disconnect(comboBoxLcCollate, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcCollate(int)));
    disconnect(comboBoxLcNumeric, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcNumeric(int)));
    disconnect(comboBoxLcMonetary, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcMonetary(int)));

    QList<QStringList> rowsLocale;
    rowsLocale
        << (QStringList()
            //% "None"
            << qtTrId("xx_locale_none")
            + QString::fromUtf8(" “”")
            << "")
        << (QStringList()
            //% "Arabic"
            << qtTrId("xx_locale_ar")
            + QString::fromUtf8(" “ar”")
            << "ar")
        << (QStringList()
            //% "German"
            << qtTrId("xx_locale_de")
            + QString::fromUtf8(" “de”")
            << "de")
        << (QStringList()
            //% "German (Austria)"
            << qtTrId("xx_locale_de_AT")
            + QString::fromUtf8(" “de_AT”")
            << "de_AT")
        << (QStringList()
            //% "German (Switzerland)"
            << qtTrId("xx_locale_de_CH")
            + QString::fromUtf8(" “de_CH”")
            << "de_CH")
        << (QStringList()
        //% "English"
            << qtTrId("xx_locale_en")
            + QString::fromUtf8(" “en”")
            << "en")
        << (QStringList()
        //% "Finnish"
            << qtTrId("xx_locale_fi")
            + QString::fromUtf8(" “fi”")
            << "fi")
        << (QStringList()
        //% "Hindi"
            << qtTrId("xx_locale_hi")
            + QString::fromUtf8(" “hi”")
            << "hi")
        << (QStringList()
        //% "Hungarian"
            << qtTrId("xx_locale_hu")
            + QString::fromUtf8(" “hu”")
            << "hu")
        << (QStringList()
        //% "Japanese"
            << qtTrId("xx_locale_ja")
            + QString::fromUtf8(" “ja”")
            << "ja")
        << (QStringList()
        //% "Urdu"
            << qtTrId("xx_locale_ur")
            + QString::fromUtf8(" “ur”")
            << "ur")
        << (QStringList()
        //% "Urdu (Pakistan)"
            << qtTrId("xx_locale_ur_PK")
            + QString::fromUtf8(" “ur_PK”")
            << "ur_PK")
        << (QStringList()
        //% "Urdu (India)"
            << qtTrId("xx_locale_ur_IN")
            + QString::fromUtf8(" “ur_IN”")
            << "ur_IN")
        << (QStringList()
        //% "Chinese (China)"
            << qtTrId("xx_locale_zh_CN")
            + QString::fromUtf8(" “zh_CN”")
            << "zh_CN")
        << (QStringList()
        //% "Chinese (Taiwan)"
            << qtTrId("xx_locale_zh_TW")
            + QString::fromUtf8(" “zh_TW”")
            << "zh_TW");

    int rowsLocaleCount = rowsLocale.count();

    MLocale currentLocale;

    QStandardItemModel *newModelLanguage
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    Q_CHECK_PTR(newModelLanguage);
    for (int i = 0; i < rowsLocaleCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModelLanguage->setItem(i, j, new QStandardItem(rowsLocale[i][j]));

    comboBoxLanguage->setItemModel(newModelLanguage);
    if (modelLanguage && modelLanguage->QObject::parent() == this)
        delete modelLanguage;
    modelLanguage = newModelLanguage;
    QString currentLanguage = currentLocale.name();
    rowsLocaleCount = rowsLocale.count();
    for (int i = 0; i < rowsLocaleCount; ++i) {
        if (currentLanguage == modelLanguage->item(i, 1)->text())
            comboBoxLanguage->setCurrentIndex(i);
    }

    QStandardItemModel *newModelLcTime
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    Q_CHECK_PTR(newModelLcTime);
    for (int i = 0; i < rowsLocaleCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModelLcTime->setItem(i, j, new QStandardItem(rowsLocale[i][j]));

    comboBoxLcTime->setItemModel(newModelLcTime);
    if (modelLcTime && modelLcTime->QObject::parent() == this)
        delete modelLcTime;
    modelLcTime = newModelLcTime;
    QString currentLcTime = currentLocale.categoryName(MLocale::MLcTime);
    for (int i = 0; i < rowsLocaleCount; ++i) {
        if (currentLcTime == modelLcTime->item(i, 1)->text())
            comboBoxLcTime->setCurrentIndex(i);
    }

    QStandardItemModel *newModelLcCollate
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    Q_CHECK_PTR(newModelLcCollate);
    for (int i = 0; i < rowsLocaleCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModelLcCollate->setItem(i, j, new QStandardItem(rowsLocale[i][j]));

    comboBoxLcCollate->setItemModel(newModelLcCollate);
    if (modelLcCollate && modelLcCollate->QObject::parent() == this)
        delete modelLcCollate;
    modelLcCollate = newModelLcCollate;
    QString currentLcCollate = currentLocale.categoryName(MLocale::MLcCollate);
    for (int i = 0; i < rowsLocaleCount; ++i) {
        if (currentLcCollate == modelLcCollate->item(i, 1)->text())
            comboBoxLcCollate->setCurrentIndex(i);
    }

    QStandardItemModel *newModelLcNumeric
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    Q_CHECK_PTR(newModelLcNumeric);
    for (int i = 0; i < rowsLocaleCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModelLcNumeric->setItem(i, j, new QStandardItem(rowsLocale[i][j]));

    comboBoxLcNumeric->setItemModel(newModelLcNumeric);
    if (modelLcNumeric && modelLcNumeric->QObject::parent() == this)
        delete modelLcNumeric;
    modelLcNumeric = newModelLcNumeric;
    QString currentLcNumeric = currentLocale.categoryName(MLocale::MLcNumeric);
    for (int i = 0; i < rowsLocaleCount; ++i) {
        if (currentLcNumeric == modelLcNumeric->item(i, 1)->text())
            comboBoxLcNumeric->setCurrentIndex(i);
    }

    QStandardItemModel *newModelLcMonetary
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    Q_CHECK_PTR(newModelLcMonetary);
    for (int i = 0; i < rowsLocaleCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModelLcMonetary->setItem(i, j, new QStandardItem(rowsLocale[i][j]));

    comboBoxLcMonetary->setItemModel(newModelLcMonetary);
    if (modelLcMonetary && modelLcMonetary->QObject::parent() == this)
        delete modelLcMonetary;
    modelLcMonetary = newModelLcMonetary;
    QString currentLcMonetary = currentLocale.categoryName(MLocale::MLcMonetary);
    for (int i = 0; i < rowsLocaleCount; ++i) {
        if (currentLcMonetary == modelLcMonetary->item(i, 1)->text())
            comboBoxLcMonetary->setCurrentIndex(i);
    }

    //% "Please select language"
    comboBoxLanguage->setTitle(qtTrId("xx_language_combobox_title"));
    //% "Locale for time"
    comboBoxLcTime->setTitle(qtTrId("xx_language_combobox_lctime_title"));
    //% "Locale for collation"
    comboBoxLcCollate->setTitle(qtTrId("xx_language_combobox_lccollate_title"));
    //% "Locale for numbers"
    comboBoxLcNumeric->setTitle(qtTrId("xx_language_combobox_lcnumeric_title"));
    //% "Locale for money"
    comboBoxLcMonetary->setTitle(qtTrId("xx_language_combobox_lcmonetary_title"));

#ifdef HAVE_GCONF
    //% "Yes"
    labelHaveGconf->setText("Gconf: " + qtTrId("qtn_comm_command_yes"));
#else
    //% "No"
    labelHaveGconf->setText("Gconf: " + qtTrId("qtn_comm_command_no"));
#endif
#ifdef HAVE_ICU
    //% "Yes"
    labelHaveIcu->setText("ICU: " + qtTrId("qtn_comm_command_yes"));
#else
    //% "No"
    labelHaveIcu->setText("ICU: " + qtTrId("qtn_comm_command_no"));
#endif

    QDate date(2008, 7, 21);
    QTime time(14, 31, 0, 0);
    QDateTime datetime(date, time, Qt::UTC);

    //% "Number format example: %1\x9c!! Number example: %1\x9c!! Number: %1"
    labelExampleNumber->setText(
        qtTrId("xx_language_number_format")
        .arg(currentLocale.formatNumber(12345670.89)));
    //% "Date and time format example: %1\x9c!! Date and time: %1\x9c!! Date: %1\x9c!! %1"
    labelExampleDateTime->setText(
        qtTrId("xx_langage_date_time_format")
        .arg(currentLocale.formatDateTime(
                 datetime, MLocale::DateFull, MLocale::TimeFull,
                 MLocale::GregorianCalendar)));
    //% "Currency format example: %1 %2\x9c!! Currency: %1 %2"
    labelExampleCurrency->setText(
        qtTrId("xx_language_currency_format")
        .arg(currentLocale.formatCurrency(1234.56, "GBP"))
        .arg(currentLocale.formatCurrency(1234.56, "EUR")));

    //% "%Ln boxes with a total weight of %L1 kilogram\x9c!! %Ln boxes, total weight of %L1 kg"
    labelExampleTranslation1->setText(
        qtTrId("xx_language_trans_%Ln_boxes_weight_%L1", 0).arg(1234.56));
    labelExampleTranslation2->setText(
        qtTrId("xx_language_trans_%Ln_boxes_weight_%L1", 1).arg(2431.78));
    labelExampleTranslation3->setText(
        qtTrId("xx_language_trans_%Ln_boxes_weight_%L1", 2).arg(7891.32));

    //% "Font test:"
    labelFontTest->setText(
        qtTrId("xx_language_font_test")
        + QString::fromUtf8("<br>Russian: русский язык")
        + QString::fromUtf8("<br>Arabic: العربية")
        + QString::fromUtf8("<br>Urdu: اُردوُ")
        + QString::fromUtf8("<br>Simplified Chinese: 简体字")
        + QString::fromUtf8("<br>Traditional Chinese: 簡體字")
        + QString::fromUtf8("<br>Thai: ภาษาไทย")
        + QString::fromUtf8("<br>Hindi: हिन्दी, हिंदी")
        + QString::fromUtf8("<br>Hebrew: עִבְרִית")
        + QString::fromUtf8("<br>Greek: ελληνικά")
        + QString::fromUtf8("<br>Japanese: 日本語")
        + QString::fromUtf8("<br>Symbols: ⇥‟↵➡➨➯➔➞➽➹✩✿❥›⌘‽☁")
        );

    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLanguage(int)));
    connect(comboBoxLcTime, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLcTime(int)));
    connect(comboBoxLcCollate, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLcCollate(int)));
    connect(comboBoxLcNumeric, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLcNumeric(int)));
    connect(comboBoxLcMonetary, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLcMonetary(int)));
}

void LanguagePage::changeLanguage(int index)
{
    if (index < 0 || index >= modelLanguage->rowCount())
        return;
    QString newLanguage = modelLanguage->item(index, 1)->text();
#ifdef HAVE_GCONF
    MGConfItem languageItem("/meegotouch/i18n/language");
    if (newLanguage != languageItem.value().toString()) {
        languageItem.set(newLanguage);
    }
#else
    MLocale oldLocale;
    if (newLanguage != oldLocale.name()) {
        MLocale newLocale(newLanguage);
        MLocale::setDefault(newLocale);
    }
#endif
}

void LanguagePage::changeLcTime(int index)
{
    if (index < 0 || index >= modelLcTime->rowCount())
        return;
    QString newLcTime = modelLcTime->item(index, 1)->text();
#ifdef HAVE_GCONF
    MGConfItem lcTimeItem("/meegotouch/i18n/lc_time");
    if (newLcTime != lcTimeItem.value().toString()) {
        lcTimeItem.set(newLcTime);
    }
#else
    MLocale currentLocale;
    if (newLcTime != currentLocale.categoryName(MLocale::MLcTime)) {
        currentLocale.setCategoryLocale(MLocale::MLcTime, newLcTime);
        MLocale::setDefault(currentLocale);
    }
#endif
}

void LanguagePage::changeLcCollate(int index)
{
    if (index < 0 || index >= modelLcCollate->rowCount())
        return;
    QString newLcCollate = modelLcCollate->item(index, 1)->text();
#ifdef HAVE_GCONF
    MGConfItem lcCollateItem("/meegotouch/i18n/lc_collate");
    if (newLcCollate != lcCollateItem.value().toString()) {
        lcCollateItem.set(newLcCollate);
    }
#else
    MLocale currentLocale;
    if (newLcCollate != currentLocale.categoryName(MLocale::MLcCollate)) {
        currentLocale.setCategoryLocale(MLocale::MLcCollate, newLcCollate);
        MLocale::setDefault(currentLocale);
    }
#endif
}

void LanguagePage::changeLcNumeric(int index)
{
    if (index < 0 || index >= modelLcNumeric->rowCount())
        return;
    QString newLcNumeric = modelLcNumeric->item(index, 1)->text();
#ifdef HAVE_GCONF
    MGConfItem lcNumericItem("/meegotouch/i18n/lc_numeric");
    if (newLcNumeric != lcNumericItem.value().toString()) {
        lcNumericItem.set(newLcNumeric);
    }
#else
    MLocale currentLocale;
    if (newLcNumeric != currentLocale.categoryName(MLocale::MLcNumeric)) {
        currentLocale.setCategoryLocale(MLocale::MLcNumeric, newLcNumeric);
        MLocale::setDefault(currentLocale);
    }
#endif
}

void LanguagePage::changeLcMonetary(int index)
{
    if (index < 0 || index >= modelLcMonetary->rowCount())
        return;
    QString newLcMonetary = modelLcMonetary->item(index, 1)->text();
#ifdef HAVE_GCONF
    MGConfItem lcMonetaryItem("/meegotouch/i18n/lc_monetary");
    if (newLcMonetary != lcMonetaryItem.value().toString()) {
        lcMonetaryItem.set(newLcMonetary);
    }
#else
    MLocale currentLocale;
    if (newLcMonetary != currentLocale.categoryName(MLocale::MLcMonetary)) {
        currentLocale.setCategoryLocale(MLocale::MLcMonetary, newLcMonetary);
        MLocale::setDefault(currentLocale);
    }
#endif
}
