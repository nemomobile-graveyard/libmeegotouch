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

#include "duilocale.h"
#include "duilocale_p.h"

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

#include <DuiDebug>
#include <QTranslator>
#include <QDir>
#include <QApplication>
#include <QMutex>
#include <QDateTime>

#ifdef HAVE_ICU
#include "duicollator.h"
#include "duicalendar.h"
#include "duicalendar_p.h"
#include "duiicuconversions.h"
#endif

namespace
{
    const char *const BackupNameFormatString = "%d%t%g%t%m%t%f";
    const char *const ExtraDataBundleName = "extradata";
    const QString RtlLanguages("am:ar:fa:he:ps:ur:");
    const char *const Languages = "Languages";
    const char *const Countries = "Countries";

    const QString ScriptLatin("Latn");

    const QString SettingsLanguage("/Dui/i18n/Language");
    const QString SettingsCountry("/Dui/i18n/Country");
    const QString SettingsScript("/Dui/i18n/Script");
    const QString SettingsVariant("/Dui/i18n/Variant");
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
//// The private Duitranslationcatalog class ////

//! \internal
class DuiTranslationCatalog: public QSharedData
{
public:

    DuiTranslationCatalog(const QString &name);
    virtual ~DuiTranslationCatalog();

    // called by detach
    DuiTranslationCatalog(const DuiTranslationCatalog &other);

    /*!
    * \brief Load the actual translation file using locale and category info
    *
    * As an example lets assume that
    *
    * - DuiLocale::translationPaths()
    *   is the list ("/usr/share/l10n/dui", "/usr/share/l10n")
    * - the category is  DuiLocale::DuiLcMessages
    * - the name of the locale (returned by duilocale->categoryName(category))
    *   is "en_US"
    * - the base name of the translation file is "foo"
    *
    * then the function will try to load translation catalogs in the following order:
    *
    *   /usr/share/l10n/dui/LC_MESSAGES/foo_en_US.qm
    *   /usr/share/l10n/dui/LC_MESSAGES/foo_en_US
    *   /usr/share/l10n/dui/LC_MESSAGES/foo_en.qm
    *   /usr/share/l10n/dui/LC_MESSAGES/foo_en
    *   /usr/share/l10n/dui/LC_MESSAGES/foo.qm
    *   /usr/share/l10n/dui/LC_MESSAGES/foo
    *   /usr/share/l10n/dui/foo_en_US.qm
    *   /usr/share/l10n/dui/foo_en_US
    *   /usr/share/l10n/dui/foo_en.qm
    *   /usr/share/l10n/dui/foo_en
    *   /usr/share/l10n/dui/foo.qm
    *   /usr/share/l10n/dui/foo
    *   /usr/share/l10n/LC_MESSAGES/foo_en_US.qm
    *   /usr/share/l10n/LC_MESSAGES/foo_en_US
    *   /usr/share/l10n/LC_MESSAGES/foo_en.qm
    *   /usr/share/l10n/LC_MESSAGES/foo_en
    *   /usr/share/l10n/LC_MESSAGES/foo.qm
    *   /usr/share/l10n/LC_MESSAGES/foo
    *   /usr/share/l10n/foo_en_US.qm
    *   /usr/share/l10n/foo_en_US
    *   /usr/share/l10n/foo_en.qm
    *   /usr/share/l10n/foo_en
    *   /usr/share/l10n/foo.qm
    *   /usr/share/l10n/foo
    *
    * and return when the first translation catalog was found.
    * If no translation can be found this function returns false.
    *
    * The way locale specific parts are successively cut away from the
    * translation file name is inherited from
    * <a href="http://qt.nokia.com/doc/4.6/qtranslator.html#load">QTranslator::load()</a>
    * because this is used to load the translation files.
    *
    * @param duilocale the locale for which the translations are loaded
    * @param category  this is usually DuiLocale::DuiLcMessages but it could
    *                  also be DuiLocale::DuiLcTime, DuiLocale::DuiLcNumeric,
    *                  etc...
    *
    * @return true if translations could be found, false if not.
    */
    bool loadWith(DuiLocale *duilocale, DuiLocale::Category category);

    // the abstract name for a translation. together with locale info and
    // category a concrete path is created when the file is loaded
    QString _name;

