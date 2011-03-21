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

#ifndef MLOCALE_P_H
#define MLOCALE_P_H

#include <QSet>
#include <QList>
#include <QString>
#include <QStringList>
#include <QExplicitlySharedDataPointer>
#include <QLocale>
#include <QCache>

#ifdef HAVE_ICU
#include <unicode/datefmt.h>
#include <unicode/numfmt.h>
#include <unicode/decimfmt.h>
#include <unicode/unistr.h>
#include <unicode/dtfmtsym.h>
#endif

#ifdef HAVE_GCONF
#include "mgconfitem.h"
#endif

#include "mlocale.h"

class QString;
class MTranslationCatalog;

class MLocalePrivate
{
    Q_DECLARE_PUBLIC(MLocale)

public:
    MLocalePrivate();
    MLocalePrivate(const MLocalePrivate &other);

    virtual ~MLocalePrivate();


    MLocalePrivate &operator=(const MLocalePrivate &other);

    typedef QList<QExplicitlySharedDataPointer<MTranslationCatalog> > CatalogList;

    // return string for a category, default if category is not set
    QString categoryName(MLocale::Category category) const;

    /*!
     * \brief loads the QTranslators for all current catalogs
     */
    void loadTrCatalogs();

    /*!
     * \brief Installs the translations into QCoreApplication
     */
    void insertTrToQCoreApp();

    /*!
     * \brief Removes the translations from QCoreApplication
     */
    void removeTrFromQCoreApp();

    /*!
     * \brief inserts a text direction translator into QCoreApplication
     *
     * This is a special translator used to make QCoreApplication
     * detect the correct layout direction used for the language
     * of the current locale. For example, in case of Arabic or
     * Hebrew right-to-left layout is used.
     */
    void insertDirectionTrToQCoreApp();

    QLocale createQLocale(MLocale::Category catetory) const;

    void setCategoryLocale(MLocale *mlocale, MLocale::Category category,
                           const QString &localeName);

#ifdef HAVE_ICU
    /*!
     * \brief tries to truncate a locale name to search for fallbacks
     * \param a pointer to a QString containing an ICU locale name
     *
     * When searching for ICU resource bundles or when trying to
     * get ICU resource strings by keys it is sometimes necessary
     * to use fallbacks, i.e. when nothing is found for the
     * locale name “en_US”, try “en” as well. This function
     * tries to truncate the locale name given as the parameter to
     * the next fallback to try. If truncating was possible and
     * the function returns true. If no further truncating is
     * possible it returns false and the locale name given
     * as the parameter is left unchanged. I.e. if false is returned
     * there are no fallbacks left to try.
     */
    static bool truncateLocaleName(QString *localeName);

    // creates an icu::Locale for specific category
    icu::Locale getCategoryLocale(MLocale::Category category) const;

    static icu::DateFormatSymbols *createDateFormatSymbols(const icu::Locale &locale);

    // checks if an ICU format string is a twelve hour format string or not
    bool isTwelveHours(const QString &icuFormatQString) const;
    // converts an ICU date format to 24 hour clock
    void dateFormatTo24h(icu::DateFormat *df) const;
    // converts an ICU date format to 12 hour clock
    void dateFormatTo12h(icu::DateFormat *df) const;

    QString fixCategoryNameForNumbers(const QString &categoryName) const;
    /*!
     * \brief returns ICU date and time format string of the current locale
     * \param dateType style of date formatting
     * \param timeType style of time formatting
     * \param calendarType calendar to use for formatting
     * \param timeFormat24h enum to choose 12 hour or 24 hour or default time format
     */
    QString icuFormatString(MLocale::DateType dateType,
                            MLocale::TimeType timeType,
                            MLocale::CalendarType calendarType,
                            MLocale::TimeFormat24h timeFormat24h) const;

    // creates a dateformat object for datetime formatting/parsing
    // the caller is responsible for deleting the dateformat object
    icu::DateFormat *createDateFormat(MLocale::DateType dateType,
                                      MLocale::TimeType timeType,
                                      MLocale::CalendarType calendarType,
                                      MLocale::TimeFormat24h timeFormat24h) const;
#endif

    static bool parseIcuLocaleString(const QString &localeString, QString *language, QString *script, QString *country, QString *variant);
    // these return the requested part of a locale string,
    // e.g. parseLanguage("fi_FI") -> "fi"
    static QString parseLanguage(const QString &localeString);
    static QString parseCountry(const QString &localeString);
    static QString parseScript(const QString &localeString);
    static QString parseVariant(const QString &localeString);

    QString formatPhoneNumber( const QString& phoneNumber,
                               MLocale::PhoneNumberGrouping grouping ) const;

    bool isValidCountryCode( const QString& code ) const;

    void dropCaches();

    bool _valid;

    // the default locale is used for messages and other categories if not
    // overridden
    QString _defaultLocale;
    QString _messageLocale;
    QString _numericLocale;
    QString _collationLocale;
    QString _calendarLocale;
    QString _monetaryLocale;
    QString _nameLocale;
    QString _telephoneLocale;

    // the list of valid country codes for the formatPhoneNumber function
    QSet<QString> _validCountryCodes;

    MLocale::TimeFormat24h _timeFormat24h;

    MLocale::PhoneNumberGrouping _phoneNumberGrouping;

#ifdef HAVE_ICU
    void removeDirectionalFormattingCodes(QString *str) const;
    void fixFormattedNumberForRTL(QString *formattedNumber) const;
    // number format caching for better performance.
    icu::NumberFormat *_numberFormat;
    icu::NumberFormat *_numberFormatLcTime;
    mutable QCache<QString, icu::DateFormat> _dateFormatCache;
    mutable QCache<QString, icu::SimpleDateFormat> _simpleDateFormatCache;
    mutable QCache<QString, QString> _icuFormatStringCache;
#endif

    // translations for two supported translation categories
    CatalogList _messageTranslations;
    CatalogList _timeTranslations;

    // tr translations are kept separate because they have to be loaded when inserted
    // into QCoreApplication
    CatalogList _trTranslations;

    static QStringList dataPaths;
    static QStringList translationPaths;

#ifdef HAVE_GCONF
    MGConfItem currentLanguageItem;
    MGConfItem currentLcTimeItem;
    MGConfItem currentLcTimeFormat24hItem;
    MGConfItem currentLcCollateItem;
    MGConfItem currentLcNumericItem;
    MGConfItem currentLcMonetaryItem;
    MGConfItem currentLcTelephoneItem;
#endif

    // calendar instance used formatDateTimeICU()
#ifdef HAVE_ICU
    MCalendar *_pDateTimeCalendar;
#endif

    MLocale *q_ptr;
};

#endif
