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

#include "mlocale.h"
#include "mlocale_p.h"

#ifdef HAVE_ICU
#include <unicode/unistr.h>
#include <unicode/ucal.h>
#include <unicode/coll.h>
#include <unicode/fieldpos.h>
#include <unicode/datefmt.h>
#include <unicode/calendar.h>
#include <unicode/smpdtfmt.h> // SimpleDateFormat
#include <unicode/numfmt.h>
#include <unicode/uloc.h>
#include <unicode/dtfmtsym.h> // date format symbols
#include <unicode/putil.h> // u_setDataDirectory
#endif

#include <MDebug>
#include <QTranslator>
#include <QDir>
#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <QPointer>
#include <MApplication>

#ifdef HAVE_ICU
#include "mcollator.h"
#include "mcalendar.h"
#include "mcalendar_p.h"
#include "micuconversions.h"
#endif

static QPointer<QTranslator> s_ltrTranslator = 0;
static QPointer<QTranslator> s_rtlTranslator = 0;

namespace
{
    const char *const BackupNameFormatString = "%d%t%g%t%m%t%f";
    const QString RtlLanguages("ar:fa:he:ps:ur:");
    const char *const Languages = "Languages";
    const char *const Countries = "Countries";

    const QString SettingsLanguage("/meegotouch/i18n/language");
    const QString SettingsLcTime("/meegotouch/i18n/lc_time");
    const QString SettingsLcTimeFormat24h("/meegotouch/i18n/lc_timeformat24h");
    const QString SettingsLcCollate("/meegotouch/i18n/lc_collate");
    const QString SettingsLcNumeric("/meegotouch/i18n/lc_numeric");
    const QString SettingsLcMonetary("/meegotouch/i18n/lc_monetary");
    const QString SettingsLcTelephone("/meegotouch/i18n/lc_telephone");
}

/// Helper
// Copied from Qt's QCoreApplication
static void replacePercentN(QString *result, int n)
{
    if (n >= 0) {
        int percentPos = 0;
        int len = 0;
        while ((percentPos = result->indexOf(QLatin1Char('%'), percentPos + len)) != -1) {
            len = 1;
            //TODO replace fmt to other type to do our own native digit conversions
            QString fmt;
            if (result->at(percentPos + len) == QLatin1Char('L')) {
                ++len;
                fmt = QLatin1String("%L1");
            } else {
                fmt = QLatin1String("%1");
            }
            if (result->at(percentPos + len) == QLatin1Char('n')) {
                fmt = fmt.arg(n);
                ++len;
                result->replace(percentPos, len, fmt);
                len = fmt.length();
            }
        }
    }
}


/////////////////////////////////////////////////
//// The private Mtranslationcatalog class ////

//! \internal
class MTranslationCatalog: public QSharedData
{
public:

    MTranslationCatalog(const QString &name);
    virtual ~MTranslationCatalog();

    // called by detach
    MTranslationCatalog(const MTranslationCatalog &other);

    /*!
    * \brief Load the actual translation file using locale and category info
    *
    * As an example lets assume that
    *
    * - MLocale::translationPaths()
    *   is the list ("/usr/share/l10n/meegotouch", "/usr/share/l10n")
    * - the category is  MLocale::MLcMessages
    * - the name of the locale (returned by mlocale->categoryName(category))
    *   is "en_US"
    * - the base name of the translation file is "foo"
    *
    * then the function will try to load translation catalogs in the following order:
    *
    *   /usr/share/l10n/meegotouch/foo_en_US.qm
    *   /usr/share/l10n/meegotouch/foo_en_US
    *   /usr/share/l10n/meegotouch/foo_en.qm
    *   /usr/share/l10n/meegotouch/foo_en
    *   /usr/share/l10n/foo_en_US.qm
    *   /usr/share/l10n/foo_en_US
    *   /usr/share/l10n/foo_en.qm
    *   /usr/share/l10n/foo_en
    *
    * and return when the first translation catalog was found.
    * If no translation can be found this function returns false.
    *
    * The way locale specific parts are successively cut away from the
    * translation file name is inherited from
    * <a href="http://qt.nokia.com/doc/4.6/qtranslator.html#load">QTranslator::load()</a>
    * because this is used to load the translation files.
    *
    * @param mlocale the locale for which the translations are loaded
    * @param category  this is usually MLocale::MLcMessages but it could
    *                  also be MLocale::MLcTime, MLocale::MLcNumeric,
    *                  etc...
    *
    * @return true if translations could be found, false if not.
    */
    bool loadWith(MLocale *mlocale, MLocale::Category category);

    // the abstract name for a translation. together with locale info and
    // category a concrete path is created when the file is loaded
    QString _name;

    // the actual translator
    QTranslator _translator;

private:
    MTranslationCatalog &operator=(const MTranslationCatalog &other);
};
//! \internal_end

// //////
// MTranslationCatalog implementation

MTranslationCatalog::MTranslationCatalog(const QString &name)
    : _name(name), _translator()
{
    // nothing
}

MTranslationCatalog::~MTranslationCatalog()
{
    // nothing
}

MTranslationCatalog::MTranslationCatalog(const MTranslationCatalog &other)
    : QSharedData(), _name(other._name)
{
    // nothing
}

bool MTranslationCatalog::loadWith(MLocale *mlocale, MLocale::Category category)
{
    QStringList localeDirs;
    QString fname;
    if (QFileInfo(_name).isRelative()) {
        localeDirs = MLocale::translationPaths();
        fname = _name;
    }
    else {
        localeDirs = (QStringList() << QFileInfo(_name).path());
        fname = QFileInfo(_name).fileName();
    }

    const int size = localeDirs.size();
    for (int i = 0; i < size; ++i) {
        QString prefix = QDir(localeDirs.at(i)).absolutePath();
        if (prefix.length() && !prefix.endsWith(QLatin1Char('/')))
            prefix += QLatin1Char('/');
        QString realname;

        if (fname.endsWith(QLatin1String(".qm"))) {
            // this is either engineering English or the locale
            // specific parts of the file name have been fully
            // specified already. We don’t want any fallbacks in that
            // case, we try to load only the exact file name:
            realname = prefix + fname;
            if(QFileInfo(realname).isReadable() && _translator.load(realname))
                return true;
        }
        else {
            QString delims("_.@");
            QString engineeringEnglishName = fname;
            fname += '_' + mlocale->categoryName(category);
            for (;;) {
                realname = prefix + fname + ".qm";
                if (QFileInfo(realname).isReadable() && _translator.load(realname))
                    return true;
                realname = prefix + fname;
                if (QFileInfo(realname).isReadable() && _translator.load(realname))
                    return true;

                int rightmost = 0;
                for (int i = 0; i < (int)delims.length(); i++) {
                    int k = fname.lastIndexOf(delims[i]);
                    if (k > rightmost)
                        rightmost = k;
                }

                // no truncations?
                if (rightmost == 0)
                    break;

                fname.truncate(rightmost);

                // do not fall back to engineering English when trying
                // to load real translations:
                if (fname == engineeringEnglishName)
                     break;
            }
        }
    }
    // Loading the new file into the QTranslator failed.
    // Clear any old contents of the QTranslator before returning false.
    // This is necessary because the QTranslator may still have old contents.
    // For example, assume that an Arabic translation "foo_ar.qm" has been loaded
    // into the translator before and now this loadWith() function tries to
    // load "foo_de.qm" because the language has been switched to German
    // but "foo_de.qm" does not exist. We do *not* want to keep the previous
    // "foo_ar.qm" contents in that case.
    _translator.load("", 0);
    return false;
}

////////////////////////////////
//// Private stuff for MLocale

QStringList MLocalePrivate::translationPaths;
QStringList MLocalePrivate::dataPaths;

#ifdef HAVE_ICU
bool MLocalePrivate::truncateLocaleName(QString *localeName)
{
    // according to http://userguide.icu-project.org/locale the separators
    // that specify the parts of a locale are "_", "@", and ";", e.g.
    // in sr_Latn_RS_REVISED@currency=USD;calendar=islamic-civil
    // so we remove them from the end of the locale string.

    int semicolonIndex = localeName->lastIndexOf(';');
    if (semicolonIndex != -1)
    {
        // found semicolon, remove it and remaining part of string
        localeName->truncate(semicolonIndex);
        return true;
    }
    else
    {
        int atIndex = localeName->lastIndexOf('@');
        if (atIndex != -1)
        {
            // found "@", remove it and remaining part of string
            localeName->truncate(atIndex);
            return true;
        }
        else
        {
            int underscoreIndex = localeName->lastIndexOf('_');
            if (underscoreIndex != -1)
            {
                // found "_", remove it and remaining part of string
                localeName->truncate(underscoreIndex);
                return true;
            }
        }
    }
    // no truncation possible
    return false;
}
#endif

#ifdef HAVE_ICU
icu::DateFormatSymbols *MLocalePrivate::createDateFormatSymbols(const icu::Locale &locale)
{
    // This is a bit dirty but the only way to currently get the symbols
    // is like this. Only the internal API supports directly creating DateFormatSymbols
    // with an arbitrary calendar
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat dummyFormatter("", locale, status);

    if (U_FAILURE(status)) {
        return 0;
    }

    const DateFormatSymbols *dfs = dummyFormatter.getDateFormatSymbols();
    return new DateFormatSymbols(*dfs);
}
#endif

#ifdef HAVE_ICU
bool MLocalePrivate::isTwelveHours(const QString &icuFormatQString) const
{
    if (icuFormatQString.contains('\'')) {
        bool isQuoted = false;
        for (int i = 0; i < icuFormatQString.size(); ++i) {
            if (icuFormatQString[i] == '\'')
                isQuoted = !isQuoted;
            if (!isQuoted && icuFormatQString[i] == 'a')
                return true;
        }
        return false;
    }
    else {
        if(icuFormatQString.contains('a'))
            return true;
        else
            return false;
    }
}
#endif

#ifdef HAVE_ICU
void MLocalePrivate::dateFormatTo24h(icu::DateFormat *df) const
{
    if (df) {
        icu::UnicodeString icuFormatString;
        QString icuFormatQString;
        static_cast<SimpleDateFormat *>(df)->toPattern(icuFormatString);
        icuFormatQString = MIcuConversions::unicodeStringToQString(icuFormatString);
        if (isTwelveHours(icuFormatQString)) {
            // remove unquoted 'a' characters and remove space left of 'a'
            // and change unquoted h -> H and K -> k
            QString tmp;
            bool isQuoted = false;
            for (int i = 0; i < icuFormatQString.size(); ++i) {
                QChar c = icuFormatQString[i];
                if (c == '\'')
                    isQuoted = !isQuoted;
                if (!isQuoted) {
                    if (c == 'h')
                        tmp.append("H");
                    else if (c == 'K')
                        tmp.append("k");
                    else if (c == 'a') {
                        if (tmp.endsWith(' ')) {
                            // remove space before 'a' if character
                            // after 'a' is space as well:
                            if (i < icuFormatQString.size() - 1
                                && icuFormatQString[i+1] == ' ')
                                tmp.remove(tmp.size()-1,1);
                            // remove space before 'a' if 'a' is last
                            // character in string:
                            if (i == icuFormatQString.size() - 1)
                                tmp.remove(tmp.size()-1,1);
                        }
                    }
                    else
                        tmp.append(c);
                }
                else {
                    tmp.append(c);
                }
            }
            icuFormatQString = tmp;
        }
        icuFormatString = MIcuConversions::qStringToUnicodeString(icuFormatQString);
        static_cast<SimpleDateFormat *>(df)->applyPattern(icuFormatString);
    }
}
#endif

#ifdef HAVE_ICU
void MLocalePrivate::dateFormatTo12h(icu::DateFormat *df) const
{
    if (df) {
        icu::UnicodeString icuFormatString;
        QString icuFormatQString;
        static_cast<SimpleDateFormat *>(df)->toPattern(icuFormatString);
        icuFormatQString = MIcuConversions::unicodeStringToQString(icuFormatString);
        if (!isTwelveHours(icuFormatQString)) {
            // change unquoted H -> h and k -> K
            // add 'a' at the right position (maybe adding a space as well)
            QString tmp;
            bool isQuoted = false;
            bool amPmMarkerWritten = false;
            QString language = categoryName(MLocale::MLcTime);
            bool writeAmPmMarkerBeforeHours = false;
            if (language.startsWith( QLatin1String("ja"))
                || language.startsWith( QLatin1String("zh")))
                writeAmPmMarkerBeforeHours = true;
            if (writeAmPmMarkerBeforeHours) {
                for (int i = 0; i < icuFormatQString.size(); ++i) {
                    QChar c = icuFormatQString[i];
                    if (c == '\'')
                        isQuoted = !isQuoted;
                    if (!isQuoted) {
                        if (c == 'H') {
                            if (!amPmMarkerWritten) {
                                tmp.append("a");
                                amPmMarkerWritten = true;
                            }
                            tmp.append("h");
                        }
                        else if (c == 'k') {
                            if (!amPmMarkerWritten) {
                                tmp.append("a");
                                amPmMarkerWritten = true;
                            }
                            tmp.append("K");
                        }
                        else
                            tmp.append(c);
                    }
                    else {
                        tmp.append(c);
                    }
                }
                icuFormatQString = tmp;
            }
            else {
                for (int i = 0; i < icuFormatQString.size(); ++i) {
                    QChar c = icuFormatQString[i];
                    if (c == '\'')
                        isQuoted = !isQuoted;
                    if (!isQuoted) {
                        if (c == 'H')
                            tmp.append("h");
                        else if (c == 'k')
                            tmp.append("K");
                        else if (c == 'z') {
                            if (!amPmMarkerWritten) {
                                if (!tmp.endsWith(' '))
                                    tmp.append(' ');
                                tmp.append("a ");
                                amPmMarkerWritten = true;
                            }
                            tmp.append(c);
                        }
                        else
                            tmp.append(c);
                    }
                    else {
                        tmp.append(c);
                    }
                }
                if (!amPmMarkerWritten)
                    tmp.append(" a");
                icuFormatQString = tmp;
            }
        }
        icuFormatString = MIcuConversions::qStringToUnicodeString(icuFormatQString);
        static_cast<SimpleDateFormat *>(df)->applyPattern(icuFormatString);
    }
}
#endif

