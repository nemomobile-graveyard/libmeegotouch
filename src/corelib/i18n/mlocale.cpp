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
#include <MApplication>

#ifdef HAVE_ICU
#include "mcollator.h"
#include "mcalendar.h"
#include "mcalendar_p.h"
#include "micuconversions.h"
#endif

namespace
{
    const char *const BackupNameFormatString = "%d%t%g%t%m%t%f";
    const char *const ExtraDataBundleName = "extradata";
    const QString RtlLanguages("am:ar:fa:he:ps:ur:");
    const char *const Languages = "Languages";
    const char *const Countries = "Countries";

    const QString ScriptLatin("Latn");

    const QString SettingsLanguage("/meegotouch/i18n/language");
    const QString SettingsLcTime("/meegotouch/i18n/lc_time");
    const QString SettingsLcCollate("/meegotouch/i18n/lc_collate");
    const QString SettingsLcNumeric("/meegotouch/i18n/lc_numeric");
    const QString SettingsLcMonetary("/meegotouch/i18n/lc_monetary");
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
//! \endinternal

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
            QString delims("_.");
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
    // For example, assume that we an Arabic translation "foo_ar.qm" has been loaded
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

QString MLocalePrivate::createLocaleString(const QString &language,
        const QString &country,
        const QString &script,
        const QString &variant)
{
    if (language.isEmpty())
        return "";

    // construct the locale string from the parameters
    QString localeString(language);

    if (!script.isEmpty())
        localeString += '_' + script;

    if (!country.isEmpty())
        localeString += '_' + country;

    if (!variant.isEmpty())
        localeString += '_' + variant;

    return localeString;
}

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
icu::DateFormat *MLocalePrivate::createDateFormat(MLocale::DateType dateType,
                                                    MLocale::TimeType timeType,
                                                    const MCalendar& mcalendar) const
{
    // Create calLocale which has the time pattern we want to use
    icu::Locale calLocale = MIcuConversions::createLocale(
        categoryName(MLocale::MLcTime),
        mcalendar.type());

    icu::DateFormat::EStyle dateStyle = MIcuConversions::toEStyle(dateType);
    icu::DateFormat::EStyle timeStyle = MIcuConversions::toEStyle(timeType);
    icu::DateFormat *df
    = icu::DateFormat::createDateTimeInstance(dateStyle, timeStyle, calLocale);

    // Symbols come from the messages locale
    icu::Locale symbolLocale
    = MIcuConversions::createLocale(categoryName(MLocale::MLcMessages),
                                      mcalendar.type());

    DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    // This is not nice but seems to be the only way to set the
    // symbols with the public API
    static_cast<SimpleDateFormat *>(df)->adoptDateFormatSymbols(dfs);

    return df;
}
#endif

// Constructors

MLocalePrivate::MLocalePrivate()
    : _valid(true),
      _calendarType(MLocale::DefaultCalendar),
      _collation(MLocale::DefaultCollation)
#ifdef HAVE_ICU
      , _numberFormat(0)
#endif
#ifdef HAVE_GCONF
      , currentLanguageItem(SettingsLanguage),
      currentLcTimeItem(SettingsLcTime),
      currentLcCollateItem(SettingsLcCollate),
      currentLcNumericItem(SettingsLcNumeric),
      currentLcMonetaryItem(SettingsLcMonetary)
#endif
      , q_ptr(0)
{
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
      _calendarType(other._calendarType),
      _collation(other._collation),
#ifdef HAVE_ICU
      _numberFormat(0),
#endif
      _messageTranslations(other._messageTranslations),
      _timeTranslations(other._timeTranslations),
      _trTranslations(other._trTranslations)
#ifdef HAVE_GCONF
      , currentLanguageItem(SettingsLanguage),
      currentLcTimeItem(SettingsLcTime),
      currentLcCollateItem(SettingsLcCollate),
      currentLcNumericItem(SettingsLcNumeric),
      currentLcMonetaryItem(SettingsLcMonetary)
#endif
      , q_ptr(0)
{
#ifdef HAVE_ICU
    if (other._numberFormat != 0) {
        _numberFormat = static_cast<icu::NumberFormat *>((other._numberFormat)->clone());
    }
#endif
}

MLocalePrivate::~MLocalePrivate()
{
#ifdef HAVE_ICU
    delete _numberFormat;
#endif
    // note: if tr translations are inserted into QCoreApplication
    // deleting the QTranslator removes them from the QCoreApplication
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
    _calendarType = other._calendarType;
    _collation = other._collation;
    _messageTranslations = other._messageTranslations;
    _timeTranslations = other._timeTranslations;
    _trTranslations = other._trTranslations;

#ifdef HAVE_ICU
    delete _numberFormat;

    if (other._numberFormat) {
        _numberFormat = static_cast<icu::NumberFormat *>((other._numberFormat)->clone());

    } else {
        _numberFormat = 0;
    }
#endif

    return *this;
}

#ifdef HAVE_ICU
// creates icu::Locale presenting a category
Locale MLocalePrivate::getCategoryLocale(MLocale::Category category) const
{
    QString baseString = categoryName(category);
    return MIcuConversions::createLocale(baseString, _calendarType, _collation);
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
    }

    return _defaultLocale;
}

void MLocalePrivate::loadTrCatalogs()
{
    Q_Q(MLocale);
    foreach(const QExplicitlySharedDataPointer<MTranslationCatalog>& sharedCatalog, _trTranslations) {
        if(sharedCatalog->_translator.isEmpty()
           || !sharedCatalog->_name.endsWith(QLatin1String(".qm"))) {
            sharedCatalog->loadWith(q, MLocale::MLcMessages);
        }
    }
}

void MLocalePrivate::insertTrToQCoreApp()
{
    foreach(const QExplicitlySharedDataPointer<MTranslationCatalog>& sharedCatalog, _trTranslations) {
        QCoreApplication::installTranslator(&sharedCatalog->_translator);
    }
}

void MLocalePrivate::removeTrFromQCoreApp()
{
    foreach(const QExplicitlySharedDataPointer<MTranslationCatalog>& sharedCatalog, _trTranslations) {
        QCoreApplication::removeTranslator(&sharedCatalog->_translator);
    }
}

QLocale::QLocale MLocalePrivate::createQLocale(MLocale::Category category) const
{
    Q_Q(const MLocale);
    QLocale qlocale(q->categoryLanguage(category)
                    + '_' +
                    q->categoryCountry(category));
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
    } else if (category == MLocale::MLcNumeric) {
        _numericLocale = localeName;
#ifdef HAVE_ICU
        // recreate the number formatter
        delete _numberFormat;

        UErrorCode status = U_ZERO_ERROR;
        icu::Locale numericLocale = getCategoryLocale(MLocale::MLcNumeric);
        _numberFormat = icu::NumberFormat::createInstance(numericLocale, status);

        if (!U_SUCCESS(status)) {
            mDebug("MLocalePrivate") << "Unable to create number format";
            _valid = false;
        }
#endif
    } else if (category == MLocale::MLcCollate) {
        _collationLocale = localeName;
    } else if (category == MLocale::MLcMonetary) {
        _monetaryLocale = localeName;
    } else if (category == MLocale::MLcName) {
        _nameLocale = localeName;
    } else {
        //mDebug("MLocalePrivate") << "unimplemented category change"; // DEBUG
    }
}

