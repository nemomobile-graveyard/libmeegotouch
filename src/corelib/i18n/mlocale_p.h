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

#ifdef HAVE_ICU
#include <unicode/datefmt.h>
#include <unicode/numfmt.h>
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

    // returns string presentation for category.
    static QString createLocaleString(const QString &language,
                                      const QString &country,
                                      const QString &script,
                                      const QString &variant);

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

    QLocale createQLocale(MLocale::Category catetory) const;

    void setCategoryLocale(MLocale *mlocale, MLocale::Category category,
                           const QString &localeName);

#ifdef HAVE_ICU
    // creates an icu::Locale for specific category
    icu::Locale getCategoryLocale(MLocale::Category category) const;

    static icu::DateFormatSymbols *createDateFormatSymbols(const icu::Locale &locale);

    // creates a dateformat object for datetime formatting/parsing
    // the caller is responsible for deleting the dateformat object
    icu::DateFormat *createDateFormat(MLocale::DateType dateType,
                                      MLocale::TimeType timeType,
                                      const MCalendar& mcalendar) const;
#endif

    // these return the requested part of a locale string,
    // e.g. parseLanguage("fi_FI") -> "fi"
    static QString parseLanguage(const QString &localeString);
    static QString parseCountry(const QString &localeString);
    static QString parseScript(const QString &localeString);
    static QString parseVariant(const QString &localeString);

    void loadCountryCodes();

    QString formatPhoneNumber( const QString& phoneNumber,
                               MLocale::PhoneNumberGrouping grouping ) const;

    bool isValidCountryCode( const QString& code ) const;

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

    // the list of valid country codes for the formatPhoneNumber function
    QSet<QString> _validCountryCodes;

    // the used calendar and collation may be overridden
    MLocale::CalendarType _calendarType;
    MLocale::Collation _collation;
    // add currency?

#ifdef HAVE_ICU
    // number format caching for better performance.
    icu::NumberFormat *_numberFormat;
#endif

    // translations for two supported translation categories
    CatalogList _messageTranslations;
    CatalogList _timeTranslations;

    // tr translations are kept separate because they have to be loaded when inserted
    // into QCoreApplication
    CatalogList _trTranslations;

    static QStringList translationPaths;

#ifdef HAVE_GCONF
    MGConfItem currentLanguageItem;
    MGConfItem currentLcTimeItem;
    MGConfItem currentLcCollateItem;
    MGConfItem currentLcNumericItem;
    MGConfItem currentLcMonetaryItem;
#endif

    MLocale *q_ptr;
};

#endif