#ifdef HAVE_ICU
QString MLocalePrivate::fixCategoryNameForNumbers(const QString &categoryName) const
{
    Q_Q(const MLocale);
    QString categoryLanguage = parseLanguage(categoryName);
    // do nothing for languages other than ar, fa, hi:
    if(categoryLanguage != "ar"
       && categoryLanguage != "fa"
       && categoryLanguage != "hi")
        return categoryName;
    // if @numbers=<something> is already there, don’t touch it
    // and return immediately:
    if(!MIcuConversions::parseOption(categoryName, "numbers").isEmpty())
        return categoryName;
    QString numericCategoryLanguage = q->categoryLanguage(MLocale::MLcNumeric);
    if(categoryLanguage == "ar") {
        if(numericCategoryLanguage == "ar")
            return MIcuConversions::setOption(categoryName, "numbers", "arab");
        else
            return MIcuConversions::setOption(categoryName, "numbers", "latn");
    }
    if(categoryLanguage == "fa") {
        if(numericCategoryLanguage == "fa")
            return MIcuConversions::setOption(categoryName, "numbers", "arabext");
        else
            return MIcuConversions::setOption(categoryName, "numbers", "latn");
    }
    if(categoryLanguage == "hi") {
        if(numericCategoryLanguage == "hi")
            return MIcuConversions::setOption(categoryName, "numbers", "deva");
        else
            return MIcuConversions::setOption(categoryName, "numbers", "latn");
    }
    // never reached:
    return categoryName;
}
#endif

#ifdef HAVE_ICU
QString MLocalePrivate::icuFormatString(MLocale::DateType dateType,
                                        MLocale::TimeType timeType,
                                        MLocale::CalendarType calendarType,
                                        MLocale::TimeFormat24h timeFormat24h) const
{
    icu::DateFormat *df = createDateFormat(dateType, timeType, calendarType, timeFormat24h);

    QString icuFormatQString;

    if (df)
    {
        icu::UnicodeString icuFormatString;
        static_cast<SimpleDateFormat *>(df)->toPattern(icuFormatString);
        icuFormatQString = MIcuConversions::unicodeStringToQString(icuFormatString);
    }
    return icuFormatQString;
}
#endif

#ifdef HAVE_ICU
icu::DateFormat *MLocalePrivate::createDateFormat(MLocale::DateType dateType,
                                                  MLocale::TimeType timeType,
                                                  MLocale::CalendarType calendarType,
                                                  MLocale::TimeFormat24h timeFormat24h) const
{
    QString categoryNameTime = categoryName(MLocale::MLcTime);
    QString categoryNameNumeric = categoryName(MLocale::MLcNumeric);
    QString categoryNameMessages = categoryName(MLocale::MLcMessages);
    QString key = QString("%1_%2_%3_%4_%5_%6_%7")
        .arg(dateType)
        .arg(timeType)
        .arg(calendarType)
        .arg(timeFormat24h)
        .arg(categoryNameTime)
        .arg(categoryNameNumeric)
        .arg(categoryNameMessages);
    if (_dateFormatCache.contains(key))
        return _dateFormatCache.object(key);
    categoryNameTime = fixCategoryNameForNumbers(
        MIcuConversions::setCalendarOption(categoryNameTime, calendarType));
    categoryNameMessages = fixCategoryNameForNumbers(
        MIcuConversions::setCalendarOption(categoryNameMessages, calendarType));
    icu::Locale calLocale = icu::Locale(qPrintable(categoryNameTime));
    icu::DateFormat::EStyle dateStyle = MIcuConversions::toEStyle(dateType);
    icu::DateFormat::EStyle timeStyle = MIcuConversions::toEStyle(timeType);
    icu::DateFormat *df
        = icu::DateFormat::createDateTimeInstance(dateStyle, timeStyle, calLocale);
    if(!categoryNameTime.contains(QRegExp("@.*mix-time-and-language=no"))) {
        // mixing in symbols like month name and weekday name from the message locale
        DateFormatSymbols *dfs =
            MLocalePrivate::createDateFormatSymbols(
                icu::Locale(qPrintable(categoryNameMessages)));
        // This is not nice but seems to be the only way to set the
        // symbols with the public API
        static_cast<SimpleDateFormat *>(df)->adoptDateFormatSymbols(dfs);
    }
    if (timeType != MLocale::TimeNone) {
        switch (timeFormat24h) {
        case(MLocale::TwelveHourTimeFormat24h):
            MLocalePrivate::dateFormatTo12h(df);
            break;
        case(MLocale::TwentyFourHourTimeFormat24h):
            MLocalePrivate::dateFormatTo24h(df);
            break;
        case(MLocale::LocaleDefaultTimeFormat24h):
            break;
        default:
            break;
        }
    }
    _dateFormatCache.insert(key, df);
    return df;
}
#endif

// Constructors

MLocalePrivate::MLocalePrivate()
    : _valid(true),
      _timeFormat24h(MLocale::LocaleDefaultTimeFormat24h),
      _phoneNumberGrouping( MLocale::DefaultPhoneNumberGrouping ),
#ifdef HAVE_ICU
      _numberFormat(0),
      _numberFormatLcTime(0),
#endif
#ifdef HAVE_GCONF
      currentLanguageItem(SettingsLanguage),
      currentLcTimeItem(SettingsLcTime),
      currentLcTimeFormat24hItem(SettingsLcTimeFormat24h),
      currentLcCollateItem(SettingsLcCollate),
      currentLcNumericItem(SettingsLcNumeric),
      currentLcMonetaryItem(SettingsLcMonetary),
      currentLcTelephoneItem(SettingsLcTelephone),
#endif
#ifdef HAVE_ICU
      _pDateTimeCalendar(0),
#endif
      q_ptr(0)
{
    if (translationPaths.isEmpty())
    {
#ifdef Q_OS_WIN
        // walk to translation dir relative to bin dir
        QDir appDir(QCoreApplication::applicationDirPath());

	appDir.cdUp();
	appDir.cd("share");
	appDir.cd("l10n");
	appDir.cd("meegotouch");

        translationPaths = (QStringList() << appDir.absolutePath());
#else
        translationPaths = (QStringList() << QString(TRANSLATION_DIR));
#endif
    }

    if (dataPaths.isEmpty())
        MLocale::setDataPath(M_ICUEXTRADATA_DIR);
}

// copy constructor
MLocalePrivate::MLocalePrivate(const MLocalePrivate &other)
    : _valid(other._valid),
      _defaultLocale(other._defaultLocale),
      _messageLocale(other._messageLocale),
      _numericLocale(other._numericLocale),
      _collationLocale(other._collationLocale),
      _calendarLocale(other._calendarLocale),
      _monetaryLocale(other._monetaryLocale),
      _nameLocale(other._nameLocale),
      _telephoneLocale(other._telephoneLocale),
      _validCountryCodes( other._validCountryCodes ),
      _timeFormat24h(other._timeFormat24h),
      _phoneNumberGrouping( other._phoneNumberGrouping ),
#ifdef HAVE_ICU
      _numberFormat(0),
      _numberFormatLcTime(0),
#endif
      _messageTranslations(other._messageTranslations),
      _timeTranslations(other._timeTranslations),
      _trTranslations(other._trTranslations),
#ifdef HAVE_GCONF
      currentLanguageItem(SettingsLanguage),
      currentLcTimeItem(SettingsLcTime),
      currentLcTimeFormat24hItem(SettingsLcTimeFormat24h),
      currentLcCollateItem(SettingsLcCollate),
      currentLcNumericItem(SettingsLcNumeric),
      currentLcMonetaryItem(SettingsLcMonetary),
      currentLcTelephoneItem(SettingsLcTelephone),
#endif
#ifdef HAVE_ICU
      _pDateTimeCalendar(0),
#endif
      q_ptr(0)
{
#ifdef HAVE_ICU
    if (other._numberFormat != 0) {
        _numberFormat = static_cast<icu::NumberFormat *>((other._numberFormat)->clone());
    }
    if (other._numberFormatLcTime != 0) {
        _numberFormatLcTime = static_cast<icu::NumberFormat *>((other._numberFormatLcTime)->clone());
    }
#endif
}

MLocalePrivate::~MLocalePrivate()
{
#ifdef HAVE_ICU
    delete _numberFormat;
    delete _numberFormatLcTime;
    // note: if tr translations are inserted into QCoreApplication
    // deleting the QTranslator removes them from the QCoreApplication

    delete _pDateTimeCalendar;
    _pDateTimeCalendar = 0;
#endif
}

MLocalePrivate &MLocalePrivate::operator=(const MLocalePrivate &other)
{
    _valid = other._valid;
    _defaultLocale = other._defaultLocale;
    _messageLocale = other._messageLocale;
    _numericLocale = other._numericLocale;
    _collationLocale = other._collationLocale;
    _calendarLocale = other._calendarLocale;
    _monetaryLocale = other._monetaryLocale;
    _nameLocale = other._nameLocale;
    _timeFormat24h = other._timeFormat24h;
    _messageTranslations = other._messageTranslations;
    _timeTranslations = other._timeTranslations;
    _trTranslations = other._trTranslations;
    _validCountryCodes = other._validCountryCodes;
    _telephoneLocale = other._telephoneLocale;

#ifdef HAVE_ICU
    delete _numberFormat;
    delete _numberFormatLcTime;

    if (other._numberFormat) {
        _numberFormat = static_cast<icu::NumberFormat *>((other._numberFormat)->clone());

    } else {
        _numberFormat = 0;
    }
    if (other._numberFormatLcTime) {
        _numberFormatLcTime = static_cast<icu::NumberFormat *>((other._numberFormatLcTime)->clone());

    } else {
        _numberFormatLcTime = 0;
    }
#endif

    return *this;
}

void MLocalePrivate::dropCaches()
{
#ifdef HAVE_ICU
    // call this function when the MLocale has changed so that
    // cached data cannot be used any more

    // delete MCalendar instance for this MLocale
    if ( _pDateTimeCalendar )
    {
        delete _pDateTimeCalendar;
        _pDateTimeCalendar = 0;
    }

    // drop cached formatString conversions
    _icuFormatStringCache.clear();
#endif
}

bool MLocalePrivate::isValidCountryCode( const QString& code ) const
{

    // no valid code starts with 0
    if (code.at(0) == '0') {
        return false;
    }

    // if the conversion fails, it will return 0, which is an invalid
    // code, so we don't need to check for the error.
    uint uIntCode = code.toUInt();

    switch (uIntCode) {
    case 1:
    case 20:
    case 212:
    case 213:
    case 214:
    case 215:
    case 216:
    case 218:
    case 219:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 231:
    case 232:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 240:
    case 241:
    case 242:
    case 243:
    case 244:
    case 245:
    case 246:
    case 247:
    case 248:
    case 249:
    case 250:
    case 251:
    case 252:
    case 253:
    case 254:
    case 255:
    case 256:
    case 257:
    case 258:
    case 259:
    case 260:
    case 261:
    case 262:
    case 263:
    case 264:
    case 265:
    case 266:
    case 267:
    case 268:
    case 269:
    case 27:
    case 290:
    case 291:
    case 297:
    case 298:
    case 299:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 350:
    case 351:
    case 352:
    case 353:
    case 354:
    case 355:
    case 356:
    case 357:
    case 358:
    case 359:
    case 36:
    case 370:
    case 371:
    case 372:
    case 373:
    case 374:
    case 375:
    case 376:
    case 377:
    case 378:
    case 379:
    case 380:
    case 381:
    case 382:
    case 385:
    case 386:
    case 387:
    case 388:
    case 389:
    case 39:
    case 40:
    case 41:
    case 420:
    case 421:
    case 423:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 500:
    case 501:
    case 502:
    case 503:
    case 504:
    case 505:
    case 506:
    case 507:
    case 508:
    case 509:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 590:
    case 591:
    case 592:
    case 593:
    case 594:
    case 595:
    case 596:
    case 597:
    case 598:
    case 599:
    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 670:
    case 672:
    case 673:
    case 674:
    case 675:
    case 676:
    case 677:
    case 678:
    case 679:
    case 680:
    case 681:
    case 682:
    case 683:
    case 685:
    case 686:
    case 687:
    case 688:
    case 689:
    case 690:
    case 691:
    case 692:
    case 7:
    case 800:
    case 808:
    case 81:
    case 82:
    case 84:
    case 850:
    case 852:
    case 853:
    case 855:
    case 856:
    case 86:
    case 870:
    case 871:
    case 872:
    case 873:
    case 874:
    case 878:
    case 880:
    case 881:
    case 882:
    case 883:
    case 886:
    case 888:
    case 90:
    case 91:
    case 92:
    case 93:
    case 94:
    case 95:
    case 960:
    case 961:
    case 962:
    case 963:
    case 964:
    case 965:
    case 966:
    case 967:
    case 968:
    case 970:
    case 971:
    case 972:
    case 973:
    case 974:
    case 975:
    case 976:
    case 977:
    case 979:
    case 98:
    case 991:
    case 992:
    case 993:
    case 994:
    case 995:
    case 996:
    case 998:
        return true;
    default:
        return false;
    }
}

#ifdef HAVE_ICU
// creates icu::Locale presenting a category
Locale MLocalePrivate::getCategoryLocale(MLocale::Category category) const
{
    return icu::Locale(qPrintable(categoryName(category)));
}
#endif

// creates an QString for category or default as a fallback
QString MLocalePrivate::categoryName(MLocale::Category category) const
{
    switch (category) {
    case(MLocale::MLcMessages):
        if (!_messageLocale.isEmpty()) {
            return _messageLocale;
        }
        break;

    case(MLocale::MLcNumeric):
        if (!_numericLocale.isEmpty()) {
            return _numericLocale;
        }
        break;

    case(MLocale::MLcCollate):
        if (!_collationLocale.isEmpty()) {
            return _collationLocale;
        }
        break;

    case(MLocale::MLcMonetary):
        if (!_monetaryLocale.isEmpty()) {
            return _monetaryLocale;
        }
        break;

    case(MLocale::MLcTime):
        if (!_calendarLocale.isEmpty()) {
            return _calendarLocale;
        }
        break;

    case(MLocale::MLcName):
        if (!_nameLocale.isEmpty()) {
            return _nameLocale;
        }
        break;

    case(MLocale::MLcTelephone):
        if (!_telephoneLocale.isEmpty()) {
            return _telephoneLocale;
        }
        break;
    }

    return _defaultLocale;
}

void MLocalePrivate::loadTrCatalogs()
{
    Q_Q(MLocale);
    foreach(const QExplicitlySharedDataPointer<MTranslationCatalog>& sharedCatalog, _trTranslations) { // krazy:exclude=foreach
        if(sharedCatalog->_translator.isEmpty()
           || !sharedCatalog->_name.endsWith(QLatin1String(".qm"))) {
            sharedCatalog->loadWith(q, MLocale::MLcMessages);
        }
    }
}