QString MLocalePrivate::parseLanguage(const QString &localeString)
{
    int endOfLanguage = localeString.indexOf('_');

    if (endOfLanguage > 0) {
        return localeString.left(endOfLanguage);
    } else {
        return localeString;
    }
}

QString MLocalePrivate::parseCountry(const QString &localeString)
{
    int startOfCountry = localeString.indexOf('_') + 1;
    int endOfCountry = localeString.indexOf('_', startOfCountry);

    if (startOfCountry > 0) {
        QString candidate = localeString.mid(startOfCountry, endOfCountry - startOfCountry);
        if (candidate.size() == 4) {
            startOfCountry = endOfCountry + 1;
            endOfCountry = localeString.indexOf('_', startOfCountry);
            if (startOfCountry > 0) {
                return localeString.mid(startOfCountry, endOfCountry - startOfCountry);
            } else {
                return "";
            }
        } else {
            return candidate;
        }
    } else {
        return "";
    }
}

QString MLocalePrivate::parseScript(const QString &localeString)
{
    int startOfScript = localeString.indexOf('_') + 1;
    int endOfScript = localeString.indexOf('_', startOfScript);

    if (startOfScript > 0) {
        QString candidate = localeString.mid(startOfScript, endOfScript - startOfScript);
        if (candidate.size() == 4) {
            return candidate;
        } else {
            return ScriptLatin;
        }
    } else {
        return ScriptLatin;
    }
}