    // the actual translator
    QTranslator _translator;

private:
    DuiTranslationCatalog &operator=(const DuiTranslationCatalog &other);
};
//! \endinternal

// //////
// DuiTranslationCatalog implementation

DuiTranslationCatalog::DuiTranslationCatalog(const QString &name)
    : _name(name), _translator()
{
    // nothing
}

DuiTranslationCatalog::~DuiTranslationCatalog()
{
    // nothing
}

DuiTranslationCatalog::DuiTranslationCatalog(const DuiTranslationCatalog &other)
    : QSharedData(), _name(other._name)
{
    // nothing
}

bool DuiTranslationCatalog::loadWith(DuiLocale *duilocale, DuiLocale::Category category)
{
    QStringList localeDirs;
    QString fname;
    if (QFileInfo(_name).isRelative()) {
        localeDirs = DuiLocale::translationPaths();
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
            fname += '_' + duilocale->categoryName(category);
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

    return false;
}

////////////////////////////////
//// Private stuff for DuiLocale

QStringList DuiLocalePrivate::translationPaths;

// categories have their translations in their own subdirectories.
QString DuiLocalePrivate::categoryToDirectoryName(DuiLocale::Category category)
{
    switch (category) {
    case DuiLocale::DuiLcMessages:
        return "LC_MESSAGES";

    case DuiLocale::DuiLcTime:
        return "LC_TIME";

    default:
        return QString();
    }
}


QString DuiLocalePrivate::createLocaleString(const QString &language,
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
icu::DateFormatSymbols *DuiLocalePrivate::createDateFormatSymbols(icu::Locale locale)
{
    // This is a bit dirty but the only way to currently get the symbols
    // is like this. only internal API supports directly creating DateFormatSymbols
    // with arbitrary calendar
    UErrorCode status = U_ZERO_ERROR;
    SimpleDateFormat dummyFormatter("", locale, status);

    if (U_FAILURE(status)) {
        return 0;
    }

    const DateFormatSymbols *dfs = dummyFormatter.getDateFormatSymbols();
    return new DateFormatSymbols(*dfs);
}
#endif

// Constructors

DuiLocalePrivate::DuiLocalePrivate()
    : _valid(true),
      _calendar(DuiLocale::DefaultCalendar),
      _collation(DuiLocale::DefaultCollation)
#ifdef HAVE_ICU
      , _numberFormat(0)
#endif
#ifdef HAVE_GCONF
      , currentLanguageItem(SettingsLanguage),
      currentCountryItem(SettingsCountry),
      currentScriptItem(SettingsScript),
      currentVariantItem(SettingsVariant)
#endif
{
}

// copy constructor
DuiLocalePrivate::DuiLocalePrivate(const DuiLocalePrivate &other)
    : _valid(other._valid),
      _defaultLocale(other._defaultLocale),
      _messageLocale(other._messageLocale),
      _numericLocale(other._numericLocale),
      _collationLocale(other._collationLocale),
      _calendarLocale(other._calendarLocale),
      _monetaryLocale(other._monetaryLocale),
      _nameLocale(other._nameLocale),
      _calendar(other._calendar),
      _collation(other._collation),
#ifdef HAVE_ICU
      _numberFormat(0),
#endif
      _messageTranslations(other._messageTranslations),
      _timeTranslations(other._timeTranslations),
      _trTranslations(other._trTranslations)
#ifdef HAVE_GCONF
      , currentLanguageItem(SettingsLanguage),
      currentCountryItem(SettingsCountry),
      currentScriptItem(SettingsScript),
      currentVariantItem(SettingsVariant)
#endif
{
#ifdef HAVE_ICU
    if (other._numberFormat != 0) {
        _numberFormat = static_cast<icu::NumberFormat *>((other._numberFormat)->clone());
    }
#endif
}

DuiLocalePrivate::~DuiLocalePrivate()
{
#ifdef HAVE_ICU
    delete _numberFormat;
#endif
    // note: if tr translations are inserted into QCoreApplication
    // deleting the QTranslator removes them from the QCoreApplication
}

DuiLocalePrivate &DuiLocalePrivate::operator=(const DuiLocalePrivate &other)
{
    _valid = other._valid;
    _defaultLocale = other._defaultLocale;
    _messageLocale = other._messageLocale;
    _numericLocale = other._numericLocale;
    _collationLocale = other._collationLocale;
    _calendarLocale = other._calendarLocale;
    _monetaryLocale = other._monetaryLocale;
    _nameLocale = other._nameLocale;
    _calendar = other._calendar;
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
Locale DuiLocalePrivate::getCategoryLocale(DuiLocale::Category category) const
{
    QString baseString = categoryName(category);
    return DuiIcuConversions::createLocale(baseString, _calendar, _collation);
}
#endif

// creates an QString for category or default as a fallback
QString DuiLocalePrivate::categoryName(DuiLocale::Category category) const
{
    switch (category) {
    case(DuiLocale::DuiLcMessages):
        if (!_messageLocale.isEmpty()) {
            return _messageLocale;
        }
        break;

    case(DuiLocale::DuiLcNumeric):
        if (!_numericLocale.isEmpty()) {
            return _numericLocale;
        }
        break;

    case(DuiLocale::DuiLcCollate):
        if (!_collationLocale.isEmpty()) {
            return _collationLocale;
        }
        break;

    case(DuiLocale::DuiLcMonetary):
        if (!_monetaryLocale.isEmpty()) {
            return _monetaryLocale;
        }
        break;

    case(DuiLocale::DuiLcTime):
        if (!_calendarLocale.isEmpty()) {
            return _calendarLocale;
        }
        break;

    case(DuiLocale::DuiLcName):
        if (!_nameLocale.isEmpty()) {
            return _nameLocale;
        }
        break;
    }

    return _defaultLocale;
}

void DuiLocalePrivate::loadTrCatalogs()
{
    Q_Q(DuiLocale);
    foreach(const QExplicitlySharedDataPointer<DuiTranslationCatalog>& sharedCatalog, _trTranslations) {
        if(sharedCatalog->_translator.isEmpty()
           || !sharedCatalog->_name.endsWith(QLatin1String(".qm"))) {
            sharedCatalog->loadWith(q, DuiLocale::DuiLcMessages);
        }
    }
}

void DuiLocalePrivate::reloadCatalogList(CatalogList &cataloglist,
        DuiLocale *duilocale,
        DuiLocale::Category category)
{
    DuiLocalePrivate::CatalogList::iterator end = cataloglist.end();
    for (DuiLocalePrivate::CatalogList::iterator it = cataloglist.begin();
            it != end; ++it) {
        // if the duitranslationcatalog is shared we need to make a new copy
        (*it).detach();
        (*it)->loadWith(duilocale, category);
    }
}

// sets category to specific locale
void DuiLocalePrivate::setCategoryLocale(DuiLocale *duilocale,
        DuiLocale::Category category,
        const QString &localeName)
{
    if (category == DuiLocale::DuiLcMessages) {
        _messageLocale = localeName;

        // detach message translations
        reloadCatalogList(_messageTranslations, duilocale, DuiLocale::DuiLcMessages);
        reloadCatalogList(_trTranslations, duilocale, DuiLocale::DuiLcMessages);


    } else if (category == DuiLocale::DuiLcTime) {
        _calendarLocale = localeName;
        reloadCatalogList(_timeTranslations, duilocale, DuiLocale::DuiLcTime);


    } else if (category == DuiLocale::DuiLcNumeric) {
        _numericLocale = localeName;

#ifdef HAVE_ICU
        // recreate the number formatter
        delete _numberFormat;

        UErrorCode status = U_ZERO_ERROR;
        icu::Locale numericLocale = getCategoryLocale(DuiLocale::DuiLcNumeric);
        _numberFormat = icu::NumberFormat::createInstance(numericLocale, status);

        if (!U_SUCCESS(status)) {
            duiDebug("DuiLocalePrivate") << "Unable to create number format";
            _valid = false;
        }
#endif

    } else if (category == DuiLocale::DuiLcCollate) {
        _collationLocale = localeName;

    } else if (category == DuiLocale::DuiLcMonetary) {
        _monetaryLocale = localeName;

    } else if (category == DuiLocale::DuiLcName) {
        _nameLocale = localeName;

    } else {
        //duiDebug("DuiLocalePrivate") << "unimplemented category change"; // DEBUG
    }
}


QString DuiLocalePrivate::parseLanguage(const QString &localeString)
{
    int endOfLanguage = localeString.indexOf('_');

    if (endOfLanguage > 0) {
        return localeString.left(endOfLanguage);
    } else {
        return localeString;
    }
}


QString DuiLocalePrivate::parseCountry(const QString &localeString)
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

QString DuiLocalePrivate::parseScript(const QString &localeString)
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

QString DuiLocalePrivate::parseVariant(const QString &localeString)
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
///// DuiLocale class implementation


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

DuiLocale *
DuiLocale::createSystemDuiLocale()
{
#ifdef HAVE_GCONF
    DuiGConfItem languageItem(SettingsLanguage);
    DuiGConfItem countryItem(SettingsCountry);
    DuiGConfItem scriptItem(SettingsScript);
    DuiGConfItem variantItem(SettingsVariant);

    QString language = languageItem.value().toString();
    QString country  = countryItem.value().toString();
    QString script   = scriptItem.value().toString();
    QString variant  = variantItem.value().toString();

    DuiLocale *systemLocale;

    if (language.isEmpty()) {
        language = qgetenv("LANG");
        QString locale = cleanLanguageCountryPosix(language);

        if (locale.isEmpty())
            locale = PosixStr;

        // No need to set the category according to env here
        systemLocale = new DuiLocale(locale);
    } else {
        systemLocale = new DuiLocale(
            DuiLocalePrivate::createLocaleString(QString(language),
                    QString(country),
                    QString(script),
                    QString(variant)));
    }

    return systemLocale;
#else
    QString language = qgetenv("LANG");
    QString locale = cleanLanguageCountryPosix(language);
    if (language.isEmpty())
        language = PosixStr;

    return new DuiLocale(locale);
#endif
}


//! creates a "C" locale
DuiLocale DuiLocale::createCLocale()
{
    return DuiLocale(PosixStr);
}

void
DuiLocale::connectSettings()
{
#ifdef HAVE_GCONF
    Q_D(DuiLocale);

    QObject::connect(&d->currentLanguageItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));

    QObject::connect(&d->currentCountryItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));

    QObject::connect(&d->currentScriptItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));

    QObject::connect(&d->currentVariantItem, SIGNAL(valueChanged()),
                     this, SLOT(refreshSettings()));
#endif
}

void
DuiLocale::disconnectSettings()
{
#ifdef HAVE_GCONF
    Q_D(DuiLocale);

    QObject::disconnect(&d->currentLanguageItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));

