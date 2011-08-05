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

#include "languagepage.h"

#include <MLayout>
#include <MLocale>
#include <MPopupList>
#include <MLabel>
#include <MGridLayoutPolicy>
#include <MButton>
#include <MComboBox>
#include <MTheme>
#include <MPannableViewport>
#ifdef HAVE_GCONF
#include <MGConfItem>
#endif
#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#ifdef HAVE_ICU
#include <unicode/uloc.h>
#endif

LanguagePage::LanguagePage()
    : TemplatePage(TemplatePage::ApplicationView),
      comboBoxLanguage(0),
      modelLanguage(0),
      comboBoxLcTime(0),
      modelLcTime(0),
      comboBoxLcTimeFormat24h(0),
      modelLcTimeFormat24h(0),
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
      labelExampleWeekNumber(0),
      labelExampleCurrency(0),
      labelExampleTranslation1(0),
      labelExampleTranslation2(0),
      labelExampleTranslation3(0),
      labelLtrTest(0),
      labelRtlTest(0),
      labelLtrTestRich(0),
      labelRtlTestRich(0),
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
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    setStyleName(inv("CommonApplicationPage"));

    QGraphicsWidget *panel = centralWidget();
    panel->setObjectName("panel");
    MLayout *layout = new MLayout(panel);

    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);

    comboBoxLanguage = new MComboBox;
    comboBoxLanguage->setObjectName("comboBoxLanguage");
    comboBoxLanguage->setStyleName(inv("CommonComboBox"));
    policy->addItem(comboBoxLanguage, 1, 1);

    comboBoxLcTime = new MComboBox;
    comboBoxLcTime->setObjectName("comboBoxLcTime");
    comboBoxLcTime->setStyleName(inv("CommonComboBox"));

    comboBoxLcTimeFormat24h = new MComboBox;
    comboBoxLcTimeFormat24h->setObjectName("comboBoxLcTimeFormat24h");
    comboBoxLcTimeFormat24h->setStyleName(inv("CommonComboBox"));

    comboBoxLcCollate = new MComboBox;
    comboBoxLcCollate->setObjectName("comboBoxLcCollate");
    comboBoxLcCollate->setStyleName(inv("CommonComboBox"));

    comboBoxLcNumeric = new MComboBox;
    comboBoxLcNumeric->setObjectName("comboBoxLcNumeric");
    comboBoxLcNumeric->setStyleName(inv("CommonComboBox"));

    comboBoxLcMonetary = new MComboBox;
    comboBoxLcMonetary->setObjectName("comboBoxLcMonetary");
    comboBoxLcMonetary->setStyleName(inv("CommonComboBox"));

    labelHaveGconf = new MLabel;
    labelHaveGconf->setObjectName("labelHaveGconf");
    labelHaveGconf->setStyleName(inv("CommonBodyText"));
    labelHaveGconf->setWordWrap(true);

    labelHaveIcu = new MLabel;
    labelHaveIcu->setObjectName("labelHaveIcu");
    labelHaveIcu->setStyleName(inv("CommonBodyText"));
    labelHaveIcu->setWordWrap(true);

    labelExampleNumber = new MLabel;
    labelExampleNumber->setObjectName("labelExampleNumber");
    labelExampleNumber->setStyleName(inv("CommonBodyText"));
    labelExampleNumber->setWordWrap(true);

    labelExampleDateTime = new MLabel;
    labelExampleDateTime->setObjectName("labelExampleDateTime");
    labelExampleDateTime->setStyleName(inv("CommonBodyText"));
    labelExampleDateTime->setWordWrap(true);

    labelExampleWeekNumber = new MLabel;
    labelExampleWeekNumber->setObjectName("labelExampleWeekNumber");
    labelExampleWeekNumber->setStyleName(inv("CommonBodyText"));
    labelExampleWeekNumber->setWordWrap(true);

    labelExampleCurrency = new MLabel;
    labelExampleCurrency->setObjectName("labelExampleCurrency");
    labelExampleCurrency->setStyleName(inv("CommonBodyText"));
    labelExampleCurrency->setWordWrap(true);

    labelExampleTranslation1 = new MLabel;
    labelExampleTranslation1->setObjectName("labelExampleTranslation1");
    labelExampleTranslation1->setStyleName(inv("CommonBodyText"));
    labelExampleTranslation1->setWordWrap(true);

    labelExampleTranslation2 = new MLabel;
    labelExampleTranslation2->setObjectName("labelExampleTranslation2");
    labelExampleTranslation2->setStyleName(inv("CommonBodyText"));
    labelExampleTranslation2->setWordWrap(true);

    labelExampleTranslation3 = new MLabel;
    labelExampleTranslation3->setObjectName("labelExampleTranslation3");
    labelExampleTranslation3->setStyleName(inv("CommonBodyText"));
    labelExampleTranslation3->setWordWrap(true);

    labelLtrTest = new MLabel;
    labelLtrTest->setObjectName("labelLtrTest");
    labelLtrTest->setStyleName(inv("CommonBodyText"));
    labelLtrTest->setWordWrap(true);

    labelRtlTest = new MLabel;
    labelRtlTest->setObjectName("labelRtlTest");
    labelRtlTest->setStyleName(inv("CommonBodyText"));
    labelRtlTest->setWordWrap(true);

    labelLtrTestRich = new MLabel;
    labelLtrTestRich->setObjectName("labelLtrTestRich");
    labelLtrTestRich->setStyleName(inv("CommonBodyText"));
    labelLtrTestRich->setWordWrap(true);

    labelRtlTestRich = new MLabel;
    labelRtlTestRich->setObjectName("labelRtlTestRich");
    labelRtlTestRich->setStyleName(inv("CommonBodyText"));
    labelRtlTestRich->setWordWrap(true);

    labelFontTest = new MLabel;
    labelFontTest->setObjectName("labelFontTest");
    labelFontTest->setStyleName(inv("CommonBodyText"));
    labelFontTest->setWordWrap(true);
    labelFontTest->setWordWrap(true);

    policy->addItem(comboBoxLcTime, 2, 1);
    policy->addItem(comboBoxLcTimeFormat24h, 3, 1);
    policy->addItem(comboBoxLcCollate, 4, 1);
    policy->addItem(comboBoxLcNumeric, 5, 1);
    policy->addItem(comboBoxLcMonetary, 6, 1);
    policy->addItem(labelHaveGconf, 7, 1);
    policy->addItem(labelHaveIcu, 8, 1);
    policy->addItem(labelExampleNumber, 9, 1);
    policy->addItem(labelExampleDateTime, 10, 1);
    policy->addItem(labelExampleWeekNumber, 11, 1);
    policy->addItem(labelExampleCurrency, 12, 1);
    policy->addItem(labelExampleTranslation1, 13, 1);
    policy->addItem(labelExampleTranslation2, 14, 1);
    policy->addItem(labelExampleTranslation3, 15, 1);
    policy->addItem(labelLtrTest, 16, 1);
    policy->addItem(labelRtlTest, 17, 1);
    policy->addItem(labelLtrTestRich, 18, 1);
    policy->addItem(labelRtlTestRich, 19, 1);
    policy->addItem(labelFontTest, 20, 1);

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
    disconnect(comboBoxLcTimeFormat24h, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcTimeFormat24h(int)));
    disconnect(comboBoxLcCollate, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcCollate(int)));
    disconnect(comboBoxLcNumeric, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcNumeric(int)));
    disconnect(comboBoxLcMonetary, SIGNAL(currentIndexChanged(int)),
               this, SLOT(changeLcMonetary(int)));

    QStringList localeNames;