QString MLocalePrivate::parseVariant(const QString &localeString)
{
    int startOfCountry = localeString.indexOf('_') + 1;
    int startOfVariant = localeString.indexOf('_', startOfCountry) + 1;

    if (startOfVariant > 0) {
        QString candidate = localeString.mid(startOfVariant);
        int startOfVariant = candidate.indexOf('_') + 1;

        if (startOfVariant > 0) {
            return candidate.mid(startOfVariant);
        } else {
            return candidate;
        }
    } else {
        return "";
    }
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
    // parentheses for the encoding part here:
    QRegExp regexp("([a-z]{2,3})(_([A-Z]{2,2}))?(?:.(?:[a-zA-Z0-9-]+))?(@([A-Z][a-z]+))?");

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
    MGConfItem lcCollateItem(SettingsLcCollate);
    MGConfItem lcNumericItem(SettingsLcNumeric);
    MGConfItem lcMonetaryItem(SettingsLcMonetary);

    QString language = languageItem.value().toString();
    QString lcTime = lcTimeItem.value().toString();
    QString lcCollate = lcCollateItem.value().toString();
    QString lcNumeric = lcNumericItem.value().toString();
    QString lcMonetary = lcMonetaryItem.value().toString();

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
    if (!lcCollate.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcCollate, lcCollate);
    if (!lcNumeric.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    if (!lcMonetary.isEmpty())
        systemLocale->setCategoryLocale(MLocale::MLcMonetary, lcMonetary);

    return systemLocale;
#else
    QString language = qgetenv("LANG");
    QString locale = cleanLanguageCountryPosix(language);
    if (language.isEmpty())
        language = PosixStr;

    return new MLocale(locale);
#endif
}

//! creates a "C" locale
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
    QObject::connect(&d->currentLcCollateItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcNumericItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
    QObject::connect(&d->currentLcMonetaryItem, SIGNAL(valueChanged()),
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
    QObject::disconnect(&d->currentLcCollateItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcNumericItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
    QObject::disconnect(&d->currentLcMonetaryItem, SIGNAL(valueChanged()),
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
    UErrorCode status = U_ZERO_ERROR;
    d->_numberFormat = icu::NumberFormat::createInstance(d->getCategoryLocale(MLcNumeric),
                       status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed:" << u_errorName(status);
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

//#ifdef HAVE_GCONF
//    QString oldLanguage = (locale.d_ptr)->currentLanguageItem.value().toString();
//    QString newLanguage = (locale.d_ptr)->_defaultLocale;
//    if (oldLanguage != newLanguage)
//        (locale.d_ptr)->currentLanguageItem.set(newLanguage);
//#endif

    // Setting the default QLocale is needed to get localized number
    // support in translations via %Ln, %L1, %L2, ...:
    QLocale::setDefault((s_systemDefault->d_ptr)->createQLocale(MLcNumeric));
    // sends QEvent::LanguageChange to qApp:
    (s_systemDefault->d_ptr)->insertTrToQCoreApp();
    // sends QEvent::ApplicationLayoutDirectionChange to qApp:
    qApp->setLayoutDirection(s_systemDefault->textDirection());

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
}

void MLocale::setCollation(Collation collation)
{
    Q_D(MLocale);
    d->_collation = collation;
}

MLocale::Collation MLocale::collation() const
{
    Q_D(const MLocale);
    return d->_collation;
}

void MLocale::setCalendarType(CalendarType calendarType)
{
    Q_D(MLocale);
    d->_calendarType = calendarType;
}


MLocale::CalendarType MLocale::calendarType() const
{
    Q_D(const MLocale);
    return d->_calendarType;
}

#ifdef HAVE_ICU
MCollator MLocale::collator() const
{
    return MCollator(*this);
}
#endif

QString MLocale::language() const
{
    return MLocalePrivate::parseLanguage(name());
}

QString MLocale::country() const
{
    return MLocalePrivate::parseCountry(name());
}

//TODO This shall return an enum
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

//////////////////////////
//// Formatting stuff ////

QString MLocale::formatNumber(qlonglong i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UnicodeString str;
    // This might generate a warning by the Krazy code analyzer,
    // but it allows the code to compile with ICU 4.0
    d->_numberFormat->format(static_cast<int64_t>(i), str); //krazy:exclude=typedefs
    return MIcuConversions::unicodeStringToQString(str);
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i);
#endif
}


QString MLocale::formatNumber(short i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UnicodeString str;
    d->_numberFormat->format(i, str);
    return MIcuConversions::unicodeStringToQString(str);
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i);
#endif
}


QString MLocale::formatNumber(int i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UnicodeString str;
    d->_numberFormat->format(i, str);
    return MIcuConversions::unicodeStringToQString(str);
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i);
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
        UErrorCode status = U_ZERO_ERROR;
        icu::NumberFormat *nf;

        nf = icu::NumberFormat::createInstance(d->getCategoryLocale(MLcNumeric),
                                               status);

        if (!U_SUCCESS(status)) {
            qWarning() << "NumberFormat creating failed";
            return QString(); // "null" string
        }

        nf->setMaximumFractionDigits(prec);
        nf->format(i, str);
        delete nf;
    }

    return MIcuConversions::unicodeStringToQString(str);
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i, 'g', prec);
#endif
}