    QObject::disconnect(&d->currentCountryItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));

    QObject::disconnect(&d->currentScriptItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));

    QObject::disconnect(&d->currentVariantItem, SIGNAL(valueChanged()),
                        this, SLOT(refreshSettings()));
#endif
}

///// Constructors  /////

//! Constructs a DuiLocale with data copied from default Locale
DuiLocale::DuiLocale(QObject *parent)
    : QObject(parent),
      d_ptr(new DuiLocalePrivate)
{
    Q_D(DuiLocale);
    d->q_ptr = this;
    // copy the system default
    DuiLocale &defaultLocale = getDefault();
    *this = defaultLocale;
}

DuiLocale::DuiLocale(const QString &localeName, QObject *parent)
    : QObject(parent),
      d_ptr(new DuiLocalePrivate)
{
    Q_D(DuiLocale);
    d->q_ptr = this;
    d->_defaultLocale = qPrintable(localeName);

#ifdef HAVE_ICU
    // we cache the number formatter for better performance
    UErrorCode status = U_ZERO_ERROR;
    d->_numberFormat = icu::NumberFormat::createInstance(d->getCategoryLocale(DuiLcNumeric),
                       status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed:" << u_errorName(status);
        d->_valid = false;
    }
#endif
}


//! Copy constructor
DuiLocale::DuiLocale(const DuiLocale &other, QObject *parent)
    : QObject(parent),
      d_ptr(new DuiLocalePrivate(*other.d_ptr))
{
    Q_D(DuiLocale);
    d->q_ptr = this;
}


//! Destructor
DuiLocale::~DuiLocale()
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
DuiLocale &DuiLocale::operator=(const DuiLocale &other)
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
DuiLocale *DuiLocale::s_systemDefault = 0;

void DuiLocale::setDefault(const DuiLocale &locale)
{
    defaultLocaleMutex.lock();

    if (s_systemDefault == 0) {
        s_systemDefault = new DuiLocale(locale);
    } else if (&locale == s_systemDefault || locale.d_ptr == s_systemDefault->d_ptr) {
        defaultLocaleMutex.unlock();
        return;
    } else {
        s_systemDefault->disconnectSettings();
        QObject::disconnect(s_systemDefault, SIGNAL(settingsChanged()), qApp, SIGNAL(localeSettingsChanged()));
        // remove the previous tr translations
        s_systemDefault->removeTrFromQCoreApp();
        *s_systemDefault = locale;
    }
    defaultLocaleMutex.unlock();

#ifdef HAVE_GCONF
    QString oldLanguage = (locale.d_ptr)->currentLanguageItem.value().toString();
    QString newLanguage = (locale.d_ptr)->_defaultLocale;
    if (oldLanguage != newLanguage)
        (locale.d_ptr)->currentLanguageItem.set(newLanguage);
#endif

    QCoreApplication::processEvents();
    // Setting the default QLocale is needed to get localized number
    // support in translations via %Ln, %L1, %L2, ...:
    QLocale qlocale(locale.language() + '_' + locale.country());
    QLocale::setDefault(qlocale);
    // sends QEvent::LanguageChange to qApp:
    s_systemDefault->insertTrToQCoreApp();
    // sends QEvent::ApplicationLayoutDirectionChange to qApp:
    qApp->setLayoutDirection(s_systemDefault->textDirection());
    // the following 2 events have already been sent above:
    // QEvent ev(QEvent::LanguageChange);
    // qApp->sendEvent(qApp, &ev);
    // QEvent dirEv(QEvent::ApplicationLayoutDirectionChange);
    // qApp->sendEvent(qApp, &dirEv);
    QCoreApplication::processEvents();

    QObject::connect(s_systemDefault, SIGNAL(settingsChanged()), qApp, SIGNAL(localeSettingsChanged()));
    emit s_systemDefault->settingsChanged();
    s_systemDefault->connectSettings();
}