void MLocalePrivate::insertTrToQCoreApp()
{
    foreach(const QExplicitlySharedDataPointer<MTranslationCatalog>& sharedCatalog, _trTranslations) { // krazy:exclude=foreach
        QCoreApplication::installTranslator(&sharedCatalog->_translator);
    }
}

void MLocalePrivate::removeTrFromQCoreApp()
{
    foreach(const QExplicitlySharedDataPointer<MTranslationCatalog>& sharedCatalog, _trTranslations) { // krazy:exclude=foreach
        QCoreApplication::removeTranslator(&sharedCatalog->_translator);
    }
}

void MLocalePrivate::insertDirectionTrToQCoreApp()
{
    if (s_rtlTranslator == 0) {
        s_rtlTranslator = new QTranslator( qApp );
        bool ok = s_rtlTranslator->load(":/libmeegotouch_rtl.qm");
	Q_UNUSED(ok);
        Q_ASSERT(ok);
    }
    if (s_ltrTranslator == 0) {
        s_ltrTranslator = new QTranslator( qApp );
        bool ok = s_ltrTranslator->load(":/libmeegotouch_ltr.qm");
	Q_UNUSED(ok);
        Q_ASSERT(ok);
    }

    if (MLocale::s_systemDefault->textDirection() == Qt::RightToLeft) {
        // make sure previous installations of the direction translators
        // are removed:
        QCoreApplication::removeTranslator(s_ltrTranslator);
        QCoreApplication::removeTranslator(s_rtlTranslator);
        // install the correct direction translator for the current
        // system default locale:
        QCoreApplication::installTranslator(s_rtlTranslator);
    }
    else {
        // make sure previous installations of the direction translators
        // are removed:
        QCoreApplication::removeTranslator(s_rtlTranslator);
        QCoreApplication::removeTranslator(s_ltrTranslator);
        // install the correct direction translator for the current
        // system default locale:
        QCoreApplication::installTranslator(s_ltrTranslator);
    }
}

QLocale MLocalePrivate::createQLocale(MLocale::Category category) const
{
    // This function is mainly used to create a QLocale which is then passed to
    // QLocale::setDefault(...) to get support for localized numbers
    // in translations via %Ln, %L1, %L2, ... .
    Q_Q(const MLocale);
    QString language = q->categoryLanguage(category);
    QString country = q->categoryCountry(category);
    QString categoryName = q->categoryName(category);
    QString numberOption = MIcuConversions::parseOption(categoryName, "numbers");

    switch(category) {
    case(MLocale::MLcNumeric):
    case(MLocale::MLcTime):
    case(MLocale::MLcMonetary):
        if(language == "ar" || language == "fa") {
            if(numberOption == "latn") {
                // We have no way to disable use of Eastern Arabic digits
                // in QLocale. Therefore, we change the locale to US English
                // if Latin numbers are requested, this produces reasonably
                // good results:
                language = QLatin1String("en");
                country = QLatin1String("US");
            }
            else if(country == "TN" || country == "MA" || country == "DZ") {
                // for TN (Tunisia), MA (Morocco), and DZ (Algeria),
                // Qt always formats with Western digits (because that
                // is the default in CLDR for these countries, for the
                // same reason libicu formats with Western digits by
                // default for these countries). But we want Arabic digits
                // by default, unless they are explicitely disabled
                // by an option like “ar_TN@numbers=latn” (this case is handled
                // above). So we switch the country to EG (Egypt) because
                // the numeric formats for Egypt are similar to those for
                // the above 3 countries except that Qt uses Eastern Arabic
                // digits for Egypt:
                country = "EG";
            }
        }
        break;
    default:
        break;
    }
    QLocale qlocale(language + '_' + country);
    return qlocale;
}

// sets category to specific locale
void MLocalePrivate::setCategoryLocale(MLocale *mlocale,
        MLocale::Category category,
        const QString &localeName)
{
    Q_UNUSED(mlocale);

    if (category == MLocale::MLcMessages) {
        _messageLocale = localeName;
    } else if (category == MLocale::MLcTime) {
        _calendarLocale = localeName;
#ifdef HAVE_ICU
        // recreate the number formatter
        delete _numberFormatLcTime;
        QString categoryNameTime =
            fixCategoryNameForNumbers(categoryName(MLocale::MLcTime));
        icu::Locale timeLocale = icu::Locale(qPrintable(categoryNameTime));
        UErrorCode status = U_ZERO_ERROR;
        _numberFormatLcTime = icu::NumberFormat::createInstance(timeLocale, status);
        if (!U_SUCCESS(status)) {
            mDebug("MLocalePrivate") << "Unable to create number format for LcTime" << u_errorName(status);
            _valid = false;
        }
#endif
    } else if (category == MLocale::MLcNumeric) {
        _numericLocale = localeName;
#ifdef HAVE_ICU
        // recreate the number formatters
        delete _numberFormat;
        QString categoryNameNumeric =
            fixCategoryNameForNumbers(categoryName(MLocale::MLcNumeric));
        icu::Locale numericLocale = icu::Locale(qPrintable(categoryNameNumeric));
        UErrorCode status = U_ZERO_ERROR;
        _numberFormat = icu::NumberFormat::createInstance(numericLocale, status);
        if (!U_SUCCESS(status)) {
            mDebug("MLocalePrivate") << "Unable to create number format for LcNumeric" << u_errorName(status);
            _valid = false;
        }
        delete _numberFormatLcTime;
        QString categoryNameTime =
            fixCategoryNameForNumbers(categoryName(MLocale::MLcTime));
        icu::Locale timeLocale = icu::Locale(qPrintable(categoryNameTime));
        status = U_ZERO_ERROR;
        _numberFormatLcTime = icu::NumberFormat::createInstance(timeLocale, status);
        if (!U_SUCCESS(status)) {
            mDebug("MLocalePrivate") << "Unable to create number format for LcTime" << u_errorName(status);
            _valid = false;
        }
#endif
    } else if (category == MLocale::MLcCollate) {
        _collationLocale = localeName;
    } else if (category == MLocale::MLcMonetary) {
        _monetaryLocale = localeName;
    } else if (category == MLocale::MLcName) {
        _nameLocale = localeName;
    } else if (category == MLocale::MLcTelephone) {
        _telephoneLocale = localeName;
        // here we set the phone number grouping depending on the
        // setting in the gconf key
        if ( _telephoneLocale.startsWith( QLatin1String( "en_US" ) ) ) {
            _phoneNumberGrouping = MLocale::NorthAmericanPhoneNumberGrouping;
        } else {
            _phoneNumberGrouping = MLocale::NoPhoneNumberGrouping;
        }
    } else {
        //mDebug("MLocalePrivate") << "unimplemented category change"; // DEBUG
    }
}

bool MLocalePrivate::parseIcuLocaleString(const QString &localeString, QString *language, QString *script, QString *country, QString *variant)
{
    // A ICU locale string looks like this:
    //     aa_Bbbb_CC_DDDDDD@foo=fooval;bar=barval;
    // see also http://userguide.icu-project.org/locale
    // The country part is usually a 2 letter uppercase code
    // as in the above example, but there is the exception
    // es_419, i.e. Spanish in Latin America where the “country code”
    // is “419”.
    QRegExp regexp("^([a-z]{2,3})(?:_([A-Z][a-z]{3,3}))?(?:_([A-Z]{2,2}|419))?(?:_{1,2}([A-Z][A-Z_]*))?(?:@.*)?$");
    if (regexp.indexIn(localeString) == 0
        && regexp.capturedTexts().size() == 5) {
        *language = regexp.capturedTexts().at(1);
        *script   = regexp.capturedTexts().at(2); // "" if no match
        *country  = regexp.capturedTexts().at(3); // "" if no match
        *variant  = regexp.capturedTexts().at(4); // "" if no match
        return true;
    }
    else {
        *language = "";
        *script = "";
        *country = "";
        *variant= "";
        return false;
    }
}

QString MLocalePrivate::parseLanguage(const QString &localeString)
{
    QString language, script, country, variant;
    parseIcuLocaleString(localeString, &language, &script, &country, &variant);
    return language;
}

QString MLocalePrivate::parseCountry(const QString &localeString)
{
    QString language, script, country, variant;
    parseIcuLocaleString(localeString, &language, &script, &country, &variant);
    return country;
}

QString MLocalePrivate::parseScript(const QString &localeString)
{
    QString language, script, country, variant;
    parseIcuLocaleString(localeString, &language, &script, &country, &variant);
    return script;
}

QString MLocalePrivate::parseVariant(const QString &localeString)
{
    QString language, script, country, variant;
    parseIcuLocaleString(localeString, &language, &script, &country, &variant);
    return variant;
}

//////////////////////////////////
///// MLocale class implementation


// icu handles this string as synonym for posix locale behaviour
namespace
{
    const char *const PosixStr = "en_US_POSIX";
}

// Converts POSIX style locale code to Nokia (ICU) style without variant
// eg. snd_AF.UTF-8@Arab (POSIX) to snd_Arab_AF (Nokia)
//
// The syntax of the locale string in the POSIX environment variables
// related to locale is:
//
//    [language[_territory][.codeset][@modifier]]
//
// (see: http://www.opengroup.org/onlinepubs/000095399/basedefs/xbd_chap08.html)
//
// language is usually lower case in Linux but according to the above specification
// it may start with uppercase as well (i.e. LANG=Fr_FR is allowed).
//
static QString
cleanLanguageCountryPosix(QString &localeString)
{
    // we do not need the encoding and therefore use non-capturing
    // parentheses for the encoding part here.
    // The country part is usually a 2 letter uppercase code
    // as in the above example, but there is the exception
    // es_419, i.e. Spanish in Latin America where the “country code”
    // is “419”. es_419 isn’t really a valid value for LANG, but for consistency
    // let’s make this behave the same way as the icu locale names work for es_419,
    // we only use LANG as a fallback to specify a locale when gconf isn’t available
    // or doesn’t work.
    QRegExp regexp("([a-z]{2,3})(_([A-Z]{2,2}|419))?(?:.(?:[a-zA-Z0-9-]+))?(@([A-Z][a-z]+))?");

    if (regexp.indexIn(localeString) == 0 &&
            regexp.capturedTexts().size() == 6) { // size of regexp pattern above
        QStringList strings;

        strings << regexp.capturedTexts().at(1); // language

        // POSIX locale modifier, interpreted as script
        if (!regexp.capturedTexts().at(5).isEmpty())
            strings << regexp.capturedTexts().at(5);

        if (!regexp.capturedTexts().at(3).isEmpty())
            strings << regexp.capturedTexts().at(3); // country

        // we don't need variant
        return strings.join("_");
    } else {
        //Malformed locale code
        return QString(PosixStr);
    }
}