QString MLocale::formatNumber(float i) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    icu::UnicodeString str;
    icu::FieldPosition pos;
    d->_numberFormat->format(i, str, pos);
    return MIcuConversions::unicodeStringToQString(str);
#else
    Q_D(const MLocale);
    return d->createQLocale(MLcNumeric).toString(i, 'g');
#endif
}

#ifdef HAVE_ICU
QString MLocale::formatPercent(double i, int decimals) const
{
    Q_D(const MLocale);
    icu::UnicodeString str;
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale numericLocale = d->getCategoryLocale(MLcNumeric);
    icu::NumberFormat *nf = NumberFormat::createPercentInstance(numericLocale, status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed";
        return QString();
    }

    nf->setMinimumFractionDigits(decimals);
    nf->format(i, str);
    delete nf;

    return MIcuConversions::unicodeStringToQString(str);
}
#endif

QString MLocale::formatCurrency(double amount, const QString &currency) const
{
#ifdef HAVE_ICU
    Q_D(const MLocale);
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale monetaryLocale = d->getCategoryLocale(MLcMonetary);
    icu::NumberFormat *nf = icu::NumberFormat::createCurrencyInstance(monetaryLocale, status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed";
        return QString();
    }

    icu::UnicodeString currencyString = MIcuConversions::qStringToUnicodeString(currency);
    nf->setCurrency(currencyString.getTerminatedBuffer(), status);

    if (!U_SUCCESS(status)) {
        qWarning() << "currency setting failed";
        delete nf;
        return QString();
    }

    icu::UnicodeString str;
    nf->format(amount, str);
    delete nf;

    return MIcuConversions::unicodeStringToQString(str);
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

    icu::DateFormat *df = d->createDateFormat(datetype, timetype, mcalendar);
    df->format(*cal, resString, pos);
    delete df;

    return MIcuConversions::unicodeStringToQString(resString);
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
    // convert QDateTime to MCalendar and format
    MCalendar calendar(*this);
    calendar.setDateTime(dateTime);
    return formatDateTime(calendar, formatString);
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
    // convert QDateTime to MCalendar and format
    MCalendar calendar(*this);
    calendar.setDateTime(dateTime);
    return formatDateTimeICU(calendar, formatString);
}
#endif