DuiLocale &DuiLocale::getDefault()
{
    if (s_systemDefault == 0) {
        // no default created, do it now

        // avoid race condition for multiple getDefaults()
        defaultLocaleMutex.lock();

        if (s_systemDefault == 0) {
            // we won the race
            s_systemDefault = createSystemDuiLocale();
        }

        defaultLocaleMutex.unlock();
    }

    return *s_systemDefault;
}

bool DuiLocale::isValid() const
{
    Q_D(const DuiLocale);
    return d->_valid;
}

void DuiLocale::setCategoryLocale(Category category, const QString &localeName)
{
    Q_D(DuiLocale);
    d->setCategoryLocale(this, category, localeName);
}

void DuiLocale::setCollation(Collation collation)
{
    Q_D(DuiLocale);
    d->_collation = collation;
}

DuiLocale::Collation DuiLocale::collation() const
{
    Q_D(const DuiLocale);
    return d->_collation;
}

void DuiLocale::setCalendar(Calendar calendar)
{
    Q_D(DuiLocale);
    d->_calendar = calendar;
}


DuiLocale::Calendar DuiLocale::calendar() const
{
    Q_D(const DuiLocale);
    return d->_calendar;
}

#ifdef HAVE_ICU
DuiCollator DuiLocale::collator() const
{
    return DuiCollator(*this);
}
#endif

QString DuiLocale::language() const
{
    return DuiLocalePrivate::parseLanguage(name());
}

QString DuiLocale::country() const
{
    return DuiLocalePrivate::parseCountry(name());
}

//TODO This shall return an enum
QString DuiLocale::script() const
{
    return DuiLocalePrivate::parseScript(name());
}

QString DuiLocale::variant() const
{
    return DuiLocalePrivate::parseVariant(name());
}

QString DuiLocale::name() const
{
    Q_D(const DuiLocale);
    return d->_defaultLocale;
}

QString DuiLocale::categoryLanguage(Category category) const
{
    QString wholeName = categoryName(category);
    return DuiLocalePrivate::parseLanguage(wholeName);
}

QString DuiLocale::categoryCountry(Category category) const
{
    QString wholeName = categoryName(category);
    return DuiLocalePrivate::parseCountry(wholeName);
}

QString DuiLocale::categoryVariant(Category category) const
{
    QString wholeName = categoryName(category);
    return DuiLocalePrivate::parseVariant(wholeName);
}

QString DuiLocale::categoryName(Category category) const
{
    Q_D(const DuiLocale);
    return d->categoryName(category);
}

//////////////////////////
//// Formatting stuff ////

QString DuiLocale::formatNumber(qlonglong i) const
{
#ifdef HAVE_ICU
    Q_D(const DuiLocale);
    UnicodeString str;
    // This might generate a warning by the Krazy code analyzer,
    // but it allows the code to compile with ICU 4.0
    d->_numberFormat->format(static_cast<int64_t>(i), str); //krazy:exclude=typedefs
    return DuiIcuConversions::unicodeStringToQString(str);
#else
    return QString::number(i);
#endif
}


QString DuiLocale::formatNumber(short i) const
{
#ifdef HAVE_ICU
    Q_D(const DuiLocale);
    UnicodeString str;
    d->_numberFormat->format(i, str);
    return DuiIcuConversions::unicodeStringToQString(str);
#else
    return QString::number(i);
#endif
}


QString DuiLocale::formatNumber(int i) const
{
#ifdef HAVE_ICU
    Q_D(const DuiLocale);
    UnicodeString str;
    d->_numberFormat->format(i, str);
    return DuiIcuConversions::unicodeStringToQString(str);
#else
    return QString::number(i);
#endif
}

QString DuiLocale::formatNumber(double i, int prec) const
{
#ifdef HAVE_ICU
    Q_D(const DuiLocale);
    icu::UnicodeString str;
    icu::FieldPosition pos;

    if (prec == -1) {
        d->_numberFormat->format(i, str, pos);
    } else {
        // the cached number formatter isn't sufficient
        UErrorCode status = U_ZERO_ERROR;
        icu::NumberFormat *nf;

        nf = icu::NumberFormat::createInstance(d->getCategoryLocale(DuiLcNumeric),
                                               status);

        if (!U_SUCCESS(status)) {
            qWarning() << "NumberFormat creating failed";
            return QString(); // "null" string
        }

        nf->setMaximumFractionDigits(prec);
        nf->format(i, str);
        delete nf;
    }

    return DuiIcuConversions::unicodeStringToQString(str);
#else
    return QString::number(i, 'g', prec);
#endif
}

QString DuiLocale::formatNumber(float i) const
{
#ifdef HAVE_ICU
    Q_D(const DuiLocale);
    icu::UnicodeString str;
    icu::FieldPosition pos;
    d->_numberFormat->format(i, str, pos);
    return DuiIcuConversions::unicodeStringToQString(str);
#else
    return QString::number(i, 'g');
#endif
}