#ifdef HAVE_ICU
    int numberOfAvailableLocales = uloc_countAvailable();
    for (int i = 0; i < numberOfAvailableLocales; ++i)
        localeNames << QString::fromUtf8(uloc_getAvailable(i));
    localeNames << "no";
    localeNames << "no_NO";
    localeNames << "es_419";
    localeNames << "de@collation=phonebook";
    localeNames << "de_DE@collation=phonebook";
    localeNames << "zh_Hans_CN@collation=pinyin";
    localeNames << "zh_Hans_CN@collation=stroke";
    QStringList localeNamesWithoutScript;
    QRegExp regexp("^([a-z]{2,3}_)([A-Z][a-z]{3,3}_)(.*)$");
    foreach (const QString &localeName, localeNames) {
        if (regexp.indexIn(localeName) == 0) {
            localeNamesWithoutScript <<
                regexp.capturedTexts().at(1) + regexp.capturedTexts().at(3);
        }
    }
    foreach (const QString &localeName, localeNamesWithoutScript)
        localeNames << localeName;
#else
    for (unsigned language = QLocale::C;
         language <= QLocale::LastLanguage;
         language++) {
        foreach (const QLocale::Country &country,
                 QLocale::countriesForLanguage (QLocale::Language(language))) {
            localeNames << QLocale(QLocale::Language(language), country).name();
        }
    }