MLocale *
MLocale::createSystemMLocale()
{
#ifdef HAVE_GCONF
    MGConfItem languageItem(SettingsLanguage);
    MGConfItem lcTimeItem(SettingsLcTime);
    MGConfItem lcTimeFormat24hItem(SettingsLcTimeFormat24h);
    MGConfItem lcCollateItem(SettingsLcCollate);
    MGConfItem lcNumericItem(SettingsLcNumeric);
    MGConfItem lcMonetaryItem(SettingsLcMonetary);
    MGConfItem lcTelephoneItem(SettingsLcTelephone);

    QString language = languageItem.value("en_GB").toString();
    QString lcTime = lcTimeItem.value("en_GB").toString();
    QString lcTimeFormat24h = lcTimeFormat24hItem.value("12").toString();
    QString lcCollate = lcCollateItem.value("en_GB").toString();
    QString lcNumeric = lcNumericItem.value("en_GB").toString();
    QString lcMonetary = lcMonetaryItem.value("en_GB").toString();
    QString lcTelephone = lcTelephoneItem.value().toString();

    MLocale *systemLocale;

    if (language.isEmpty()) {
        language = qgetenv("LANG");
        QString locale = cleanLanguageCountryPosix(language);

        if (locale.isEmpty())
            locale = PosixStr;

        // No need to set the category according to env here
        systemLocale = new MLocale(locale);
    } else {
        systemLocale = new MLocale(language);
    }

    if (!lcTime.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcTime, lcTime);
    if (lcTimeFormat24h == "24")
        systemLocale->setTimeFormat24h(MLocale::TwentyFourHourTimeFormat24h);
    else if (lcTimeFormat24h == "12")
        systemLocale->setTimeFormat24h(MLocale::TwelveHourTimeFormat24h);
    else
        systemLocale->setTimeFormat24h(MLocale::LocaleDefaultTimeFormat24h);
    if (!lcCollate.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcCollate, lcCollate);
    if (!lcNumeric.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    if (!lcMonetary.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcMonetary, lcMonetary);
    if (!lcTelephone.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcTelephone, lcTelephone);

    return systemLocale;
#else
    QString language = qgetenv("LANG");
    QString locale = cleanLanguageCountryPosix(language);
    if (language.isEmpty())
        language = PosixStr;

    return new MLocale(locale);
#endif
}

MLocale MLocale::createCLocale()
{
    return MLocale(PosixStr);
}

void
MLocale::connectSettings()
{
#ifdef HAVE_GCONF
    Q_D(MLocale);

    QObject::connect(&d->currentLanguageItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcTimeItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcTimeFormat24hItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcCollateItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcNumericItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcMonetaryItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcTelephoneItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
#endif
}

void
MLocale::disconnectSettings()
{
#ifdef HAVE_GCONF
    Q_D(MLocale);

    QObject::disconnect(&d->currentLanguageItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcTimeItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcTimeFormat24hItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcCollateItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcNumericItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcMonetaryItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcTelephoneItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
#endif
}

///// Constructors  /////

//! Constructs a MLocale with data copied from default Locale
MLocale::MLocale(QObject *parent)
    : QObject(parent),
      d_ptr(new MLocalePrivate)
{
    Q_D(MLocale);
    d->q_ptr = this;
    // copy the system default
    MLocale &defaultLocale = getDefault();
    *this = defaultLocale;
}

MLocale::MLocale(const QString &localeName, QObject *parent)
    : QObject(parent),
      d_ptr(new MLocalePrivate)
{
    Q_D(MLocale);
    d->q_ptr = this;
    d->_defaultLocale = qPrintable(localeName);
    // If a system default locale exists already copy the translation
    // catalogs and reload them for this locale:
    if (s_systemDefault)
        copyCatalogsFrom(*s_systemDefault);

#ifdef HAVE_ICU
    // we cache the number formatter for better performance
    QString categoryNameNumeric =
        d->fixCategoryNameForNumbers(categoryName(MLocale::MLcNumeric));
    UErrorCode status = U_ZERO_ERROR;
    d->_numberFormat =
        icu::NumberFormat::createInstance(icu::Locale(qPrintable(categoryNameNumeric)),
                                          status);
    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating for LcNumeric failed:" << u_errorName(status);
        d->_valid = false;
    }
    QString categoryNameTime =
        d->fixCategoryNameForNumbers(categoryName(MLocale::MLcTime));
    status = U_ZERO_ERROR;
    d->_numberFormatLcTime =
        icu::NumberFormat::createInstance(icu::Locale(qPrintable(categoryNameTime)),
                                          status);
    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating for LcTime failed:" << u_errorName(status);
        d->_valid = false;
    }
#endif
}


//! Copy constructor
MLocale::MLocale(const MLocale &other, QObject *parent)
    : QObject(parent),
      d_ptr(new MLocalePrivate(*other.d_ptr))
{
    Q_D(MLocale);
    d->q_ptr = this;
}


//! Destructor
MLocale::~MLocale()
{
    // do not delete the d_ptr of s_systemDefault unless we are s_systemDefault
    if (d_ptr) {
        if (s_systemDefault == 0) {
            delete d_ptr;
        } else if (d_ptr != s_systemDefault->d_ptr) {
            delete d_ptr;
        } else if (this == s_systemDefault) {
            delete d_ptr;
            s_systemDefault = 0;
        }
    }
}

//! Assignment operator
MLocale &MLocale::operator=(const MLocale &other)
{
    if (this == &other) {
        return *this;
    }

    *d_ptr = *other.d_ptr;

    return *this;
}



///////////////////
//// normal methods

// mutex to guard default locale
static QMutex defaultLocaleMutex;

// The static default locale
MLocale *MLocale::s_systemDefault = 0;

static Qt::LayoutDirection _defaultLayoutDirection = Qt::LeftToRight;

struct MStaticLocaleDestroyer {
    ~MStaticLocaleDestroyer() {
        delete MLocale::s_systemDefault; MLocale::s_systemDefault = 0;
    }
};
static MStaticLocaleDestroyer staticLocaleDestroyer;

void MLocale::setDefault(const MLocale &locale)
{
    defaultLocaleMutex.lock();

    if (s_systemDefault == 0) {
        s_systemDefault = new MLocale(locale);
    } else if (&locale == s_systemDefault || locale.d_ptr == s_systemDefault->d_ptr) {
        defaultLocaleMutex.unlock();
        return;
    } else {
        s_systemDefault->disconnectSettings();
        if (qobject_cast<MApplication *> (qApp))
            QObject::disconnect(s_systemDefault, SIGNAL(settingsChanged()),
                                qApp, SIGNAL(localeSettingsChanged()));
        // remove the previous tr translations
        (s_systemDefault->d_ptr)->removeTrFromQCoreApp();
        *s_systemDefault = locale;
    }
    defaultLocaleMutex.unlock();
    // load special translations to make QApplication detect the
    // correct direction (see qapplication.cpp in the Qt source
    // code). If this is not done, the QEvent::LanguageChange events
    // triggered by QCoreApplication::removeTranslator() and
    // QCoreApplication::installTranslator() which are called by
    // removeTrFromQCoreApp() and insertTrToQCoreApp() may set a wrong
    // direction because these QEvent::LanguageChange may be processed
    // later than the QEvent::ApplicationLayoutDirectionChange event
    // triggered by
    // qApp->setLayoutDirection(s_systemDefault->textDirection());
    (s_systemDefault->d_ptr)->insertDirectionTrToQCoreApp();

    // sends QEvent::LanguageChange to qApp:
    (s_systemDefault->d_ptr)->insertTrToQCoreApp();
    // Setting the default QLocale is needed to get localized number
    // support in translations via %Ln, %L1, %L2, ...:
    QLocale::setDefault((s_systemDefault->d_ptr)->createQLocale(MLcNumeric));
    // sends QEvent::ApplicationLayoutDirectionChange to qApp:
    qApp->setLayoutDirection(s_systemDefault->textDirection());
#ifdef HAVE_ICU
    _defaultLayoutDirection = MIcuConversions::parseLayoutDirectionOption(s_systemDefault->name());
#else
    _defaultLayoutDirection = Qt::LeftToRight;
#endif

    if (qobject_cast<MApplication *> (qApp))
        QObject::connect(s_systemDefault, SIGNAL(settingsChanged()),
                         qApp, SIGNAL(localeSettingsChanged()));
    emit s_systemDefault->settingsChanged();
    s_systemDefault->connectSettings();
}

MLocale &MLocale::getDefault()
{
    if (s_systemDefault == 0) {
        // no default created, do it now

        // avoid race condition for multiple getDefaults()
        defaultLocaleMutex.lock();

        if (s_systemDefault == 0) {
            // we won the race
            s_systemDefault = createSystemMLocale();
        }

        defaultLocaleMutex.unlock();
    }

    return *s_systemDefault;
}

bool MLocale::isValid() const
{
    Q_D(const MLocale);
    return d->_valid;
}

void MLocale::setCategoryLocale(Category category, const QString &localeName)
{
    Q_D(MLocale);
    d->setCategoryLocale(this, category, localeName);

    d->dropCaches();
}

void MLocale::setCollation(Collation collation)
{
    Q_D(MLocale);
    d->dropCaches();
#ifdef HAVE_ICU
    if(!d->_collationLocale.isEmpty())
        d->_collationLocale =
            MIcuConversions::setCollationOption(d->_collationLocale, collation);
    else
        d->_defaultLocale =
            MIcuConversions::setCollationOption(d->_defaultLocale, collation);
#else
    Q_UNUSED(collation);
#endif
}

MLocale::Collation MLocale::collation() const
{
#ifdef HAVE_ICU
    return MIcuConversions::parseCollationOption(categoryName(MLcCollate));
#else
    return MLocale::DefaultCollation;
#endif
}

void MLocale::setCalendarType(CalendarType calendarType)
{
    Q_D(MLocale);
    d->dropCaches();
#ifdef HAVE_ICU
    if(!d->_calendarLocale.isEmpty())
        d->_calendarLocale =
            MIcuConversions::setCalendarOption(d->_calendarLocale, calendarType);
    else
        d->_defaultLocale =
            MIcuConversions::setCalendarOption(d->_defaultLocale, calendarType);
#else
    Q_UNUSED(calendarType);
#endif
}

MLocale::CalendarType MLocale::calendarType() const
{
#ifdef HAVE_ICU
    return MIcuConversions::parseCalendarOption(categoryName(MLcTime));
#else
    return MLocale::DefaultCalendar;
#endif
}

void MLocale::setTimeFormat24h(TimeFormat24h timeFormat24h)
{
    Q_D(MLocale);
    d->_timeFormat24h = timeFormat24h;

    d->dropCaches();
}

MLocale::TimeFormat24h MLocale::timeFormat24h() const
{
    Q_D(const MLocale);
    return d->_timeFormat24h;
}

#ifdef HAVE_ICU
MLocale::TimeFormat24h MLocale::defaultTimeFormat24h() const
{
    Q_D(const MLocale);
    QString defaultTimeShortFormat
        = d->icuFormatString(MLocale::DateNone, MLocale::TimeShort,
                             calendarType(),
                             MLocale::LocaleDefaultTimeFormat24h);
    if (d->isTwelveHours(defaultTimeShortFormat))
        return MLocale::TwelveHourTimeFormat24h;
    else
        return MLocale::TwentyFourHourTimeFormat24h;
}
#endif

#ifdef HAVE_ICU
MCollator MLocale::collator() const
{
    return MCollator(*this);
}
#endif

QString MLocale::toLower(const QString &string) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    // we don’t have MLcCtype, MLcMessages comes closest
    return MIcuConversions::unicodeStringToQString(
        MIcuConversions::qStringToUnicodeString(string).toLower(
            d->getCategoryLocale(MLcMessages)));
#else
    // QString::toLower() is *not* locale aware, this is only
    // a “better than nothing” fallback.
    return string.toLower();
#endif
}

QString MLocale::toUpper(const QString &string) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    // we don’t have MLcCtype, MLcMessages comes closest
    return MIcuConversions::unicodeStringToQString(
        MIcuConversions::qStringToUnicodeString(string).toUpper(
            d->getCategoryLocale(MLcMessages)));
#else
    // QString::toUpper() is *not* locale aware, this is only
    // a “better than nothing” fallback.
    return string.toUpper();
#endif
}

QString MLocale::language() const
{
    return MLocalePrivate::parseLanguage(name());
}

QString MLocale::country() const
{
    return MLocalePrivate::parseCountry(name());
}

QString MLocale::script() const
{
    return MLocalePrivate::parseScript(name());
}

QString MLocale::variant() const
{
    return MLocalePrivate::parseVariant(name());
}

QString MLocale::name() const
{
    Q_D(const MLocale);
    return d->_defaultLocale;
}

QString MLocale::categoryLanguage(Category category) const
{
    QString wholeName = categoryName(category);
    return MLocalePrivate::parseLanguage(wholeName);
}

QString MLocale::categoryCountry(Category category) const
{
    QString wholeName = categoryName(category);
    return MLocalePrivate::parseCountry(wholeName);
}

QString MLocale::categoryScript(Category category) const
{
    QString wholeName = categoryName(category);
    return MLocalePrivate::parseScript(wholeName);
}

QString MLocale::categoryVariant(Category category) const
{
    QString wholeName = categoryName(category);
    return MLocalePrivate::parseVariant(wholeName);
}

QString MLocale::categoryName(Category category) const
{
    Q_D(const MLocale);
    return d->categoryName(category);
}

QString MLocale::formatNumber(qlonglong i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UnicodeString str;
    // This might generate a warning by the Krazy code analyzer,
    // but it allows the code to compile with ICU 4.0
    d->_numberFormat->format(static_cast<int64_t>(i), str); //krazy:exclude=typedefs
    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i);
#endif
}

qlonglong MLocale::toLongLong(const QString &s, bool *ok) const
{
    if (s.length() == 0) {
        if (ok != NULL)
            *ok = false;
        return (int(0));
    }
#ifdef HAVE_ICU
    Q_D(const MLocale);
    QString parseInput = s;
    d->removeDirectionalFormattingCodes(&parseInput);
    icu::UnicodeString str = MIcuConversions::qStringToUnicodeString(parseInput);
    icu::Formattable formattable;
    icu::ParsePosition parsePosition;
    qint64 result;
    icu::DecimalFormat *decimalFormat
        = static_cast<icu::DecimalFormat *>(d->_numberFormat);
    if (!decimalFormat->isParseIntegerOnly()) {
        decimalFormat->setParseIntegerOnly(true);
        decimalFormat->parse(str, formattable, parsePosition);
        decimalFormat->setParseIntegerOnly(false);
    }
    else
        decimalFormat->parse(str, formattable, parsePosition);
    if (parsePosition.getIndex() < str.length()) {
        if (ok != NULL)
            *ok = false;
        return (qlonglong(0));
    }
    else {
        UErrorCode status = U_ZERO_ERROR;
        result = formattable.getInt64(status);
        if (!U_SUCCESS(status)) {
            if (ok != NULL)
                *ok = false;
            return (qlonglong(0));
        }
        else {
            if (ok != NULL)
                *ok = true;
            return (qlonglong(result));
        }
    }
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toLongLong(s, ok);
#endif
}

QString MLocale::formatNumber(short i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UnicodeString str;
    d->_numberFormat->format(i, str);
    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i);
#endif
}

short MLocale::toShort(const QString &s, bool *ok) const
{
    if (s.length() == 0) {
        if (ok != NULL)
            *ok = false;
        return (int(0));
    }
#ifdef HAVE_ICU
    Q_D(const MLocale);
    QString parseInput = s;
    d->removeDirectionalFormattingCodes(&parseInput);
    icu::UnicodeString str = MIcuConversions::qStringToUnicodeString(parseInput);
    icu::Formattable formattable;
    icu::ParsePosition parsePosition;
    qint64 result;
    icu::DecimalFormat *decimalFormat
        = static_cast<icu::DecimalFormat *>(d->_numberFormat);
    if (!decimalFormat->isParseIntegerOnly()) {
        decimalFormat->setParseIntegerOnly(true);
        decimalFormat->parse(str, formattable, parsePosition);
        decimalFormat->setParseIntegerOnly(false);
    }
    else
        decimalFormat->parse(str, formattable, parsePosition);
    if (parsePosition.getIndex() < str.length()) {
        if (ok != NULL)
            *ok = false;
        return (short(0));
    }
    else {
        UErrorCode status = U_ZERO_ERROR;
        result = formattable.getInt64(status);
        if (!U_SUCCESS(status)) {
            if (ok != NULL)
                *ok = false;
            return (short(0));
        }
        else {
            if (result < SHRT_MIN || result > SHRT_MAX) {
                if (ok != NULL)
                    *ok = false;
                return (short(0));
            }
            else {
                if (ok != NULL)
                    *ok = true;
                return (short(result));
            }
        }
    }
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toShort(s, ok);
#endif
}

QString MLocale::formatNumber(int i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UnicodeString str;
    d->_numberFormat->format(i, str);
    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i);
#endif
}

int MLocale::toInt(const QString &s, bool *ok) const
{
    if (s.length() == 0) {
        if (ok != NULL)
            *ok = false;
        return (int(0));
    }
#ifdef HAVE_ICU
    Q_D(const MLocale);
    QString parseInput = s;
    d->removeDirectionalFormattingCodes(&parseInput);
    icu::UnicodeString str = MIcuConversions::qStringToUnicodeString(parseInput);
    icu::Formattable formattable;
    icu::ParsePosition parsePosition;
    qint64 result;
    icu::DecimalFormat *decimalFormat
        = static_cast<icu::DecimalFormat *>(d->_numberFormat);
    if (!decimalFormat->isParseIntegerOnly()) {
        decimalFormat->setParseIntegerOnly(true);
        decimalFormat->parse(str, formattable, parsePosition);
        decimalFormat->setParseIntegerOnly(false);
    }
    else
        decimalFormat->parse(str, formattable, parsePosition);
    if (parsePosition.getIndex() < str.length()) {
        if (ok != NULL)
            *ok = false;
        return (int(0));
    }
    else {
        UErrorCode status = U_ZERO_ERROR;
        result = formattable.getInt64(status);
        if (!U_SUCCESS(status)) {
            if (ok != NULL)
                *ok = false;
            return (int(0));
        }
        else {
            if (result < INT_MIN || result > INT_MAX) {
                if (ok != NULL)
                    *ok = false;
                return (int(0));
            }
            else {
                if (ok != NULL)
                    *ok = true;
                return (int(result));
            }
        }
    }
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toInt(s, ok);
#endif
}

