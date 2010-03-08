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

#ifndef DUILOCALE_P_H
#define DUILOCALE_P_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QExplicitlySharedDataPointer>

#ifdef HAVE_ICU
#include <unicode/datefmt.h>
#include <unicode/numfmt.h>
#include <unicode/unistr.h>
#include <unicode/dtfmtsym.h>
#endif

#ifdef HAVE_GCONF
#include "duigconfitem.h"
#endif

#include "duilocale.h"

class QString;
class DuiTranslationCatalog;

class DuiLocalePrivate
{
    Q_DECLARE_PUBLIC(DuiLocale)

public:
    DuiLocalePrivate();
    DuiLocalePrivate(const DuiLocalePrivate &other);

    virtual ~DuiLocalePrivate();


    DuiLocalePrivate &operator=(const DuiLocalePrivate &other);


    static QString categoryToDirectoryName(DuiLocale::Category category);

    // returns string presentation for category.
    static QString createLocaleString(const QString &language,
                                      const QString &country,
                                      const QString &script,
                                      const QString &variant);

    typedef QList<QExplicitlySharedDataPointer<DuiTranslationCatalog> > CatalogList;

    // return string for a category, default if category is not set
    QString categoryName(DuiLocale::Category category) const;

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

    void setCategoryLocale(DuiLocale *duilocale, DuiLocale::Category category,
                           const QString &localeName);

#ifdef HAVE_ICU
    // creates an icu::Locale for specific category
    icu::Locale getCategoryLocale(DuiLocale::Category category) const;

    static icu::DateFormatSymbols *createDateFormatSymbols(icu::Locale locale);
#endif

    // these return the requested part of a locale string,
    // e.g. parseLanguage("fi_FI") -> "fi"
    static QString parseLanguage(const QString &localeString);
    static QString parseCountry(const QString &localeString);
    static QString parseScript(const QString &localeString);
    static QString parseVariant(const QString &localeString);

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

    // the used calendar and collation may be overridden
    DuiLocale::Calendar _calendar;
    DuiLocale::Collation _collation;
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
    DuiGConfItem currentLanguageItem;
    DuiGConfItem currentCountryItem;
    DuiGConfItem currentScriptItem;
    DuiGConfItem currentVariantItem;
#endif

    DuiLocale *q_ptr;
};

#endif