#endif
    localeNames.sort();

    QList<QStringList> rowsLocale;
    rowsLocale
        << (QStringList()
            //% "None"
            << QString::fromUtf8("“”: ") + qtTrId("xx_locale_none")
            << "");
    foreach (const QString &localeName, localeNames) {
        QString localeNameDescription;
        MLocale locale(localeName);
        QString directionMarker;
        if (locale.textDirection() == Qt::RightToLeft)
            directionMarker = QChar(0x200F); // U+200F RIGHT-TO-LEFT MARK
        else
            directionMarker = QChar(0x200E); // U+200E LEFT-TO-RIGHT MARK
        QString languageEndonym = locale.languageEndonym();
        if (!languageEndonym.isEmpty())
            languageEndonym[0] = languageEndonym.at(0).toUpper();
        localeNameDescription +=
            localeName + ": "
            + directionMarker
            + languageEndonym
            + directionMarker;
        if (!locale.countryEndonym().isEmpty())
            localeNameDescription +=
                directionMarker
                + " (" + locale.countryEndonym() + ')'
                + directionMarker;
        rowsLocale
            << (QStringList()
                << localeNameDescription
                << localeName);
    }

    int rowsLocaleCount = rowsLocale.count();

    QList<QStringList> rowsTimeFormat24h;
    rowsTimeFormat24h
        << (QStringList()
            //% "Locale default"
            << qtTrId("xx_timeformat24h_locale_default")
            << "")
        << (QStringList()
            //% "24 hour mode"
            << qtTrId("xx_timeformat24h_24_hour_mode")
            << "24")
        << (QStringList()
            //% "12 hour mode"
            << qtTrId("xx_timeformat24h_12_hour_mode")
            << "12");

    int rowsTimeFormat24hCount = rowsTimeFormat24h.count();

    MLocale currentLocale;

    QStandardItemModel *newModelLanguage
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    newModelLanguage->setObjectName("newModelLanguage");
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
    newModelLcTime->setObjectName("newModelLcTime");
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

    QStandardItemModel *newModelLcTimeFormat24h
        = new QStandardItemModel(rowsTimeFormat24h.count(), 2, this);
    newModelLcTimeFormat24h->setObjectName("newModelLcTimeFormat24h");
    Q_CHECK_PTR(newModelLcTimeFormat24h);
    for (int i = 0; i < rowsTimeFormat24hCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModelLcTimeFormat24h->setItem(i, j, new QStandardItem(rowsTimeFormat24h[i][j]));

    comboBoxLcTimeFormat24h->setItemModel(newModelLcTimeFormat24h);
    if (modelLcTimeFormat24h && modelLcTimeFormat24h->QObject::parent() == this)
        delete modelLcTimeFormat24h;
    modelLcTimeFormat24h = newModelLcTimeFormat24h;
    MLocale::TimeFormat24h currentLcTimeFormat24h = currentLocale.timeFormat24h();
    QString currentLcTimeFormat24hString;
    switch(currentLcTimeFormat24h) {
    case MLocale::TwentyFourHourTimeFormat24h:
        currentLcTimeFormat24hString = "24";
        break;
    case MLocale::TwelveHourTimeFormat24h:
        currentLcTimeFormat24hString = "12";
        break;
    case MLocale::LocaleDefaultTimeFormat24h:
        currentLcTimeFormat24hString = "";
        break;
    default:
        currentLcTimeFormat24hString = "";
        break;
    }
    for (int i = 0; i < rowsTimeFormat24hCount; ++i) {
        if (currentLcTimeFormat24hString == modelLcTimeFormat24h->item(i, 1)->text())
            comboBoxLcTimeFormat24h->setCurrentIndex(i);
    }

    QStandardItemModel *newModelLcCollate
        = new QStandardItemModel(rowsLocale.count(), 2, this);
    newModelLcCollate->setObjectName("newModelLcCollate");
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
    newModelLcNumeric->setObjectName("newModelLcNumeric");
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
    newModelLcMonetary->setObjectName("newModelLcMonetary");
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
    //% "Choose 12/24 hour mode"
    comboBoxLcTimeFormat24h->setTitle(qtTrId("xx_language_combobox_lctimeformat24h_title"));
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

    //% "Number format example: %1\x9c!! Number example: %1\x9c!! Number: %1"
    labelExampleNumber->setText(
        qtTrId("xx_language_number_format")
        .arg(currentLocale.formatNumber(12345670.89)));

    QDate date(2008, 7, 21);
    QTime time(14, 31, 0, 0);
    QDateTime datetime(date, time, Qt::UTC);
    QString formattedDateTime =
        currentLocale.formatDateTime(
            datetime, MLocale::DateFull, MLocale::TimeFull,
            MLocale::GregorianCalendar);
    if (!formattedDateTime.isEmpty())
        formattedDateTime[0] = formattedDateTime.at(0).toUpper();

    //% "Date and time format example: %1\x9c!! Date and time: %1\x9c!! Date: %1\x9c!! %1"
    labelExampleDateTime->setText(
        qtTrId("xx_langage_date_time_format")
        .arg(formattedDateTime));

    //% "Week number example: %1"
    labelExampleWeekNumber->setText(
        qtTrId("xx_language_week_number")
        .arg(
#ifdef HAVE_ICU
            currentLocale.formatDateTime(datetime, "%%U %U %%v %v %%V %V")
#else
            datetime.date().weekNumber()
#endif
            ));

    //% "Currency format example: %1 %2\x9c!! Currency: %1 %2"
    labelExampleCurrency->setText(
        qtTrId("xx_language_currency_format")
        .arg(currentLocale.formatCurrency(1234.56, "GBP"))
        .arg(currentLocale.formatCurrency(1234.56, "EUR")));

    //% "%Ln boxes with a total weight of %L1 kilogram\x9c!! %Ln boxes, total weight of %L1 kg"
    labelExampleTranslation1->setText(
        qtTrId("xx_language_trans_%Ln_boxes_weight_%L1", 0).arg(0));
    labelExampleTranslation2->setText(
        qtTrId("xx_language_trans_%Ln_boxes_weight_%L1", 1).arg(2431.78));
    labelExampleTranslation3->setText(
        qtTrId("xx_language_trans_%Ln_boxes_weight_%L1", 2).arg(7891.32));

    // Some simple test labels to check wether alignment and bidi
    // reordering of RTL and LTR text in labels works correctly.
    // To test Arabic, the labels are set to English first, then
    // a processEvents() is executed, then to Arabic, and vice versa.
    // This processEvents() is inserted between a change of the
    // contents from RTL to LTR because this did cause breakage
    // and I want to make sure that this is fixed and remains so.
    labelLtrTest->setText(QString::fromUtf8("العربية"));
    qApp->processEvents();
    labelLtrTest->setText("LTR text (next line is RTL text)");

    labelRtlTest->setText("English");
    qApp->processEvents();
    labelRtlTest->setText(QString::fromUtf8("العربية (مصر)"));

    labelLtrTestRich->setText(QString::fromUtf8("<b>العربية</b>"));
    qApp->processEvents();
    labelLtrTestRich->setText("<b>LTR rich text (next line is RTL rich text)</b>");

    labelRtlTestRich->setText("<b>English</b>");
    qApp->processEvents();
    labelRtlTestRich->setText(QString::fromUtf8("<b>العربية (مصر)</b>"));

    //% "Font test:"
    labelFontTest->setText(
        qtTrId("xx_language_font_test")
        + QString::fromUtf8("<br>Russian: русский язык")
        + QString::fromUtf8("<br>Kazakh: Қазақ ₸")
        + QString::fromUtf8("<br>Arabic: العربية")
        + QString::fromUtf8("<br>Urdu: اُردوُ")
        + QString::fromUtf8("<br>Simplified Chinese: 简体字")
        + QString::fromUtf8("<br>Traditional Chinese: 簡體字")
        + QString::fromUtf8("<br>Thai: ภาษาไทย")
        + QString::fromUtf8("<br>Hindi: हिन्दी, हिंदी ₨ ₹")
        + QString::fromUtf8("<br>Hebrew: עִבְרִית")
        + QString::fromUtf8("<br>Greek: ελληνικά")
        + QString::fromUtf8("<br>Japanese: 日本語")
        + QString::fromUtf8("<br>Symbols: ⇥‟↵➡➨➯➔➞➽➹✩✿❥›⌘‽☁")
        );

    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLanguage(int)));
    connect(comboBoxLcTime, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLcTime(int)));
    connect(comboBoxLcTimeFormat24h, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeLcTimeFormat24h(int)));
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

void LanguagePage::changeLcTimeFormat24h(int index)
{
    if (index < 0 || index >= modelLcTimeFormat24h->rowCount())
        return;
    QString newLcTimeFormat24hString = modelLcTimeFormat24h->item(index, 1)->text();
#ifdef HAVE_GCONF
    MGConfItem lcTimeFormat24hItem("/meegotouch/i18n/lc_timeformat24h");
    if (newLcTimeFormat24hString != lcTimeFormat24hItem.value().toString()) {
        lcTimeFormat24hItem.set(newLcTimeFormat24hString);
    }
#else
    MLocale::TimeFormat24h newLcTimeFormat24h;
    if (newLcTimeFormat24hString == "24")
        newLcTimeFormat24h = MLocale::TwentyFourHourTimeFormat24h;
    else if (newLcTimeFormat24hString == "12")
        newLcTimeFormat24h = MLocale::TwelveHourTimeFormat24h;
    else
        newLcTimeFormat24h = MLocale::LocaleDefaultTimeFormat24h;
    MLocale currentLocale;
    if (newLcTimeFormat24h != currentLocale.timeFormat24h()) {
        currentLocale.setTimeFormat24h(newLcTimeFormat24h);
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