QString MLocale::formatNumber(double i, int prec) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    icu::UnicodeString str;
    icu::FieldPosition pos;

    if (prec == -1) {
        d->_numberFormat->format(i, str, pos);
    } else {
        // the cached number formatter isn't sufficient
        QString categoryNameNumeric =
            d->fixCategoryNameForNumbers(categoryName(MLocale::MLcNumeric));
        UErrorCode status = U_ZERO_ERROR;
        icu::NumberFormat *nf;
        nf = icu::NumberFormat::createInstance(icu::Locale(qPrintable(categoryNameNumeric)),
                                               status);
        if (!U_SUCCESS(status)) {
            qWarning() << "NumberFormat creating failed" << u_errorName(status);
            return QString(); // "null" string
        }

        nf->setMaximumFractionDigits(prec);
        nf->format(i, str);
        delete nf;
    }

    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i, 'g', prec);
#endif
}

double MLocale::toDouble(const QString &s, bool *ok) const
{
    if (s.length() == 0) {
        if (ok != NULL)
            *ok = false;
        return (int(0));
    }
#ifdef HAVE_ICU
    Q_D(const MLocale);
    icu::DecimalFormat *decimalFormat
        = static_cast<icu::DecimalFormat *>(d->_numberFormat);
    const icu::DecimalFormatSymbols *decimalFormatSymbols
        = decimalFormat->getDecimalFormatSymbols();
    QString exponentialSymbol
        = MIcuConversions::unicodeStringToQString(
            decimalFormatSymbols->getSymbol(DecimalFormatSymbols::kExponentialSymbol));
    QString parseInput = s;
    d->removeDirectionalFormattingCodes(&parseInput);
    // parse the exponential symbol in the input case insensitive:
    parseInput.replace(exponentialSymbol, exponentialSymbol, Qt::CaseInsensitive);
    icu::UnicodeString str = MIcuConversions::qStringToUnicodeString(parseInput);
    icu::Formattable formattable;
    icu::ParsePosition parsePosition;
    double result;
    if (decimalFormat->isParseIntegerOnly()) {
        decimalFormat->setParseIntegerOnly(false);
        decimalFormat->parse(str, formattable, parsePosition);
        decimalFormat->setParseIntegerOnly(true);
    }
    else
        decimalFormat->parse(str, formattable, parsePosition);
    if (parsePosition.getIndex() < str.length()) {
        if (ok != NULL)
            *ok = false;
        return (double(0.0));
    }
    else {
        UErrorCode status = U_ZERO_ERROR;
        result = formattable.getDouble(status);
        if (!U_SUCCESS(status)) {
            if (ok != NULL)
                *ok = false;
            return (double(0.0));
        }
        else {
            if (ok != NULL)
                *ok = true;
            return (result);
        }
    }
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toDouble(s, ok);
#endif
}

QString MLocale::formatNumber(float i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    icu::UnicodeString str;
    icu::FieldPosition pos;
    d->_numberFormat->format(i, str, pos);
    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i, 'g');
#endif
}

float MLocale::toFloat(const QString &s, bool *ok) const
{
    if (s.length() == 0) {
        if (ok != NULL)
            *ok = false;
        return (int(0));
    }
#ifdef HAVE_ICU
    Q_D(const MLocale);
    icu::DecimalFormat *decimalFormat
        = static_cast<icu::DecimalFormat *>(d->_numberFormat);
    const icu::DecimalFormatSymbols *decimalFormatSymbols
        = decimalFormat->getDecimalFormatSymbols();
    QString exponentialSymbol
        = MIcuConversions::unicodeStringToQString(
            decimalFormatSymbols->getSymbol(DecimalFormatSymbols::kExponentialSymbol));
    QString parseInput = s;
    d->removeDirectionalFormattingCodes(&parseInput);
    // parse the exponential symbol in the input case insensitive:
    parseInput.replace(exponentialSymbol, exponentialSymbol, Qt::CaseInsensitive);
    icu::UnicodeString str = MIcuConversions::qStringToUnicodeString(parseInput);
    icu::Formattable formattable;
    icu::ParsePosition parsePosition;
    double result;
    if (decimalFormat->isParseIntegerOnly()) {
        decimalFormat->setParseIntegerOnly(false);
        decimalFormat->parse(str, formattable, parsePosition);
        decimalFormat->setParseIntegerOnly(true);
    }
    else
        decimalFormat->parse(str, formattable, parsePosition);
    if (parsePosition.getIndex() < str.length()) {
        if (ok != NULL)
            *ok = false;
        return (float(0.0));
    }
    else {
        UErrorCode status = U_ZERO_ERROR;
        result = formattable.getDouble(status);
        if (!U_SUCCESS(status)) {
            if (ok != NULL)
                *ok = false;
            return (float(0.0));
        }
        else {
            if (qAbs(result) > FLT_MAX) {
                if (ok != NULL)
                    *ok = false;
                return (float(0.0));
            }
            else {
                if (ok != NULL)
                    *ok = true;
                return (float(result));
            }
        }
    }
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toFloat(s, ok);
#endif
}

#ifdef HAVE_ICU
void MLocalePrivate::removeDirectionalFormattingCodes(QString *str) const
{
    str->remove(QChar(0x200F)); // RIGHT-TO-LEFT MARK
    str->remove(QChar(0x200E)); // LEFT-TO-RIGHT MARK
    str->remove(QChar(0x202D)); // LEFT-TO-RIGHT OVERRIDE
    str->remove(QChar(0x202E)); // RIGHT-TO-LEFT OVERRIDE
    str->remove(QChar(0x202A)); // LEFT-TO-RIGHT EMBEDDING
    str->remove(QChar(0x202B)); // RIGHT-TO-LEFT EMBEDDING
    str->remove(QChar(0x202C)); // POP DIRECTIONAL FORMATTING
}
#endif

#ifdef HAVE_ICU
void MLocalePrivate::fixFormattedNumberForRTL(QString *formattedNumber) const
{
    // see https://projects.maemo.org/bugzilla/show_bug.cgi?id=232757
    // and http://comments.gmane.org/gmane.comp.internationalization.bidi/2
    //
    // If the user interface (lc_messages) is Arabic or Persian,
    // make formatted numbers with percents, minus-signs,... at the end
    // or the beginning reorder correctly, even if a string starts with
    // such a number and even if the number is alone in a label.
    QString categoryNameMessages = categoryName(MLocale::MLcMessages);
    if(!categoryNameMessages.startsWith(QLatin1String("ar"))
       && !categoryNameMessages.startsWith(QLatin1String("fa")))
        return;
    // Some number formats, for example Persian, already contain some
    // markup for bidi reordering, although it doesn’t seem to be right.
    // Remove all already existing markers first to avoid conflicts
    // with the markers I insert:
    removeDirectionalFormattingCodes(formattedNumber);
    // Now insert a RIGHT-TO-LEFT MARK before the first EN or AN
    // and another one after the last EN or AN:
    int first = -1;
    int last = -1;
    for(int i = 0; i < formattedNumber->size(); ++i) {
        switch(formattedNumber->at(i).direction()) {
        case QChar::DirEN:
        case QChar::DirAN:
            last = i;
            if(first == -1)
                first = i;
            break;
        default:
            break;
        }
    }
    if(last != -1)
        formattedNumber->insert(last+1, QChar(0x200F)); // RIGHT-TO-LEFT MARK
    if(first != -1)
        formattedNumber->insert(first, QChar(0x200F)); // RIGHT-TO-LEFT MARK
    return;
}
#endif

#ifdef HAVE_ICU
QString MLocale::formatPercent(double i, int decimals) const
{
    Q_D(const MLocale);
    QString categoryNameNumeric
        = d->fixCategoryNameForNumbers(categoryName(MLocale::MLcNumeric));
    icu::Locale numericLocale = icu::Locale(qPrintable(categoryNameNumeric));
    UErrorCode status = U_ZERO_ERROR;
    icu::NumberFormat *nf = NumberFormat::createPercentInstance(numericLocale, status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed";
        return QString();
    }

    nf->setMinimumFractionDigits(decimals);
    icu::UnicodeString str;
    nf->format(i, str);
    delete nf;
    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
}
#endif

QString MLocale::formatCurrency(double amount, const QString &currency) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    QString monetaryCategoryName = d->fixCategoryNameForNumbers(categoryName(MLcMonetary));
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale monetaryLocale = icu::Locale(qPrintable(monetaryCategoryName));
    icu::NumberFormat *nf = icu::NumberFormat::createCurrencyInstance(monetaryLocale, status);

    if (!U_SUCCESS(status)) {
        qWarning() << "icu::NumberFormat::createCurrencyInstance failed with error"
                   << u_errorName(status);
        return QString();
    }

    icu::UnicodeString currencyString = MIcuConversions::qStringToUnicodeString(currency);
    nf->setCurrency(currencyString.getTerminatedBuffer(), status);

    if (!U_SUCCESS(status)) {
        qWarning() << "icu::NumberFormat::setCurrency failed with error"
                   << u_errorName(status);
        delete nf;
        return QString();
    }

    icu::UnicodeString str;
    nf->format(amount, str);
    delete nf;
    QString result = MIcuConversions::unicodeStringToQString(str);
    d->fixFormattedNumberForRTL(&result);
    return result;
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcMonetary).toString(amount) + ' ' + currency;
#endif
}

QString MLocale::formatDateTime(const QDateTime &dateTime, DateType dateType,
                                  TimeType timeType, CalendarType calendarType) const
{
#ifdef HAVE_ICU
    MCalendar calendar(calendarType);
    calendar.setDateTime(dateTime);
    return formatDateTime(calendar, dateType, timeType);
#else
    Q_UNUSED(dateType);
    Q_UNUSED(timeType);
    Q_UNUSED(calendarType);
    Q_D(const MLocale);
    return d->createQLocale(MLcTime).toString(dateTime);
#endif
}

#ifdef HAVE_ICU
QString MLocale::formatDateTime(const MCalendar &mcalendar,
                                  DateType datetype, TimeType timetype) const
{
    if (datetype == DateNone && timetype == TimeNone)
        return QString("");

    Q_D(const MLocale);

    icu::FieldPosition pos;
    icu::UnicodeString resString;
    icu::Calendar *cal = mcalendar.d_ptr->_calendar;

    icu::DateFormat *df = d->createDateFormat(datetype, timetype,
                                              mcalendar.type(),
                                              d->_timeFormat24h);
    if(df)
        df->format(*cal, resString, pos);
    QString result = MIcuConversions::unicodeStringToQString(resString);
    return result;
}
#endif

#ifdef HAVE_ICU
QString MLocale::formatDateTime(const QDateTime &dateTime, CalendarType calendarType) const
{
    return formatDateTime(dateTime, DateLong, TimeLong, calendarType);
}
#endif

#ifdef HAVE_ICU
QString MLocale::formatDateTime(const QDateTime &dateTime,
                                  const QString &formatString) const
{
    Q_D(const MLocale);

    // convert QDateTime to MCalendar and format

    if ( ! d->_pDateTimeCalendar )
    {
        const_cast<MLocalePrivate *>(d)->_pDateTimeCalendar = new MCalendar( *this );
    }

    const_cast<MLocalePrivate *>(d)->_pDateTimeCalendar->setDateTime(dateTime);
    return formatDateTime(*d->_pDateTimeCalendar, formatString);
}
#endif

#ifdef HAVE_ICU
//! creates a string presentation for a QDateTime with specific format string
//! \param dateTime QDateTime to format
//! \param formatString in ICU SimpleDateFormat format
//! \note THIS MAY BE REMOVED FROM PUBLIC API
QString MLocale::formatDateTimeICU(const QDateTime &dateTime,
                                     const QString &formatString) const
{
    Q_D(const MLocale);

    // convert QDateTime to MCalendar and format

    if ( ! d->_pDateTimeCalendar )
    {
        const_cast<MLocalePrivate *>(d)->_pDateTimeCalendar = new MCalendar( *this );
    }

    const_cast<MLocalePrivate *>(d)->_pDateTimeCalendar->setDateTime(dateTime);
    return formatDateTimeICU(*d->_pDateTimeCalendar, formatString);
}
#endif

#ifdef HAVE_ICU
//! Formats the date time with ICU pattern
//! \note THIS MAY BE REMOVED FROM PUBLIC API
QString MLocale::formatDateTimeICU(const MCalendar &mCalendar,
                                     const QString &formatString) const
{
    Q_D(const MLocale);
    QString categoryNameTime = categoryName(MLocale::MLcTime);
    QString categoryNameNumeric = categoryName(MLocale::MLcNumeric);
    QString categoryNameMessages = categoryName(MLocale::MLcMessages);
    QString key = QString("%1_%2_%3_%4_%5")
        .arg(formatString)
        .arg(mCalendar.type())
        .arg(categoryNameTime)
        .arg(categoryNameNumeric)
        .arg(categoryNameMessages);
    categoryNameTime = d->fixCategoryNameForNumbers(
        MIcuConversions::setCalendarOption(categoryNameTime, mCalendar.type()));
    categoryNameMessages = d->fixCategoryNameForNumbers(
        MIcuConversions::setCalendarOption(categoryNameMessages, mCalendar.type()));
    icu::SimpleDateFormat *formatter;
    if(d->_simpleDateFormatCache.contains(key)) {
        formatter = d->_simpleDateFormatCache.object(key);
    }
    else {
        UErrorCode status = U_ZERO_ERROR;
        formatter = new icu::SimpleDateFormat(
            MIcuConversions::qStringToUnicodeString(formatString),
            icu::Locale(qPrintable(categoryNameTime)), status);
        if(U_FAILURE(status)) {
            qWarning() << "icu::SimpleDateFormat() failed with error"
                       << u_errorName(status);
            formatter = NULL;
        }
        if (formatter && !categoryNameTime.contains(QRegExp("@.*mix-time-and-language=no"))) {
            // mixing in symbols like month name and weekday name from the message locale
            DateFormatSymbols *dfs =
                MLocalePrivate::createDateFormatSymbols(
                    icu::Locale(qPrintable(categoryNameMessages)));
            formatter->adoptDateFormatSymbols(dfs);
         }
        if(formatter)
            d->_simpleDateFormatCache.insert(key, formatter);
    }
    if(!formatter) {
        return QString();
    }
    else {
        icu::FieldPosition pos;
        icu::UnicodeString resString;
        formatter->format(*mCalendar.d_ptr->_calendar, resString, pos);
        return MIcuConversions::unicodeStringToQString(resString);
    }
}
#endif