#ifdef HAVE_ICU
//! Formats the date time with ICU pattern
//! \note THIS MAY BE REMOVED FROM PUBLIC API
QString MLocale::formatDateTimeICU(const MCalendar &mCalendar,
                                     const QString &formatString) const
{
    Q_D(const MLocale);
    // Note: using MESSAGES locale here to get symbols in right language
    UErrorCode status = U_ZERO_ERROR;
    icu::SimpleDateFormat formatter(MIcuConversions::qStringToUnicodeString(formatString),
                                    d->getCategoryLocale(MLcMessages), status);

    if (U_FAILURE(status)) {
        return QString();
    }

    icu::FieldPosition pos;
    icu::UnicodeString resString;

    formatter.format(*mCalendar.d_ptr->_calendar, resString, pos);

    return MIcuConversions::unicodeStringToQString(resString);
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
                icuFormat.append("EEE");
                break;

            case 'A':
                // full weekday name
                icuFormat.append("EEEE");
                break;

            case 'b':
            case 'h':
                // abbreviated month name
                icuFormat.append("MMM");
                break;

            case 'B':
                // full month name
                icuFormat.append("MMMM");
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
                break;
            }

            case 'C': {
                // century, no corresponding icu pattern
                int century = mCalendar.year() / 100;
                icuFormat.append(QString::number(century));
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
                    = icuFormatString(MLocale::DateNone, MLocale::TimeShort,
                                      MLocale::GregorianCalendar);
                timeShortFormat.replace(QChar('k'), QChar('K'), Qt::CaseSensitive);
                timeShortFormat.replace(QChar('H'), QChar('h'), Qt::CaseSensitive);
                if (!timeShortFormat.contains('a', Qt::CaseSensitive))
                    timeShortFormat.append(QLatin1String(" a"));
                icuFormat.append(timeShortFormat);
            }
                break;

            case 'R': {
                // 24-hour clock time, in the format "%H:%M"
                QString timeShortFormat
                    = icuFormatString(MLocale::DateNone, MLocale::TimeShort,
                                      MLocale::GregorianCalendar);
                timeShortFormat.replace(QRegExp(" *a"), QLatin1String(""));
                timeShortFormat.replace(QChar('K'), QChar('k'), Qt::CaseSensitive);
                timeShortFormat.replace(QChar('h'), QChar('H'), Qt::CaseSensitive);
                icuFormat.append(timeShortFormat);
            }
                break;

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

            case 'u':
                // Weekday, as a decimal number (1(Monday)-7)
                // no corresponding icu pattern for monday based weekday
                icuFormat.append(QString::number(mCalendar.dayOfWeek()));
                break;

            case 'U': {
                // Week number of the year (Sunday as the first day of the week) as a
                // decimal number (00-53). first week starts from first sunday.
                int weeknumber = weekNumberStartingFromDay(mCalendar, MLocale::Sunday);
                icuFormat.append(QString::number(weeknumber));
                break;
            }

            case 'v':
                // week number of the year in two digits
                icuFormat.append("ww");
                break;

            case 'V': {
                // Week of the year (Monday as the first day of the week), as a decimal
                // number (01-53). according to ISO-8601

                MCalendar calendarCopy = mCalendar;
                calendarCopy.setFirstDayOfWeek(MLocale::Monday);
                calendarCopy.setMinimalDaysInFirstWeek(4);
                int weeknumber = calendarCopy.weekNumber();
                icuFormat.append(QString::number(weeknumber));
                break;
            }

            case 'w': {
                // Weekday, as a decimal number (0(Sunday)-6)
                int weekday = mCalendar.dayOfWeek();

                if (weekday == Sunday) {
                    weekday = 0;
                }

                icuFormat.append(QString::number(weekday));
                break;
            }

            case 'W': {
                // Week number of the year (Monday as the first day of the week), as a
                // decimal number (00-53). Week starts from the first monday
                int weeknumber = weekNumberStartingFromDay(mCalendar, MLocale::Monday);
                icuFormat.append(QString::number(weeknumber));
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
    }

    return formatDateTimeICU(mCalendar, icuFormat);
}
#endif