#ifdef HAVE_ICU
QString DuiLocale::formatPercent(double i, int decimals) const
{
    Q_D(const DuiLocale);
    icu::UnicodeString str;
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale numericLocale = d->getCategoryLocale(DuiLcNumeric);
    icu::NumberFormat *nf = NumberFormat::createPercentInstance(numericLocale, status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed";
        return QString();
    }

    nf->setMinimumFractionDigits(decimals);
    nf->format(i, str);
    delete nf;

    return DuiIcuConversions::unicodeStringToQString(str);
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::formatCurrency(double amount, const QString &currency) const
{
    Q_D(const DuiLocale);
    UErrorCode status = U_ZERO_ERROR;
    icu::Locale monetaryLocale = d->getCategoryLocale(DuiLcMonetary);
    icu::NumberFormat *nf = icu::NumberFormat::createCurrencyInstance(monetaryLocale, status);

    if (!U_SUCCESS(status)) {
        qWarning() << "NumberFormat creating failed";
        return QString();
    }

    icu::UnicodeString currencyString = DuiIcuConversions::qStringToUnicodeString(currency);
    nf->setCurrency(currencyString.getTerminatedBuffer(), status);

    if (!U_SUCCESS(status)) {
        qWarning() << "currency setting failed";
        delete nf;
        return QString();
    }

    icu::UnicodeString str;
    nf->format(amount, str);
    delete nf;

    return DuiIcuConversions::unicodeStringToQString(str);
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::formatDateTime(const QDateTime &dateTime, DateType dateType,
                                  TimeType timeType, Calendar calendarType) const
{
    DuiCalendar calendar(calendarType);
    calendar.setDateTime(dateTime);
    return formatDateTime(calendar, dateType, timeType);
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::formatDateTime(const DuiCalendar &duicalendar,
                                  DateType datetype, TimeType timetype) const
{
    if (datetype == DateNone && timetype == TimeNone)
        return QString("");

    Q_D(const DuiLocale);
    // Create calLocale which has the time pattern we want to use
    icu::Locale calLocale = DuiIcuConversions::createLocale(d->categoryName(DuiLcTime),
                            duicalendar.type());

    icu::DateFormat::EStyle dateStyle = DuiIcuConversions::toEStyle(datetype);
    icu::DateFormat::EStyle timeStyle = DuiIcuConversions::toEStyle(timetype);
    icu::DateFormat *df
    = icu::DateFormat::createDateTimeInstance(dateStyle, timeStyle, calLocale);

    // Symbols come from the messages locale
    icu::Locale symbolLocale
    = DuiIcuConversions::createLocale(d->categoryName(DuiLcMessages),
                                      duicalendar.type());

    DateFormatSymbols *dfs = DuiLocalePrivate::createDateFormatSymbols(symbolLocale);

    // This is not nice but seems to be the only way to set the
    // symbols with the public API
    static_cast<SimpleDateFormat *>(df)->adoptDateFormatSymbols(dfs);

    icu::FieldPosition pos;
    icu::UnicodeString resString;
    icu::Calendar *cal = duicalendar.d_ptr->_calendar;
    df->format(*cal, resString, pos);
    delete df;

    return DuiIcuConversions::unicodeStringToQString(resString);
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::formatDateTime(const QDateTime &dateTime, Calendar calendarType) const
{
    return formatDateTime(dateTime, DateLong, TimeLong, calendarType);
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::formatDateTime(const QDateTime &dateTime,
                                  const QString &formatString) const
{
    // convert QDateTime to DuiCalendar and format
    DuiCalendar calendar(*this);
    calendar.setDateTime(dateTime);
    return formatDateTime(calendar, formatString);
}
#endif

#ifdef HAVE_ICU
//! creates a string presentation for a QDateTime with specific format string
//! \param dateTime QDateTime to format
//! \param formatString in ICU SimpleDateFormat format
//! \note THIS MAY BE REMOVED FROM PUBLIC API
QString DuiLocale::formatDateTimeICU(const QDateTime &dateTime,
                                     const QString &formatString) const
{
    // convert QDateTime to DuiCalendar and format
    DuiCalendar calendar(*this);
    calendar.setDateTime(dateTime);
    return formatDateTimeICU(calendar, formatString);
}
#endif

#ifdef HAVE_ICU
//! Formats the date time with ICU pattern
//! \note THIS MAY BE REMOVED FROM PUBLIC API
QString DuiLocale::formatDateTimeICU(const DuiCalendar &duiCalendar,
                                     const QString &formatString) const
{
    Q_D(const DuiLocale);
    // Note: using MESSAGES locale here to get symbols in right language
    UErrorCode status = U_ZERO_ERROR;
    icu::SimpleDateFormat formatter(DuiIcuConversions::qStringToUnicodeString(formatString),
                                    d->getCategoryLocale(DuiLcMessages), status);

    if (U_FAILURE(status)) {
        return QString();
    }

    icu::FieldPosition pos;
    icu::UnicodeString resString;

    formatter.format(*duiCalendar.d_ptr->_calendar, resString, pos);

    return DuiIcuConversions::unicodeStringToQString(resString);
}
#endif

#ifdef HAVE_ICU
// return weeknumber based on first week starting from the first given weekday of the year
// i.e. using sunday week 1 is the first week that contains sunday, zero before it
// note: week also starts from given weekday
// TODO: should this be moved to DuiCalendar?
static int weekNumberStartingFromDay(const DuiCalendar &calendar, int weekday)
{
    DuiCalendar calendarCopy = calendar;
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
QString DuiLocale::formatDateTime(const DuiCalendar &duiCalendar,
                                  const QString &formatString) const
{
    Q_D(const DuiLocale);
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
                icu::Locale msgLocale = d->getCategoryLocale(DuiLcMessages);
                DateFormat *df
                = icu::DateFormat::createDateTimeInstance(icu::DateFormat::kDefault,
                        icu::DateFormat::kDefault,
                        msgLocale);
                icu::UnicodeString dateTime;
                icu::FieldPosition fieldPos;
                dateTime = df->format(*duiCalendar.d_ptr->_calendar, dateTime, fieldPos);
                icuFormat.append('\'');
                QString pattern = DuiIcuConversions::unicodeStringToQString(dateTime);
                icuFormat.append(DuiIcuConversions::escapeIcuDatePattern(pattern));
                icuFormat.append('\'');
                delete df;
                break;
            }

            case 'C': {
                // century, no corresponding icu pattern
                int century = duiCalendar.year() / 100;
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
                // 24 hour clock
                icuFormat.append("kk");
                break;

            case 'I':
                // 12 hour clock
                icuFormat.append("KK");
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

            case 'r':
                // 12 hour clock with am/pm
                icuFormat.append("KK aaa"); // correct?
                break;

            case 'R':
                // 24-hour clock time, in the format "%H:%M"
                icuFormat.append("kk:mm");
                break;

            case 'S':
                // seconds
                icuFormat.append("ss");
                break;

            case 't':
                // tab
                icuFormat.append('\t');
                break;

            case 'T':
                // 24 hour clock HH:MM:SS
                icuFormat.append("kk:mm:ss");
                break;

            case 'u':
                // Weekday, as a decimal number (1(Monday)-7)
                // no corresponding icu pattern for monday based weekday
                icuFormat.append(QString::number(duiCalendar.dayOfWeek()));
                break;

            case 'U': {
                // Week number of the year (Sunday as the first day of the week) as a
                // decimal number (00-53). first week starts from first sunday.
                int weeknumber = weekNumberStartingFromDay(duiCalendar, DuiLocale::Sunday);
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

                DuiCalendar calendarCopy = duiCalendar;
                calendarCopy.setFirstDayOfWeek(DuiLocale::Monday);
                calendarCopy.setMinimalDaysInFirstWeek(4);
                int weeknumber = calendarCopy.weekNumber();
                icuFormat.append(QString::number(weeknumber));
                break;
            }

            case 'w': {
                // Weekday, as a decimal number (0(Sunday)-6)
                int weekday = duiCalendar.dayOfWeek();

                if (weekday == Sunday) {
                    weekday = 0;
                }

                icuFormat.append(QString::number(weekday));
                break;
            }

            case 'W': {
                // Week number of the year (Monday as the first day of the week), as a
                // decimal number (00-53). Week starts from the first monday
                int weeknumber = weekNumberStartingFromDay(duiCalendar, DuiLocale::Monday);
                icuFormat.append(QString::number(weeknumber));
                break;
            }

            case 'x': {
                // appropriate date representation
                icu::Locale msgLocale = d->getCategoryLocale(DuiLcMessages);
                DateFormat *df
                = icu::DateFormat::createDateInstance(icu::DateFormat::kDefault,
                                                      msgLocale);
                icu::UnicodeString dateTime;
                icu::FieldPosition fieldPos;
                dateTime = df->format(*duiCalendar.d_ptr->_calendar, dateTime, fieldPos);
                icuFormat.append('\'');
                QString pattern = DuiIcuConversions::unicodeStringToQString(dateTime);
                icuFormat.append(DuiIcuConversions::escapeIcuDatePattern(pattern));
                icuFormat.append('\'');
                delete df;
                break;
            }

            case 'X': {
                // appropriate time representation
                icu::Locale msgLocale = d->getCategoryLocale(DuiLcMessages);
                DateFormat *df
                = icu::DateFormat::createTimeInstance(icu::DateFormat::kDefault,
                                                      msgLocale);
                icu::UnicodeString dateTime;
                icu::FieldPosition fieldPos;
                dateTime = df->format(*duiCalendar.d_ptr->_calendar, dateTime, fieldPos);
                icuFormat.append('\'');
                QString pattern = DuiIcuConversions::unicodeStringToQString(dateTime);
                icuFormat.append(DuiIcuConversions::escapeIcuDatePattern(pattern));
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

    return formatDateTimeICU(duiCalendar, icuFormat);
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::monthName(const DuiCalendar &duiCalendar, int monthNumber) const
{
    Q_D(const DuiLocale);
    monthNumber--; // months in array starting from index zero

    // use message locale as the output language
    icu::Locale symbolLocale
    = DuiIcuConversions::createLocale(d->categoryName(DuiLcMessages),
                                      duiCalendar.type());

    icu::DateFormatSymbols *dfs = DuiLocalePrivate::createDateFormatSymbols(symbolLocale);

    int len;
    const UnicodeString *months = dfs->getMonths(len);

    QString result;

    if (monthNumber < len && monthNumber >= 0) {
        result = DuiIcuConversions::unicodeStringToQString(months[monthNumber]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::monthName(const DuiCalendar &duiCalendar, int monthNumber,
                             DateSymbolContext context,
                             DateSymbolLength symbolLength) const
{
    Q_D(const DuiLocale);

    monthNumber--; // months in array starting from index zero

    icu::Locale symbolLocale
    = DuiIcuConversions::createLocale(d->categoryName(DuiLcMessages),
                                      duiCalendar.type());

    icu::DateFormatSymbols *dfs = DuiLocalePrivate::createDateFormatSymbols(symbolLocale);

    // map context type
    icu::DateFormatSymbols::DtContextType icuContext =
        DuiIcuConversions::duiDateContextToIcu(context);

    // map length type
    icu::DateFormatSymbols::DtWidthType icuWidth =
        DuiIcuConversions::duiDateWidthToIcu(symbolLength);

    int len;
    const UnicodeString *months = dfs->getMonths(len, icuContext, icuWidth);

    QString result;

    if (monthNumber < len && monthNumber >= 0) {
        result = DuiIcuConversions::unicodeStringToQString(months[monthNumber]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::weekdayName(const DuiCalendar &duiCalendar, int weekday) const
{
    Q_D(const DuiLocale);
    // use message locale as the output language
    icu::Locale symbolLocale
    = DuiIcuConversions::createLocale(d->categoryName(DuiLcMessages),
                                      duiCalendar.type());

    icu::DateFormatSymbols *dfs = DuiLocalePrivate::createDateFormatSymbols(symbolLocale);

    int len;
    const UnicodeString *weekdayNames = dfs->getWeekdays(len);
    int weekdayNum = DuiIcuConversions::icuWeekday(weekday);

    QString result;

    if (weekdayNum < len && weekdayNum > 0) {
        result = DuiIcuConversions::unicodeStringToQString(weekdayNames[weekdayNum]);
    }

    delete dfs;

    return result;
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::weekdayName(const DuiCalendar &duiCalendar, int weekday,
                               DateSymbolContext context,
                               DateSymbolLength symbolLength) const
{
    Q_D(const DuiLocale);
    icu::Locale symbolLocale
    = DuiIcuConversions::createLocale(d->categoryName(DuiLcMessages),
                                      duiCalendar.type());

    icu::DateFormatSymbols *dfs = DuiLocalePrivate::createDateFormatSymbols(symbolLocale);

    icu::DateFormatSymbols::DtContextType icuContext
    = DuiIcuConversions::duiDateContextToIcu(context);

    icu::DateFormatSymbols::DtWidthType icuWidth
    = DuiIcuConversions::duiDateWidthToIcu(symbolLength);

    int len;
    const UnicodeString *weekdayNames = dfs->getWeekdays(len, icuContext, icuWidth);
    int weekdayNum = DuiIcuConversions::icuWeekday(weekday);

    QString result;

    if (weekdayNum < len && weekdayNum > 0) {
        result = DuiIcuConversions::unicodeStringToQString(weekdayNames[weekdayNum]);
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
            duiDebug("duilocale.cpp") << "Error ures_open" << u_errorName(status);
        }

        UResourceBundle *posixData = ures_getByKey(extraData, "posixdata", NULL, &status);

        if (U_FAILURE(status)) {
            duiDebug("duilocale.cpp") << "Error ures_open posixData" << u_errorName(status);
        }

        UResourceBundle *nameFmts = ures_getByKey(posixData, "nameFmts", NULL, &status);
        if (U_FAILURE(status)) {
            duiDebug("duilocale.cpp") << "Error ures_open nameFmts" << u_errorName(status);
        }

        ures_close(extraData);
        ures_close(posixData);

        return nameFmts;
    }
}
#endif

#ifdef HAVE_ICU
QString DuiLocale::languageEndonym() const
{
    Q_D(const DuiLocale);
    UErrorCode status = U_ZERO_ERROR;

    UResourceBundle *res = ures_open(NULL, qPrintable(d->_defaultLocale), &status);
    if (U_FAILURE(status)) {
        duiDebug("DuiLocale") << "Error ures_open" << u_errorName(status);
    }

    res = ures_getByKey(res, Languages, res, &status);
    if (U_FAILURE(status)) {
        duiDebug("DuiLocale") << "Error ures_getByKey" << u_errorName(status);
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
QString DuiLocale::countryEndonym() const
{
    Q_D(const DuiLocale);
    UErrorCode status = U_ZERO_ERROR;

    UResourceBundle *res = ures_open(NULL, qPrintable(d->_defaultLocale), &status);
    if (U_FAILURE(status)) {
        duiDebug("DuiLocale") << "Error ures_open" << u_errorName(status);
    }

    res = ures_getByKey(res, Countries, res, &status);
    if (U_FAILURE(status)) {
        duiDebug("DuiLocale") << "Error ures_getByKey" << u_errorName(status);
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

void DuiLocale::installCategoryCatalog(Category category, const QString &name)
{
    Q_D(DuiLocale);

    if (category != DuiLcMessages && category != DuiLcTime) {
        duiDebug("DuiLocale") << __PRETTY_FUNCTION__ << ": unknown category";
        return;
    }

    // don't load the catalog if it has been loaded already.
    //
    // The following function cannot be called because it is marked as
    // deprecated, therefore, I copy the code from
    // hasCategoryCatalog() here:
    //
    // if(hasCategoryCatalog(category, name)) {
    //    return;
    //}
    bool hasCategoryCatalog = false;
    if (category != DuiLcMessages && category != DuiLcTime) {
        hasCategoryCatalog = false;
    } else {
        DuiLocalePrivate::CatalogList *catList;

        if (category == DuiLcMessages) {
            catList = &d->_messageTranslations;
        } else {
            catList = &d->_timeTranslations;
        }

        // find the DuiTranslationCatalog
        for (DuiLocalePrivate::CatalogList::iterator i = catList->begin();
                i != catList->end(); ++i) {
            if (name == (*i)->_name) {
                hasCategoryCatalog = true;
            }
        }
    }
    if (hasCategoryCatalog)
        return;

    DuiTranslationCatalog *catalog = new DuiTranslationCatalog(name);
    catalog->loadWith(this, category);

    if (category == DuiLcMessages) {
        d->_messageTranslations.append(QExplicitlySharedDataPointer<DuiTranslationCatalog>(catalog));
    } else {
        d->_timeTranslations.append(QExplicitlySharedDataPointer<DuiTranslationCatalog>(catalog));
    }
}

bool DuiLocale::hasCategoryCatalog(Category category, const QString &name)
{
    Q_D(DuiLocale);

    if (category != DuiLcMessages && category != DuiLcTime) {
        return false;
    }

    DuiLocalePrivate::CatalogList *catList;

    if (category == DuiLcMessages) {
        catList = &d->_messageTranslations;
    } else {
        catList = &d->_timeTranslations;
    }

    // find the DuiTranslationCatalog
    for (DuiLocalePrivate::CatalogList::iterator i = catList->begin();
            i != catList->end(); ++i) {
        if (name == (*i)->_name) {
            return true;
        }
    }

    return false;
}

void DuiLocale::removeCategoryCatalog(Category category, const QString &name)
{
    Q_D(DuiLocale);
    if (category != DuiLcMessages && category != DuiLcTime) {
        return;
    }

    DuiLocalePrivate::CatalogList *catList;

    if (category == DuiLcMessages) {
        catList = &d->_messageTranslations;
    } else {
        catList = &d->_timeTranslations;
    }

    // find, delete and remove the DuiTranslationCatalog
    for (DuiLocalePrivate::CatalogList::iterator i = catList->begin();
            i != catList->end(); ++i) {
        if (name == (*i)->_name) {
            // erasing should delete the duitranslationcatalog if necessary
            catList->erase(i);
        }
    }
}

void DuiLocale::copyCatalogsFrom(const DuiLocale &other)
{
    Q_D(DuiLocale);

    DuiLocalePrivate::CatalogList::const_iterator end =
        other.d_ptr->_messageTranslations.constEnd();

    for (DuiLocalePrivate::CatalogList::const_iterator i
            = other.d_ptr->_messageTranslations.constBegin();
            i != end; ++i) {

        // copy the name info
        DuiTranslationCatalog *tempCatalog = new DuiTranslationCatalog(**i);

        // reload the file
        tempCatalog->loadWith(this, DuiLcMessages);
        d->_messageTranslations.append(QExplicitlySharedDataPointer<DuiTranslationCatalog>(tempCatalog));

    }

    end = other.d_ptr->_timeTranslations.constEnd();
    for (DuiLocalePrivate::CatalogList::const_iterator i = other.d_ptr->_timeTranslations.constBegin();
            i != end; ++i) {

        DuiTranslationCatalog *tempCatalog = new DuiTranslationCatalog(**i);
        tempCatalog->loadWith(this, DuiLcTime);
        d->_timeTranslations.append(QExplicitlySharedDataPointer<DuiTranslationCatalog>(tempCatalog));
    }

    end = other.d_ptr->_trTranslations.constEnd();
    for (DuiLocalePrivate::CatalogList::const_iterator i = other.d_ptr->_trTranslations.constBegin();
            i != end; ++i) {

        DuiTranslationCatalog *tempCatalog = new DuiTranslationCatalog(**i);
        tempCatalog->loadWith(this, DuiLcMessages);
        d->_trTranslations.append(QExplicitlySharedDataPointer<DuiTranslationCatalog>(tempCatalog));

    }

}

void DuiLocale::installTrCatalog(const QString &name)
{
    Q_D(DuiLocale);

    DuiTranslationCatalog *catalog
        = new DuiTranslationCatalog(name);
    d->_trTranslations.append(QExplicitlySharedDataPointer<DuiTranslationCatalog>(catalog));
    if (!name.endsWith(QLatin1String(".qm"))) {
        DuiTranslationCatalog *engineeringEnglishCatalog
            = new DuiTranslationCatalog(name + ".qm");
        d->_trTranslations.prepend(QExplicitlySharedDataPointer<DuiTranslationCatalog>(engineeringEnglishCatalog));
    }
    d->loadTrCatalogs();
}

void DuiLocale::removeTrCatalog(const QString &name)
{
    Q_D(DuiLocale);
    DuiLocalePrivate::CatalogList::iterator it = d->_trTranslations.begin();
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

void DuiLocale::loadTrCatalogs()
{
    Q_D(DuiLocale);

    d->loadTrCatalogs();
}

void DuiLocale::insertTrToQCoreApp()
{
    Q_D(DuiLocale);
    foreach(const QExplicitlySharedDataPointer<DuiTranslationCatalog>& sharedCatalog, d->_trTranslations) {
        QCoreApplication::installTranslator(&sharedCatalog->_translator);
    }
}

void DuiLocale::removeTrFromQCoreApp()
{
    Q_D(DuiLocale);
    foreach(const QExplicitlySharedDataPointer<DuiTranslationCatalog>& sharedCatalog, d->_trTranslations) {
        QCoreApplication::removeTranslator(&sharedCatalog->_translator);
    }
}

/////////////////////////////
//// translation methods ////

QString DuiLocale::translate(const char *context, const char *sourceText,
                             const char *comment, int n)
{
    Q_D(DuiLocale);
    const DuiLocalePrivate::CatalogList::const_iterator begin = d->_trTranslations.constBegin();
    DuiLocalePrivate::CatalogList::const_iterator it = d->_trTranslations.constEnd();
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

void DuiLocale::setDataPaths(const QStringList &dataPaths)
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
void DuiLocale::setDataPath(const QString &dataPath)
{
    setDataPaths(QStringList(dataPath));
}

///////
// the static convenience methods for translation

void DuiLocale::setTranslationPaths(const QStringList &paths)
{
    DuiLocalePrivate::translationPaths = paths;
}

void DuiLocale::addTranslationPath(const QString &path)
{
    if (DuiLocalePrivate::translationPaths.contains(path) == false) {
        DuiLocalePrivate::translationPaths << path;
    }
}

void DuiLocale::removeTranslationPath(const QString &path)
{
    DuiLocalePrivate::translationPaths.removeOne(path);
}

QStringList DuiLocale::translationPaths()
{
    return DuiLocalePrivate::translationPaths;
}

Qt::LayoutDirection DuiLocale::textDirection() const
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

void DuiLocale::refreshSettings()
{
    Q_D(DuiLocale);
#ifdef HAVE_GCONF
    QString language = d->currentLanguageItem.value().toString();
    QString country  = d->currentCountryItem.value().toString();
    QString script   = d->currentScriptItem.value().toString();
    QString variant  = d->currentVariantItem.value().toString();
    QString localeName = d->createLocaleString(language, country, script, variant);

    if (localeName != d->_defaultLocale) {
        d->_defaultLocale = localeName;
        setCategoryLocale(DuiLcMessages, localeName);
        setCategoryLocale(DuiLcTime, localeName);
        setCategoryLocale(DuiLcNumeric, localeName);
        setCategoryLocale(DuiLcCollate, localeName);
        setCategoryLocale(DuiLcMonetary, localeName);
        setCategoryLocale(DuiLcName, localeName);

        if (this == s_systemDefault) {
            QCoreApplication::processEvents();
            // Setting the default QLocale is needed to get localized number
            // support in translations via %Ln, %L1, %L2, ...:
            QLocale qlocale(this->language() + '_' + this->country());
            QLocale::setDefault(qlocale);
            this->removeTrFromQCoreApp();
            d->loadTrCatalogs();
            // sends QEvent::LanguageChange to qApp:
            this->insertTrToQCoreApp();
            // sends QEvent::ApplicationLayoutDirectionChange to qApp:
            qApp->setLayoutDirection(this->textDirection());
            // the following 2 events have already been sent above:
            // QEvent ev(QEvent::LanguageChange);
            // qApp->sendEvent(qApp, &ev);
            // QEvent dirEv(QEvent::ApplicationLayoutDirectionChange);
            // qApp->sendEvent(qApp, &dirEv);
            QCoreApplication::processEvents();
        }
        else {
            d->loadTrCatalogs();
        }
        emit settingsChanged();
    }
#endif
}