#ifdef HAVE_ICU
// return weeknumber based on first week starting from the first given weekday of the year
// i.e. using sunday week 1 is the first week that contains sunday, zero before it
// note: week also starts from given weekday
// TODO: should this be moved to MCalendar?
static int weekNumberStartingFromDay(const MCalendar &calendar, int weekday)
{
    MCalendar calendarCopy = calendar;
    calendarCopy.setFirstDayOfWeek(weekday);
    calendarCopy.setMinimalDaysInFirstWeek(1);
    // this is icu week number, starts from 1
    int weeknumber = calendarCopy.weekNumber();

    // check if there's week 0
    bool weekZero = true;
    int year = calendarCopy.year();
    calendarCopy.setDate(year, 1, 1); // reuse the copy

    // a bit crude. check if the first week includes sunday
    // note: should start always from week 1 because minimal days is 1.
    while (calendarCopy.weekOfYear() == 1) {
        if (calendarCopy.dayOfWeek() == weekday) {
            weekZero = false;
        }

        calendarCopy.addDays(1);
    }

    if (weekZero == true) {
        weeknumber--;
    }

    return weeknumber;
}
#endif

#ifdef HAVE_ICU
QString MLocale::formatDateTime(const MCalendar &mCalendar,
                                  const QString &formatString) const
{
    Q_D(const MLocale);
    // convert POSIX format string into ICU format

    QString icuFormat;

    if ( ! d->_icuFormatStringCache.contains( formatString ) )
    {
        // determine if we can cache this format string, or if
        // we have to add something to it that is a part of a date or time.
        bool canCacheIcuFormat = true;

        bool isInNormalText = false; // a-zA-Z should be between <'>-quotations

        const int length = formatString.length();
        for (int i = 0; i < length; ++i) {

            QChar current = formatString.at(i);

            if (current == '%') {
                i++;
                QChar next = formatString.at(i);

                // end plain text icu quotation
                if (isInNormalText == true) {
                    icuFormat.append('\'');
                    isInNormalText = false;
                }

                switch (next.unicode()) {

                    case 'a':
                        // abbreviated weekday name
                        icuFormat.append("ccc");
                        break;

                    case 'A':
                        // stand-alone full weekday name
                        icuFormat.append("cccc");
                        break;

                    case 'b':
                    case 'h':
                        // abbreviated month name
                        icuFormat.append("LLL");
                    break;

                    case 'B':
                        // full month name
                        icuFormat.append("LLLL");
                        break;

                    case 'c': {
                        // FDCC-set's appropriate date and time representation

                        // This is ugly but possibly the only way to get the appropriate presentation
                        icu::Locale msgLocale = d->getCategoryLocale(MLcMessages);
                        DateFormat *df
                            = icu::DateFormat::createDateTimeInstance(icu::DateFormat::kDefault,
                                                                      icu::DateFormat::kDefault,
                                                                      msgLocale);
                        icu::UnicodeString dateTime;
                        icu::FieldPosition fieldPos;
                        dateTime = df->format(*mCalendar.d_ptr->_calendar, dateTime, fieldPos);
                        icuFormat.append('\'');
                        QString pattern = MIcuConversions::unicodeStringToQString(dateTime);
                        icuFormat.append(MIcuConversions::icuDatePatternEscaped(pattern));
                        icuFormat.append('\'');
                        delete df;

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'C': {
                        // century, no corresponding icu pattern
                        UnicodeString str;
                        d->_numberFormatLcTime->format(static_cast<int32_t>(mCalendar.year() / 100), str); //krazy:exclude=typedefs
                        icuFormat.append(MIcuConversions::unicodeStringToQString(str));

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'd':
                        // Day of the month as a decimal number (01-31)
                        icuFormat.append("dd");
                        break;

                    case 'D':
                        // %D Date in the format mm/dd/yy.
                        icuFormat.append("MM/dd/yy"); // yy really shortened?
                        break;

                    case 'e':
                        // correct? there should be explicit space fill or something?
                        icuFormat.append("d");
                        break;

                    case 'F':
                        //The date in the format YYYY-MM-DD (An ISO 8601 format).
                        icuFormat.append("yyyy-MM-dd");
                        break;

                    case 'g':
                        icuFormat.append("YY");
                        break;

                    case 'G':
                        icuFormat.append("YYYY");
                        break;

                    case 'H':
                        // Hour (24-hour clock), as a decimal number (00-23).
                        icuFormat.append("HH");
                        break;

                    case 'I':
                        // Hour (12-hour clock), as a decimal number (01-12).
                        icuFormat.append("hh");
                        break;

                    case 'j':
                        // day of year
                        icuFormat.append("DDD");
                        break;

                    case 'm':
                        // month
                        icuFormat.append("MM");
                        break;

                    case 'M':
                        // minute
                        icuFormat.append("mm");
                        break;

                    case 'n':
                        // newline
                        icuFormat.append('\n');
                        break;

                    case 'p':
                        // AM/PM
                        icuFormat.append("aaa");
                        break;

                    case 'r': {
                        // 12 hour clock with am/pm
                        QString timeShortFormat
                            = d->icuFormatString(MLocale::DateNone, MLocale::TimeShort,
                                                 MLocale::GregorianCalendar,
                                                 MLocale::TwelveHourTimeFormat24h);
                        icuFormat.append(timeShortFormat);
                        break;
                    }

                    case 'R': {
                        // 24-hour clock time, in the format "%H:%M"
                        QString timeShortFormat
                            = d->icuFormatString(MLocale::DateNone, MLocale::TimeShort,
                                                 MLocale::GregorianCalendar,
                                                 MLocale::TwentyFourHourTimeFormat24h);
                        icuFormat.append(timeShortFormat);
                        break;
                    }

                    case 'S':
                        // seconds
                        icuFormat.append("ss");
                        break;

                    case 't':
                        // tab
                        icuFormat.append('\t');
                        break;

                    case 'T': // FIXME!
                        // 24 hour clock HH:MM:SS
                        icuFormat.append("kk:mm:ss");
                        break;

                    case 'u': {
                        // Weekday, as a decimal number (1(Monday)-7)
                        // no corresponding icu pattern for monday based weekday
                        UnicodeString str;
                        d->_numberFormatLcTime->format(static_cast<int32_t>(mCalendar.dayOfWeek()), str); //krazy:exclude=typedefs
                        icuFormat.append(MIcuConversions::unicodeStringToQString(str));

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'U': {
                        // Week number of the year (Sunday as the first day of the week) as a
                        // decimal number (00-53). First week starts from first Sunday.
                        UnicodeString str;
                        d->_numberFormatLcTime->format(static_cast<int32_t>(0), str); //krazy:exclude=typedefs
                        d->_numberFormatLcTime->format(static_cast<int32_t>(weekNumberStartingFromDay(mCalendar, MLocale::Sunday)), str); //krazy:exclude=typedefs
                        QString weeknumber = MIcuConversions::unicodeStringToQString(str);
                        if (weeknumber.length() > 2)
                            weeknumber = weeknumber.right(2);
                        icuFormat.append(weeknumber);

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'v': // same as %V, for compatibility
                    case 'V': {
                        // Week of the year (Monday as the first day of the week), as a decimal
                        // number (01-53). according to ISO-8601
                        MCalendar calendarCopy = mCalendar;
                        calendarCopy.setFirstDayOfWeek(MLocale::Monday);
                        calendarCopy.setMinimalDaysInFirstWeek(4);
                        UnicodeString str;
                        d->_numberFormatLcTime->format(static_cast<int32_t>(0), str); //krazy:exclude=typedefs
                        d->_numberFormatLcTime->format(static_cast<int32_t>(calendarCopy.weekNumber()), str); //krazy:exclude=typedefs
                        QString weeknumber = MIcuConversions::unicodeStringToQString(str);
                        if (weeknumber.length() > 2)
                            weeknumber = weeknumber.right(2); // cut leading 0
                        icuFormat.append(weeknumber);

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'w': {
                        // Weekday, as a decimal number (0(Sunday)-6)
                        int weekday = mCalendar.dayOfWeek();
                        if (weekday == Sunday) {
                            weekday = 0;
                        }
                        UnicodeString str;
                        d->_numberFormatLcTime->format(static_cast<int32_t>(weekday), str); //krazy:exclude=typedefs
                        icuFormat.append(MIcuConversions::unicodeStringToQString(str));

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'W': {
                        // Week number of the year (Monday as the first day of the week), as a
                        // decimal number (00-53). Week starts from the first monday
                        int weeknumber = weekNumberStartingFromDay(mCalendar, MLocale::Monday);
                        UnicodeString str;
                        d->_numberFormatLcTime->format(static_cast<int32_t>(weeknumber), str); //krazy:exclude=typedefs
                        icuFormat.append(MIcuConversions::unicodeStringToQString(str));

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'x': {
                        // appropriate date representation
                        icu::Locale msgLocale = d->getCategoryLocale(MLcMessages);
                        DateFormat *df
                            = icu::DateFormat::createDateInstance(icu::DateFormat::kDefault,
                                                                  msgLocale);
                        icu::UnicodeString dateTime;
                        icu::FieldPosition fieldPos;
                        dateTime = df->format(*mCalendar.d_ptr->_calendar, dateTime, fieldPos);
                        icuFormat.append('\'');
                        QString pattern = MIcuConversions::unicodeStringToQString(dateTime);
                        icuFormat.append(MIcuConversions::icuDatePatternEscaped(pattern));
                        icuFormat.append('\'');
                        delete df;

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'X': {
                        // appropriate time representation
                        icu::Locale msgLocale = d->getCategoryLocale(MLcMessages);
                        DateFormat *df
                            = icu::DateFormat::createTimeInstance(icu::DateFormat::kDefault,
                                                                  msgLocale);
                        icu::UnicodeString dateTime;
                        icu::FieldPosition fieldPos;
                        dateTime = df->format(*mCalendar.d_ptr->_calendar, dateTime, fieldPos);
                        icuFormat.append('\'');
                        QString pattern = MIcuConversions::unicodeStringToQString(dateTime);
                        icuFormat.append(MIcuConversions::icuDatePatternEscaped(pattern));
                        icuFormat.append('\'');
                        delete df;

                        canCacheIcuFormat = false;
                        break;
                    }

                    case 'y':
                        // year within century
                        icuFormat.append("yy");
                        break;

                    case 'Y':
                        // year with century
                        icuFormat.append("yyyy");
                        break;

                    case 'z':
                        // The offset from UTC in the ISO 8601 format "-0430" (meaning 4 hours
                        // 30 minutes behind UTC, west of Greenwich), or by no characters if no
                        // time zone is determinable
                        icuFormat.append("Z"); // correct?
                        break;

                    case 'Z':
                        // ISO-14652 (draft):
                        //   Time-zone name, or no characters if no time zone is determinable
                        // Linux date command, strftime (glibc):
                        //   alphabetic time zone abbreviation (e.g., EDT)
                        // note that the ISO-14652 draft does not mention abbreviation,
                        // i.e. it is a bit unclear how exactly this should look like.
                        icuFormat.append("vvvv"); // generic time zone info
                        break;

                    case '%':
                        icuFormat.append("%");
                        break;
                }

            } else {
                if (current == '\'') {
                    icuFormat.append("''"); // icu escape

                } else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')) {
                    if (isInNormalText == false) {
                        icuFormat.append('\'');
                        isInNormalText = true;
                    }

                    icuFormat.append(current);

                } else {
                    icuFormat.append(current);
                }
            }
        } // for loop

        // save formatString -> icuFormat pair for future use,
        // if it does not contain content from the input date or time
        if ( canCacheIcuFormat )
        {
            QString* value = new QString( icuFormat );

            d->_icuFormatStringCache.insert( formatString, value );
        }
    }
    else
    {
        // formatString does exist in hash
        icuFormat = *d->_icuFormatStringCache[ formatString ];
    }

    return formatDateTimeICU(mCalendar, icuFormat);
}
#endif

#ifdef HAVE_ICU
QString MLocale::icuFormatString( DateType dateType,
                          TimeType timeType,
                          CalendarType calendarType) const
{
    Q_D(const MLocale);
    return d->icuFormatString(dateType, timeType, calendarType,
                              d->_timeFormat24h);
}
#endif

#ifdef HAVE_ICU
QDateTime MLocale::parseDateTime(const QString &dateTime, DateType dateType,
                                   TimeType timeType, CalendarType calendarType) const
{
    if (dateType == DateNone && timeType == TimeNone)
        return QDateTime();

    Q_D(const MLocale);
    MCalendar mcalendar(calendarType);

    UnicodeString text = MIcuConversions::qStringToUnicodeString(dateTime);
    icu::DateFormat *df = d->createDateFormat(dateType, timeType,
                                              mcalendar.type(),
                                              d->_timeFormat24h);
    icu::ParsePosition pos(0);
    UDate parsedDate;
    if (df)
        parsedDate = df->parse(text, pos);
    else
        return QDateTime();

    UErrorCode status = U_ZERO_ERROR;
    icu::Calendar *cal = mcalendar.d_ptr->_calendar;
    cal->setTime(parsedDate, status);
    if (status != U_ZERO_ERROR)
        return QDateTime();

    return mcalendar.qDateTime();
}
#endif

#ifdef HAVE_ICU
QDateTime MLocale::parseDateTime(const QString &dateTime, CalendarType calendarType) const
{
    return parseDateTime(dateTime, DateLong, TimeLong, calendarType);
}
#endif

#ifdef HAVE_ICU
QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber) const
{
    return monthName(mCalendar, monthNumber, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide);
}
#endif

#ifdef HAVE_ICU
QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber,
                             DateSymbolContext context,
                             DateSymbolLength symbolLength) const
{
    Q_D(const MLocale);

    monthNumber--; // months in array starting from index zero

    QString categoryName = d->categoryName(MLcTime);
    if(!categoryName.contains(QRegExp("@.*mix-time-and-language=no")))
        categoryName = d->categoryName(MLcMessages);
    categoryName = MIcuConversions::setCalendarOption(categoryName, mCalendar.type());
    icu::Locale symbolLocale = icu::Locale(qPrintable(categoryName));

    icu::DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    // map context type
    icu::DateFormatSymbols::DtContextType icuContext =
        MIcuConversions::mDateContextToIcu(context);

    // map length type
    icu::DateFormatSymbols::DtWidthType icuWidth =
        MIcuConversions::mDateWidthToIcu(symbolLength);

    int len = -1;
    const UnicodeString *months = dfs->getMonths(len, icuContext, icuWidth);

    QString result;

    if (len > 0 && monthNumber < len && monthNumber >= 0) {
        result = MIcuConversions::unicodeStringToQString(months[monthNumber]);
    }

    delete dfs;

    if(!result.isEmpty() && context == MLocale::DateSymbolStandalone)
        result[0] = toUpper(result.at(0))[0];
    return result;
}
#endif

#ifdef HAVE_ICU
QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday) const
{
    return weekdayName(mCalendar, weekday, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide);
}
#endif

#ifdef HAVE_ICU
QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday,
                               DateSymbolContext context,
                               DateSymbolLength symbolLength) const
{
    Q_D(const MLocale);
    QString categoryName = d->categoryName(MLcTime);
    if(!categoryName.contains(QRegExp("@.*mix-time-and-language=no")))
        categoryName = d->categoryName(MLcMessages);
    categoryName = MIcuConversions::setCalendarOption(categoryName, mCalendar.type());
    icu::Locale symbolLocale = icu::Locale(qPrintable(categoryName));

    icu::DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    icu::DateFormatSymbols::DtContextType icuContext
    = MIcuConversions::mDateContextToIcu(context);

    icu::DateFormatSymbols::DtWidthType icuWidth
    = MIcuConversions::mDateWidthToIcu(symbolLength);

    int len = -1;
    const UnicodeString *weekdayNames = dfs->getWeekdays(len, icuContext, icuWidth);
    int weekdayNum = MIcuConversions::icuWeekday(weekday);

    QString result;

    if (len > 0 && weekdayNum < len && weekdayNum > 0) {
        result = MIcuConversions::unicodeStringToQString(weekdayNames[weekdayNum]);
    }

    delete dfs;

    if(!result.isEmpty() && context == MLocale::DateSymbolStandalone)
        result[0] = toUpper(result.at(0))[0];
    return result;
}
#endif

QString MLocale::languageEndonym() const
{
#ifdef HAVE_ICU
    return languageEndonym(this->name());
#else
    Q_D(const MLocale);
    return QLocale::languageToString(d->createQLocale(MLcMessages).language());
#endif
}

QString MLocale::countryEndonym() const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    QString resourceBundleLocaleName = d->_defaultLocale;
    QString countryCode = country();
    if (countryCode.isEmpty())
        return QString();

    do {
        // Trying several resource bundles is a workaround for
        // http://site.icu-project.org/design/resbund/issues
        UErrorCode status = U_ZERO_ERROR;
        UResourceBundle *res = ures_open(U_ICUDATA_NAME "-region",
                                         qPrintable(resourceBundleLocaleName),
                                         &status);
        if (U_FAILURE(status)) {
            mDebug("MLocale") << "Error ures_open" << u_errorName(status);
            ures_close(res);
            return countryCode;
        }
        res = ures_getByKey(res, Countries, res, &status);
        if (U_FAILURE(status)) {
            mDebug("MLocale") << "Error ures_getByKey" << u_errorName(status);
            ures_close(res);
            return countryCode;
        }
        int len;
        const UChar *val = ures_getStringByKey(res,
                                               countryCode.toStdString().c_str(),
                                               &len,
                                               &status);
        ures_close(res);
        if (U_SUCCESS(status)) {
            // found country endonym, return it:
            return QString::fromUtf16(val, len);
        }
    } while (d->truncateLocaleName(&resourceBundleLocaleName));
    // no country endonym found and there are not other resource
    // bundles left to try, return the country code as a fallback:
    return countryCode;
#else
    Q_D(const MLocale);
    return QLocale::countryToString(d->createQLocale(MLcMessages).country());
#endif
}

#ifdef HAVE_ICU
QStringList MLocale::exemplarCharactersIndex() const
{
    Q_D(const MLocale);
    QString collationLocaleName = d->categoryName(MLcCollate);
    QStringList exemplarCharactersIndex;

    // special treatment for Chinese locales because these have the
    // collation options "stroke" and "pinyin" which require different
    // index buckets.  But libicu currently supports only one index
    // bucket list per locale.  As a workaround, force use of the
    // index bucket list from the zh_TW locale if collation=stroke is
    // set and force the use of the index bucket list from the zh_CN
    // locale if collation=pinyin is set:
    if(collationLocaleName.startsWith(QLatin1String("zh"))) {
        if(collationLocaleName.contains(QLatin1String("collation=stroke")))
            collationLocaleName = QLatin1String("zh_TW");
        if(collationLocaleName.contains(QLatin1String("collation=pinyin")))
            collationLocaleName = QLatin1String("zh_CN");
    }

    UErrorCode status = U_ZERO_ERROR;
    UResourceBundle *res =
        ures_open(NULL, collationLocaleName.toUtf8().constData(), &status);

    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_open"
                          << collationLocaleName << u_errorName(status);
        ures_close(res);
        return exemplarCharactersIndex;
    }

    qint32 len;
    status = U_ZERO_ERROR;
    const UChar *val = ures_getStringByKey(res,
                                           "ExemplarCharactersIndex",
                                           &len, &status);
    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_getStringByKey"
                          << collationLocaleName << u_errorName(status);
        ures_close(res);
        return exemplarCharactersIndex;
    }

    QString charStr = QString::fromUtf16(val, len);
    ures_close(res);
    charStr.remove('[');
    charStr.remove(']');
    charStr.remove('{');
    charStr.remove('}');
    exemplarCharactersIndex = charStr.split(QLatin1String(" "),
                                            QString::SkipEmptyParts);

    // Special hack for the last Japanese bucket:
    if (exemplarCharactersIndex.last() == QString::fromUtf8("わ")) {
        exemplarCharactersIndex << QString::fromUtf8("ん"); // to get ワ, ゐ,ヰ,ヸ, ヹ, を, ヲ, ヺ, into the わ bucket
    }
    // Special hack for the last Korean bucket:
    if (exemplarCharactersIndex.last() == QString::fromUtf8("ᄒ")) {
        exemplarCharactersIndex << QString::fromUtf8("あ"); // to get 학,  學, ... ᄒ bucket
    }

    return exemplarCharactersIndex;
}
#endif