#ifdef HAVE_ICU
QString MLocale::icuFormatString( DateType dateType,
                          TimeType timeType,
                          CalendarType calendarType) const
{

    // Create calLocale which has the time pattern we want to use
    icu::Locale calLocale = MIcuConversions::createLocale(
        categoryName(MLocale::MLcTime),
        calendarType);

    icu::DateFormat::EStyle dateStyle = MIcuConversions::toEStyle(dateType);
    icu::DateFormat::EStyle timeStyle = MIcuConversions::toEStyle(timeType);
    icu::DateFormat *df
    = icu::DateFormat::createDateTimeInstance(dateStyle, timeStyle, calLocale);

    // Both the locale and the calendarType seem to
    // be ignored here anyway. I.e. the following two statements can be replaced with
    //  DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(icu::Locale(""));
    // without a change in behaviour:
    icu::Locale symbolLocale
        = MIcuConversions::createLocale(categoryName(MLocale::MLcMessages),
                                      calendarType);
    DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    QString icuFormatQString;

    if (df)
    {
        icu::UnicodeString icuFormatString;
        // This is not nice but seems to be the only way to set the
        // symbols with the public API
        static_cast<SimpleDateFormat *>(df)->adoptDateFormatSymbols(dfs);
        static_cast<SimpleDateFormat *>(df)->toPattern(icuFormatString);
        icuFormatQString = MIcuConversions::unicodeStringToQString(icuFormatString);
        delete df;
    }
    return icuFormatQString;
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
    icu::DateFormat *df = d->createDateFormat(dateType, timeType, mcalendar);
    icu::ParsePosition pos(0);
    UDate parsedDate = df->parse(text, pos);
    delete df;

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
    Q_D(const MLocale);
    monthNumber--; // months in array starting from index zero

    // use message locale as the output language
    icu::Locale symbolLocale
    = MIcuConversions::createLocale(d->categoryName(MLcMessages),
                                      mCalendar.type());

    icu::DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    int len;
    const UnicodeString *months = dfs->getMonths(len);

    QString result;

    if (monthNumber < len && monthNumber >= 0) {
        result = MIcuConversions::unicodeStringToQString(months[monthNumber]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber,
                             DateSymbolContext context,
                             DateSymbolLength symbolLength) const
{
    Q_D(const MLocale);

    monthNumber--; // months in array starting from index zero

    icu::Locale symbolLocale
    = MIcuConversions::createLocale(d->categoryName(MLcMessages),
                                      mCalendar.type());

    icu::DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    // map context type
    icu::DateFormatSymbols::DtContextType icuContext =
        MIcuConversions::mDateContextToIcu(context);

    // map length type
    icu::DateFormatSymbols::DtWidthType icuWidth =
        MIcuConversions::mDateWidthToIcu(symbolLength);

    int len;
    const UnicodeString *months = dfs->getMonths(len, icuContext, icuWidth);

    QString result;

    if (monthNumber < len && monthNumber >= 0) {
        result = MIcuConversions::unicodeStringToQString(months[monthNumber]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday) const
{
    Q_D(const MLocale);
    // use message locale as the output language
    icu::Locale symbolLocale
    = MIcuConversions::createLocale(d->categoryName(MLcMessages),
                                      mCalendar.type());

    icu::DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    int len;
    const UnicodeString *weekdayNames = dfs->getWeekdays(len);
    int weekdayNum = MIcuConversions::icuWeekday(weekday);

    QString result;

    if (weekdayNum < len && weekdayNum > 0) {
        result = MIcuConversions::unicodeStringToQString(weekdayNames[weekdayNum]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday,
                               DateSymbolContext context,
                               DateSymbolLength symbolLength) const
{
    Q_D(const MLocale);
    icu::Locale symbolLocale
    = MIcuConversions::createLocale(d->categoryName(MLcMessages),
                                      mCalendar.type());

    icu::DateFormatSymbols *dfs = MLocalePrivate::createDateFormatSymbols(symbolLocale);

    icu::DateFormatSymbols::DtContextType icuContext
    = MIcuConversions::mDateContextToIcu(context);

    icu::DateFormatSymbols::DtWidthType icuWidth
    = MIcuConversions::mDateWidthToIcu(symbolLength);

    int len;
    const UnicodeString *weekdayNames = dfs->getWeekdays(len, icuContext, icuWidth);
    int weekdayNum = MIcuConversions::icuWeekday(weekday);

    QString result;

    if (weekdayNum < len && weekdayNum > 0) {
        result = MIcuConversions::unicodeStringToQString(weekdayNames[weekdayNum]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
namespace
{
// fetches a resource bundle for name formats
    UResourceBundle *nameFmtsResourceBundle()
    {
        UErrorCode status = U_ZERO_ERROR;

        UResourceBundle *extraData = ures_openDirect(ExtraDataBundleName, "", &status);

        if (U_FAILURE(status)) {
            mDebug("mlocale.cpp") << "Error ures_open" << u_errorName(status);
        }

        UResourceBundle *posixData = ures_getByKey(extraData, "posixdata", NULL, &status);

        if (U_FAILURE(status)) {
            mDebug("mlocale.cpp") << "Error ures_open posixData" << u_errorName(status);
        }

        UResourceBundle *nameFmts = ures_getByKey(posixData, "nameFmts", NULL, &status);
        if (U_FAILURE(status)) {
            mDebug("mlocale.cpp") << "Error ures_open nameFmts" << u_errorName(status);
        }

        ures_close(extraData);
        ures_close(posixData);

        return nameFmts;
    }
}
#endif

#ifdef HAVE_ICU
QString MLocale::languageEndonym() const
{
    Q_D(const MLocale);
    UErrorCode status = U_ZERO_ERROR;

    // TODO: implement a workaround for
    // http://site.icu-project.org/design/resbund/issues
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    UResourceBundle *res = ures_open("ICUDATA-lang", qPrintable(d->_defaultLocale), &status);
#else
    UResourceBundle *res = ures_open(NULL, qPrintable(d->_defaultLocale), &status);
#endif
    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_open" << u_errorName(status);
    }

    res = ures_getByKey(res, Languages, res, &status);
    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_getByKey" << u_errorName(status);
    }

    QString lang = language();

    int len;
    const UChar *val = ures_getStringByKey(res, qPrintable(lang), &len, &status);

    QString convertedValue = lang;

    if (U_SUCCESS(status)) {
        convertedValue = QString::fromUtf16(val, len);
    }

    ures_close(res);
    return convertedValue;
}
#endif

#ifdef HAVE_ICU
QString MLocale::countryEndonym() const
{
    Q_D(const MLocale);
    UErrorCode status = U_ZERO_ERROR;

    // TODO: implement a workaround for
    // http://site.icu-project.org/design/resbund/issues
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    UResourceBundle *res = ures_open("ICUDATA-region", qPrintable(d->_defaultLocale), &status);
#else
    UResourceBundle *res = ures_open(NULL, qPrintable(d->_defaultLocale), &status);
#endif
    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_open" << u_errorName(status);
    }

    res = ures_getByKey(res, Countries, res, &status);
    if (U_FAILURE(status)) {
        mDebug("MLocale") << "Error ures_getByKey" << u_errorName(status);
    }

    QString c = country();

    int len;
    const UChar *val = ures_getStringByKey(res, c.toStdString().c_str(), &len, &status);

    QString convertedValue = c;

    if (U_SUCCESS(status)) {
        convertedValue = QString::fromUtf16(val, len);
    }

    ures_close(res);
    return convertedValue;
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

    MTranslationCatalog *catalog
        = new MTranslationCatalog(name);
    d->_trTranslations.append(QExplicitlySharedDataPointer<MTranslationCatalog>(catalog));
    if (!name.endsWith(QLatin1String(".qm"))) {
        MTranslationCatalog *engineeringEnglishCatalog
            = new MTranslationCatalog(name + ".qm");
        d->_trTranslations.prepend(QExplicitlySharedDataPointer<MTranslationCatalog>(engineeringEnglishCatalog));
    }
    d->loadTrCatalogs();
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
#ifdef HAVE_ICU
    QString pathString;

    foreach(QString string, dataPaths) {
        string.replace('/', U_FILE_SEP_CHAR);
        pathString.append(string);
        // separator gets appended to the end of the list. I hope Icu doesn't mind
        pathString.append(U_PATH_SEP_CHAR);
    }

    u_setDataDirectory(qPrintable(pathString));
#else
    Q_UNUSED(dataPaths);
#endif
}

// convenience method for just one path
void MLocale::setDataPath(const QString &dataPath)
{
    setDataPaths(QStringList(dataPath));
}

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

Qt::LayoutDirection MLocale::textDirection() const
{
    Qt::LayoutDirection dir = Qt::LeftToRight;

    // Checking for the script "arab" is needed for
    // locales where the language can be written in several scripts.
    // Eg the Uyghur language can be written in Chinese, Cyrillic,
    // or Arabic script.
    if (script().contains("arab", Qt::CaseInsensitive))
        dir = Qt::RightToLeft;
    else if (!language().isEmpty()
             && RtlLanguages.contains(language() + ':'))
        dir = Qt::RightToLeft;

    return dir;
}

void MLocale::refreshSettings()
{
#ifdef HAVE_GCONF
    Q_D(MLocale);
    bool settingsHaveReallyChanged = false;
    QString localeName = d->currentLanguageItem.value().toString();
    QString lcTime = d->currentLcTimeItem.value().toString();
    QString lcCollate = d->currentLcCollateItem.value().toString();
    QString lcNumeric = d->currentLcNumericItem.value().toString();
    QString lcMonetary = d->currentLcMonetaryItem.value().toString();

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

    if (settingsHaveReallyChanged) {
        if (this == s_systemDefault) {
            // Setting the default QLocale is needed to get localized number
            // support in translations via %Ln, %L1, %L2, ...:
            QLocale::setDefault(d->createQLocale(MLcNumeric));
            d->removeTrFromQCoreApp();
            d->loadTrCatalogs();
            // sends QEvent::LanguageChange to qApp:
            d->insertTrToQCoreApp();
            // sends QEvent::ApplicationLayoutDirectionChange to qApp:
            qApp->setLayoutDirection(this->textDirection());
        }
        else {
            d->loadTrCatalogs();
        }
        emit settingsChanged();
    }
#endif
}