#ifdef HAVE_ICU
QString MLocale::indexBucket(const QString &str, const QStringList &buckets, const MCollator &coll) const
{
    Q_D(const MLocale);
    if (str.isEmpty())
        return str;
    if (str.startsWith(QString::fromUtf8("ン")) && buckets.last() == QString::fromUtf8("ん")) {
        // ン sorts after ん but should go into the ん bucket:
        return QString::fromUtf8("ん");
    }
    QString strUpperCase =
        MIcuConversions::unicodeStringToQString(
            MIcuConversions::qStringToUnicodeString(str).toUpper(
                d->getCategoryLocale(MLocale::MLcCollate)));
    QString firstCharacter;
    if (strUpperCase.at(0).isHighSurrogate())
        firstCharacter = strUpperCase.at(0) + strUpperCase.at(1);
    else
        firstCharacter = strUpperCase.at(0);
    for (int i = 0; i < buckets.size(); ++i) {
        if (coll(strUpperCase, buckets[i])) {
            if (i == 0) {
                return firstCharacter;
            }
            else {
                return buckets[i-1];
            }
        }
    }
    return firstCharacter;
}
#endif

#ifdef HAVE_ICU
QString MLocale::indexBucket(const QString &str) const
{
    QStringList bucketList = exemplarCharactersIndex();
    return indexBucket(str, bucketList, this->collator());
}
#endif

#ifdef HAVE_ICU
QStringList MLocale::localeScripts() const
{
    Q_D(const MLocale);
    UErrorCode status = U_ZERO_ERROR;

    UResourceBundle *res = ures_open(NULL, qPrintable(d->_defaultLocale), &status);

    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_open" << u_errorName(status);
    }

    res = ures_getByKey(res, "LocaleScript", res, &status);
    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_getByKey" << u_errorName(status);
    }

    QStringList scripts;
    qint32 len;
    const UChar *val;

    while (NULL != (val = ures_getNextString(res, &len, NULL, &status))) {
        if (U_SUCCESS(status))
            scripts << QString::fromUtf16(val, len);
    }
    ures_close(res);
    if (scripts.isEmpty())
        // "Zyyy" Code for undetermined script,
        // see http://www.unicode.org/iso15924/iso15924-codes.html
        scripts << "Zyyy";

    return scripts;
}
#endif

void MLocale::copyCatalogsFrom(const MLocale &other)
{
    Q_D(MLocale);

    MLocalePrivate::CatalogList::const_iterator end =
        other.d_ptr->_messageTranslations.constEnd();

    for (MLocalePrivate::CatalogList::const_iterator i
            = other.d_ptr->_messageTranslations.constBegin();
            i != end; ++i) {

        // copy the name info
        MTranslationCatalog *tempCatalog = new MTranslationCatalog(**i);

        // reload the file
        tempCatalog->loadWith(this, MLcMessages);
        d->_messageTranslations.append(QExplicitlySharedDataPointer<MTranslationCatalog>(tempCatalog));

    }

    end = other.d_ptr->_timeTranslations.constEnd();
    for (MLocalePrivate::CatalogList::const_iterator i = other.d_ptr->_timeTranslations.constBegin();
            i != end; ++i) {

        MTranslationCatalog *tempCatalog = new MTranslationCatalog(**i);
        tempCatalog->loadWith(this, MLcTime);
        d->_timeTranslations.append(QExplicitlySharedDataPointer<MTranslationCatalog>(tempCatalog));
    }

    end = other.d_ptr->_trTranslations.constEnd();
    for (MLocalePrivate::CatalogList::const_iterator i = other.d_ptr->_trTranslations.constBegin();
            i != end; ++i) {

        MTranslationCatalog *tempCatalog = new MTranslationCatalog(**i);
        tempCatalog->loadWith(this, MLcMessages);
        d->_trTranslations.append(QExplicitlySharedDataPointer<MTranslationCatalog>(tempCatalog));

    }

}

void MLocale::installTrCatalog(const QString &name)
{
    Q_D(MLocale);

    // Make sure that previous installations of a catalog are removed
    // first before trying to install a catalog.  There is no need to
    // install the same catalog more then once with different
    // priorities.  One could skip the installation altogether if the
    // catalog is already installed, but it is better to remove the
    // first instance, then the priorities make more sense.
    // See https://projects.maemo.org/bugzilla/show_bug.cgi?id=198551
    removeTrCatalog(name);

    MTranslationCatalog *catalog
        = new MTranslationCatalog(name);
    catalog->loadWith(this, MLocale::MLcMessages);
    d->_trTranslations.append(QExplicitlySharedDataPointer<MTranslationCatalog>(catalog));
    if (!name.endsWith(QLatin1String(".qm"))) {
        MTranslationCatalog *engineeringEnglishCatalog
            = new MTranslationCatalog(name + ".qm");
        engineeringEnglishCatalog->loadWith(this, MLocale::MLcMessages);
        d->_trTranslations.prepend(QExplicitlySharedDataPointer<MTranslationCatalog>(engineeringEnglishCatalog));
    }
}

void MLocale::removeTrCatalog(const QString &name)
{
    Q_D(MLocale);
    MLocalePrivate::CatalogList::iterator it = d->_trTranslations.begin();
    while (it != d->_trTranslations.end()) {
        if ((*it)->_name == name || (*it)->_name == name + ".qm") {
            // Reset to null but first decrement reference count of
            // the shared data object and delete the shared data
            // object if the reference count became 0:
            (*it).reset();
            it = d->_trTranslations.erase(it);
        }
        else
            ++it;
    }
}

bool MLocale::isInstalledTrCatalog(const QString &name) const
{
    Q_D(const MLocale);
    if(name.isEmpty())
        return false;
    MLocalePrivate::CatalogList::const_iterator it = d->_trTranslations.constBegin();
    while (it != d->_trTranslations.constEnd()) {
        if ((*it)->_name == name)
            return true;
        else
            ++it;
    }
    return false;
}

/////////////////////////////
//// translation methods ////

QString MLocale::translate(const char *context, const char *sourceText,
                             const char *comment, int n)
{
    Q_D(MLocale);
    const MLocalePrivate::CatalogList::const_iterator begin = d->_trTranslations.constBegin();
    MLocalePrivate::CatalogList::const_iterator it = d->_trTranslations.constEnd();
    while (it != begin) {
        --it;

        QString translation = (*it)->_translator.translate(context, sourceText,
                              comment, n);

        if (translation.isEmpty() == false) {
            replacePercentN(&translation, n);
            return translation;
        }
    }

    return sourceText;
}

void MLocale::setDataPaths(const QStringList &dataPaths)
{
    MLocalePrivate::dataPaths = dataPaths;
#ifdef HAVE_ICU
    QString pathString;

    foreach(QString string, dataPaths) { // krazy:exclude=foreach
        string.replace('/', U_FILE_SEP_CHAR);
        pathString.append(string);
        // separator gets appended to the end of the list. I hope Icu doesn't mind
        pathString.append(U_PATH_SEP_CHAR);
    }

    u_setDataDirectory(qPrintable(pathString));
#endif
}

// convenience method for just one path
void MLocale::setDataPath(const QString &dataPath)
{
    setDataPaths(QStringList(dataPath));
}

QStringList MLocale::dataPaths()
{
    return MLocalePrivate::dataPaths;
}


#ifdef HAVE_ICU
QString MLocale::localeScript(const QString &locale)
{
    QString s = MLocalePrivate::parseScript(locale);

    if(!s.isEmpty())
        return s;

    UErrorCode status = U_ZERO_ERROR;
    UResourceBundle *res = ures_open(NULL, qPrintable(locale), &status);
    if(U_FAILURE(status)) // TODO: error handling++
        return QString();

    res = ures_getByKey(res, "LocaleScript", res, &status);
    if(U_FAILURE(status)) {
        ures_close(res);
        return QString();
    }

    QString ret("Zyyy");

    qint32 len;
    const UChar *v;
    v = ures_getNextString(res, &len, NULL, &status);
    if(v && U_SUCCESS(status))
        ret = QString::fromUtf16(v, len);

    ures_close(res);

    return ret;
}

QString MLocale::languageEndonym(const QString &locale)
{
    QString resourceBundleLocaleName = locale;

    do {
        // Trying several resource bundles is a workaround for
        // http://site.icu-project.org/design/resbund/issues
        UErrorCode status = U_ZERO_ERROR;
        UResourceBundle *res = ures_open(U_ICUDATA_NAME "-lang",
                                         qPrintable(resourceBundleLocaleName),
                                         &status);
        if (U_FAILURE(status)) {
            mDebug("MLocale") << "Error ures_open" << u_errorName(status);
            ures_close(res);
            return locale;
        }
        res = ures_getByKey(res, Languages, res, &status);
        if (U_FAILURE(status)) {
            mDebug("MLocale") << "Error ures_getByKey" << u_errorName(status);
            ures_close(res);
            return locale;
        }
        QString keyLocaleName = locale;
        // it’s not nice if “zh_CN”, “zh_HK”, “zh_MO”, “zh_TW” all fall back to
        // “zh” for the language endonym and display only “中文”.
        // To make the fallbacks work better, insert the script:
        if (keyLocaleName.startsWith(QLatin1String("zh_CN")))
            keyLocaleName = "zh_Hans_CN";
        else if (keyLocaleName.startsWith(QLatin1String("zh_SG")))
            keyLocaleName = "zh_Hans_SG";
        else if (keyLocaleName.startsWith(QLatin1String("zh_HK")))
            keyLocaleName = "zh_Hant_HK";
        else if (keyLocaleName.startsWith(QLatin1String("zh_MO")))
            keyLocaleName = "zh_Hant_MO";
        else if (keyLocaleName.startsWith(QLatin1String("zh_TW")))
            keyLocaleName = "zh_Hant_TW";
        do { // FIXME: this loop should probably be somewhere else
            int len;
            status = U_ZERO_ERROR;
            const UChar *val = ures_getStringByKey(res,
                                                   qPrintable(keyLocaleName),
                                                   &len,
                                                   &status);
            if (U_SUCCESS(status)) {
                // found language endonym, return it:
                ures_close(res);
                return QString::fromUtf16(val, len);
            }
        } while (MLocalePrivate::truncateLocaleName(&keyLocaleName));
        // no language endonym found in this resource bundle and there
        // is no way to shorten keyLocaleName, try the next resource
        // bundle:
        ures_close(res);
    } while (MLocalePrivate::truncateLocaleName(&resourceBundleLocaleName));
    // no language endonym found at all, no other keys or resource
    // bundles left to try, return the full locale name as a fallback:
    return locale;
}
#endif

///////
// the static convenience methods for translation

void MLocale::setTranslationPaths(const QStringList &paths)
{
    MLocalePrivate::translationPaths = paths;
}

void MLocale::addTranslationPath(const QString &path)
{
    if (MLocalePrivate::translationPaths.contains(path) == false) {
        MLocalePrivate::translationPaths << path;
    }
}

void MLocale::removeTranslationPath(const QString &path)
{
    MLocalePrivate::translationPaths.removeOne(path);
}

QStringList MLocale::translationPaths()
{
    return MLocalePrivate::translationPaths;
}

Qt::LayoutDirection MLocale::defaultLayoutDirection()
{
    return _defaultLayoutDirection;
}

Qt::LayoutDirection MLocale::textDirection() const
{
#ifdef HAVE_ICU
    Qt::LayoutDirection layoutDirectionOption =
        MIcuConversions::parseLayoutDirectionOption(this->name());
#else
    Qt::LayoutDirection layoutDirectionOption = Qt::LeftToRight;
#endif

    if (layoutDirectionOption == Qt::LayoutDirectionAuto) {
        // choose the layout direction automatically depending on the
        // script used by the locale (old behaviour of this function):
        //
        // Checking for the script "arab" is needed for
        // locales where the language can be written in several scripts.
        // Eg the Uyghur language can be written in Chinese, Cyrillic,
        // or Arabic script.
        if (script().contains("arab", Qt::CaseInsensitive))
            layoutDirectionOption = Qt::RightToLeft;
        else if (!language().isEmpty()
                 && RtlLanguages.contains(language() + ':'))
            layoutDirectionOption = Qt::RightToLeft;
        else
            layoutDirectionOption = Qt::LeftToRight;
    }
    return layoutDirectionOption;
}

Qt::LayoutDirection MLocale::directionForText(const QString & text)
{
    QString::const_iterator textEnd = text.constEnd();

    for ( QString::const_iterator it = text.constBegin(); it != textEnd; ++it ) {
        // Taken from qtextobject.cpp
        switch(QChar::direction(it->unicode()))
        {
            case QChar::DirL:
                return Qt::LeftToRight;
            case QChar::DirR:
            case QChar::DirAL:
                return Qt::RightToLeft;
            default:
                break;
        }
    }
    return Qt::LeftToRight;
}

void MLocale::refreshSettings()
{
#ifdef HAVE_GCONF
    Q_D(MLocale);

    bool settingsHaveReallyChanged = false;
    QString localeName = d->currentLanguageItem.value("en_GB").toString();
    QString lcTime = d->currentLcTimeItem.value("en_GB").toString();
    QString lcTimeFormat24h = d->currentLcTimeFormat24hItem.value("12").toString();
    QString lcCollate = d->currentLcCollateItem.value("en_GB").toString();
    QString lcNumeric = d->currentLcNumericItem.value("en_GB").toString();
    QString lcMonetary = d->currentLcMonetaryItem.value("en_GB").toString();
    QString lcTelephone = d->currentLcTelephoneItem.value().toString();

    if (localeName != d->_defaultLocale) {
        settingsHaveReallyChanged = true;
        d->_defaultLocale = localeName;
        // force recreation of the number formatter if
        // the numeric locale inherits from the default locale:
        if(d->_numericLocale.isEmpty())
            setCategoryLocale(MLcNumeric, "");
    }
    if (lcTime != d->_calendarLocale) {
        settingsHaveReallyChanged = true;
        setCategoryLocale(MLcTime, lcTime);
    }
    MLocale::TimeFormat24h timeFormat24h;
    if (lcTimeFormat24h == "24")
        timeFormat24h = MLocale::TwentyFourHourTimeFormat24h;
    else if (lcTimeFormat24h == "12")
        timeFormat24h = MLocale::TwelveHourTimeFormat24h;
    else
        timeFormat24h = MLocale::LocaleDefaultTimeFormat24h;
    if (timeFormat24h != d->_timeFormat24h) {
        settingsHaveReallyChanged = true;
        d->_timeFormat24h = timeFormat24h;
    }
    if (lcCollate != d->_collationLocale) {
        settingsHaveReallyChanged = true;
        setCategoryLocale(MLcCollate, lcCollate);
    }
    if (lcNumeric != d->_numericLocale) {
        settingsHaveReallyChanged = true;
        setCategoryLocale(MLcNumeric, lcNumeric);
    }
    if (lcMonetary != d->_monetaryLocale) {
        settingsHaveReallyChanged = true;
        setCategoryLocale(MLcMonetary, lcMonetary);
    }
    if (lcTelephone != d->_telephoneLocale) {
        settingsHaveReallyChanged = true;
        setCategoryLocale(MLcTelephone, lcTelephone);
    }

    if (settingsHaveReallyChanged) {
        if (this == s_systemDefault) {
            d->insertDirectionTrToQCoreApp();
            d->removeTrFromQCoreApp();
            d->loadTrCatalogs();
            // sends QEvent::LanguageChange to qApp:
            d->insertTrToQCoreApp();
            // Setting the default QLocale is needed to get localized number
            // support in translations via %Ln, %L1, %L2, ...:
            QLocale::setDefault(d->createQLocale(MLcNumeric));
            qApp->setLayoutDirection(this->textDirection());
            _defaultLayoutDirection = MIcuConversions::parseLayoutDirectionOption(s_systemDefault->name());
        }
        else {
            d->loadTrCatalogs();
        }
        emit settingsChanged();
    }

    d->dropCaches();
#endif
}

QString MLocale::formatPhoneNumber( const QString& phoneNumber,
                                    PhoneNumberGrouping grouping ) const
{
    Q_D(const MLocale);

    PhoneNumberGrouping tmpGrouping( grouping );

    // when called with default grouping, use the
    // system setting for the grouping
    if ( tmpGrouping == DefaultPhoneNumberGrouping )
    {
        if ( d->_telephoneLocale.startsWith( QLatin1String( "en_US" ) ) ) {
            tmpGrouping = NorthAmericanPhoneNumberGrouping;
        } else {
            tmpGrouping = NoPhoneNumberGrouping;
	}
    }

    return d->formatPhoneNumber( phoneNumber, tmpGrouping );
}

// when string starts with numbers 2 to 9
QString groupedNormalString( const QString& phoneNumber )
{
  QString result;
  QString remaining( phoneNumber );

    // for remaining number length 1 (also 0) to 3 return number unchanged
    if ( remaining.length() < 4 )
    {
      result.append( remaining );
      return result;
    }
    else if ( remaining.length() < 8 )
    {
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( '-' );
      result.append( remaining );
      return result;
    }
    else if ( remaining.length() < 11 )
    {
      result.append( '(' );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( ") " );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( '-' );
      result.append( remaining );
      return result;
    }
    else
    {
      result.append( remaining );
      return result;
    }
}

// when string starts with number 1
QString groupedOneString( const QString& phoneNumber )
{
    QString result;
    QString remaining( phoneNumber );

    // for remaining number length 1 (also 0) to 3 return number unchanged
    if ( remaining.length() < 2 )
    {
      result.append( remaining );
      return result;
    }
    else if ( remaining.length() < 3 )
    {
      result.append( remaining.left( 1 ) );
      remaining.remove( 0, 1 );
      result.append( " (" );
      result.append( remaining );
      result.append( "  )" );
      return result;
    }
    else if ( remaining.length() < 4 )
    {
      result.append( remaining.left( 1 ) );
      remaining.remove( 0, 1 );
      result.append( " (" );
      result.append( remaining );
      result.append( " )" );
      return result;
    }
    else if ( remaining.length() < 5 )
    {
      result.append( remaining.left( 1 ) );
      remaining.remove( 0, 1 );
      result.append( " (" );
      result.append( remaining );
      result.append( ')' );
      return result;
    }
    else if ( remaining.length() < 8 )
    {
      result.append( remaining.left( 1 ) );
      remaining.remove( 0, 1 );
      result.append( " (" );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( ") " );
      result.append( remaining );
      return result;
    }
    else if ( remaining.length() < 12 )
    {
      result.append( remaining.left( 1 ) );
      remaining.remove( 0, 1 );
      result.append( " (" );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( ") " );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( '-' );
      result.append( remaining );
      return result;
    }
    else
    {
      result.append( remaining );
      return result;
    }
}

// when string starts with numbers 2 to 9
QString groupedInternationalString( const QString& phoneNumber )
{
    QString result;
    QString remaining( phoneNumber );

    // for remaining number length 1 (also 0) to 3 return number unchanged
    if ( remaining.length() < 4 )
    {
      result.append( '(' );
      result.append( remaining );
      result.append( ')' );
      return result;
    }
    else if ( remaining.length() < 7 )
    {
      result.append( '(' );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( ") " );
      result.append( remaining );
      return result;
    }
    else if ( remaining.length() < 11 )
    {
      result.append( '(' );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( ") " );
      result.append( remaining.left( 3 ) );
      remaining.remove( 0, 3 );
      result.append( '-' );
      result.append( remaining );
      return result;
    }
    else
    {
      result.append( remaining );
      return result;
    }
}

QString MLocalePrivate::formatPhoneNumber( const QString& phoneNumber,
    MLocale::PhoneNumberGrouping grouping ) const
{
  // first do sanity check of the input string
  QRegExp rx( "\\+?\\d*" );
  if ( ! rx.exactMatch( phoneNumber ) )
  {
    qWarning( "MLocale::formatPhoneNumber: cannot understand number: %s",
	      qPrintable( phoneNumber ) );
    return phoneNumber;
  }

  // 00 is not a valid country calling code in north america
  // -> do not do grouping in this case at all
  if ( ( grouping == MLocale::NorthAmericanPhoneNumberGrouping )
       && phoneNumber.startsWith( QLatin1String( "00" ) ) )
  {
    return phoneNumber;
  }

  QString remaining( phoneNumber );
  QString result;

  // if we find any error, we will return the string unchanged
  //  bool error = false;

  // first extract the country code

  bool foundCountryCodeIndicator = false;

  // valid beginnings for a country code are "+", "00" or "011"
  if ( remaining.startsWith( '+' ) )
  {
    foundCountryCodeIndicator = true;
    result.append( '+' );
    remaining.remove( 0, 1 );
  }
  else if ( remaining.startsWith( QLatin1String( "00" ) ) )
  {
    foundCountryCodeIndicator = true;
    result.append( "00 " );
    remaining.remove( 0, 2 );
  }
  else if ( remaining.startsWith( QLatin1String( "011" ) ) )
  {
    foundCountryCodeIndicator = true;
    result.append( "011 " );
    remaining.remove( 0, 3 );
  }


  // now check for valid country code
  if ( foundCountryCodeIndicator )
  {
    int length = 1;
    QString code;

    code = remaining.left( length );

    if ( isValidCountryCode( code ) )
    {
      result.append( code );
      result.append( ' ' );
      remaining.remove( 0, code.length() );
    }
    else
    {
      length = 2;
      code = remaining.left( length );
      if ( isValidCountryCode( code ) )
      {
	result.append( code );
	result.append( ' ' );
	remaining.remove( 0, code.length() );
      }
      else
      {
	length = 3;
	code = remaining.left( length );
	if ( isValidCountryCode( code ) )
	{
	  result.append( code );
	  result.append( ' ' );
	  remaining.remove( 0, code.length() );
	}
	else
	{
	  // no valid country code -> error -> return string
	  return phoneNumber;
	}
      }
    }
  } // found country code indicator

  // if it exists, the country code is split off now
  if ( grouping != MLocale::NorthAmericanPhoneNumberGrouping )
  {
    result.append( remaining );
    return result;
  }
  else
  {
    // has country code -> do not handle one special.
    if ( foundCountryCodeIndicator )
    {
      result.append( groupedInternationalString( remaining ) );
      return result;
    }
    // 11 is an invalid code, so disable grouping for this case
    else if ( remaining.startsWith( QLatin1String( "11" ) ) )
    {
      result.append( remaining );
      return result;
    }
    else if ( remaining.startsWith( '1' ) )
    {
      result.append( groupedOneString( remaining ) );
      return result;
    }
    else
    {
      result.append( groupedNormalString( remaining ) );
      return result;
    }
  }
}
