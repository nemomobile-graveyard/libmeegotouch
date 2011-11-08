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

#include "mlocale.h"

#include <QStringList>
#include <QMutex>

#include "mlocale/mlocale.h"
#include "mcalendar.h"
#include "mlocale/mcalendar.h"
#include "mcollator.h"
#include "mlocale/mcollator.h"
#include "mlocale_proxy_p.h"
#include "mcalendar_proxy_p.h"
#include "mcollator_proxy_p.h"
#include "mlocalegconfconfigitemfactory.h"


static bool g_configItemFactorySet = false;

static bool g_translationPathsInitialized = false;

MLocalePrivate::MLocalePrivate() : pLocale( 0 )
{
    // make sure we set the right translation paths by default when
    // creating the first MLocalePrivate
    if ( ! g_translationPathsInitialized )
    {
        if ( ML10N::MLocale::translationPaths().isEmpty() )
        {
            ML10N::MLocale::setTranslationPaths(
                (QStringList() << QString(TRANSLATION_DIR)) );
        }

        g_translationPathsInitialized = true;
    }

    if ( ! g_configItemFactorySet )
    {
        ML10N::MLocale::setConfigItemFactory( new MLocaleGConfConfigItemFactory );
        g_configItemFactorySet = true;
    }
}

MLocalePrivate::~MLocalePrivate()
{
    delete pLocale;
}


void MLocale::initMLocale()
{
    // now we set the gconf config item factory.
    ML10N::MLocale::setConfigItemFactory( new MLocaleGConfConfigItemFactory );
    g_configItemFactorySet = true;
}

void MLocale::clearSystemDefault()
{
    ML10N::MLocale::clearSystemDefault();
}

MLocale * MLocale::createSystemMLocale()
{
    // can we do this more elegant ?
    MLocale *locale = new MLocale;

    delete locale->d_ptr->pLocale;
    locale->d_ptr->pLocale = ML10N::MLocale::createSystemMLocale();

    return locale;
}

MLocale MLocale::createCLocale()
{
    const char *const PosixStr = "en_US_POSIX";
    return MLocale(PosixStr);
}

void MLocale::connectSettings()
{
    d_ptr->pLocale->connectSettings();
    connect( d_ptr->pLocale, SIGNAL( settingsChanged() ),
             this, SIGNAL( settingsChanged() ) );
}

void MLocale::disconnectSettings()
{
    d_ptr->pLocale->disconnectSettings();
    disconnect( d_ptr->pLocale, SIGNAL( settingsChanged() ),
                this, SIGNAL( settingsChanged() ) );
}

MLocale::MLocale(QObject *parent)
    : QObject(parent),
      d_ptr(new MLocalePrivate)
{
    d_ptr->pLocale = new ML10N::MLocale( 0 );
    connect( d_ptr->pLocale, SIGNAL(localeSettingsChanged()),
             this, SIGNAL(localeSettingsChanged()));
}

MLocale::MLocale(const QString &localeName, QObject *parent)
    : QObject(parent),
      d_ptr(new MLocalePrivate)
{
    d_ptr->pLocale = new ML10N::MLocale( localeName, 0 );
    connect( d_ptr->pLocale, SIGNAL(localeSettingsChanged()),
             this, SIGNAL(localeSettingsChanged()));
}

MLocale::MLocale(const MLocale &other, QObject *parent)
    : QObject(parent),
      d_ptr(new MLocalePrivate)
{
    d_ptr->pLocale = new ML10N::MLocale( *other.d_ptr->pLocale );
    connect( d_ptr->pLocale, SIGNAL(localeSettingsChanged()),
             this, SIGNAL(localeSettingsChanged()));
}

MLocale::~MLocale()
{
    delete d_ptr;
}

MLocale &MLocale::operator=(const MLocale &other)
{
    if (this == &other) {
        return *this;
    }

    *d_ptr->pLocale = *other.d_ptr->pLocale;

    return *this;
}


MLocale *MLocale::s_systemDefault = 0;
static QMutex defaultLocaleMutex;

void MLocale::setDefault(const MLocale &locale)
{
    defaultLocaleMutex.lock();
    ML10N::MLocale::setDefault( *locale.d_ptr->pLocale );

    if (s_systemDefault == 0)
    {
        s_systemDefault = new MLocale(locale);
    }
    else if (&locale == s_systemDefault || locale.d_ptr == s_systemDefault->d_ptr)
    {
        defaultLocaleMutex.unlock();
        return;
    }
    else
    {
        *s_systemDefault = locale;
    }

    defaultLocaleMutex.unlock();
}

MLocale &MLocale::getDefault()
{
    if (s_systemDefault == 0)
    {
        defaultLocaleMutex.lock();

        if (s_systemDefault == 0)
        {
            s_systemDefault = createSystemMLocale();
        }

        defaultLocaleMutex.unlock();
    }

    return *s_systemDefault;
}

bool MLocale::isValid() const
{
    return d_ptr->pLocale->isValid();
}

void MLocale::setCategoryLocale(Category category, const QString &localeName)
{
    d_ptr->pLocale->setCategoryLocale( (ML10N::MLocale::Category)category, localeName );
}

void MLocale::setCollation(Collation collation)
{
    d_ptr->pLocale->setCollation( (ML10N::MLocale::Collation)collation );
}

MLocale::Collation MLocale::collation() const
{
    return (MLocale::Collation)d_ptr->pLocale->collation();
}

void MLocale::setCalendarType(CalendarType calendarType)
{
    d_ptr->pLocale->setCalendarType( (ML10N::MLocale::CalendarType)calendarType );
}

MLocale::CalendarType MLocale::calendarType() const
{
    return (MLocale::CalendarType)d_ptr->pLocale->calendarType();
}

void MLocale::setTimeFormat24h(TimeFormat24h timeFormat24h)
{
    d_ptr->pLocale->setTimeFormat24h( (ML10N::MLocale::TimeFormat24h)timeFormat24h );
}

MLocale::TimeFormat24h MLocale::timeFormat24h() const
{
    return (MLocale::TimeFormat24h)d_ptr->pLocale->timeFormat24h();
}

#ifdef HAVE_ICU
MLocale::TimeFormat24h MLocale::defaultTimeFormat24h() const
{
    return (MLocale::TimeFormat24h)d_ptr->pLocale->defaultTimeFormat24h();
}

MCollator MLocale::collator() const
{
    return MCollator(*this);
}
#endif

QString MLocale::toLower(const QString &string) const
{
    return d_ptr->pLocale->toLower( string );
}

QString MLocale::toUpper(const QString &string) const
{
    return d_ptr->pLocale->toUpper( string );
}

QString MLocale::language() const
{
    return d_ptr->pLocale->language();
}

QString MLocale::country() const
{
    return d_ptr->pLocale->country();
}

QString MLocale::script() const
{
    return d_ptr->pLocale->script();
}

QString MLocale::variant() const
{
    return d_ptr->pLocale->variant();
}

QString MLocale::name() const
{
    return d_ptr->pLocale->name();
}

QString MLocale::categoryLanguage(Category category) const
{
    return d_ptr->pLocale->categoryLanguage( (ML10N::MLocale::Category)category );
}

QString MLocale::categoryCountry(Category category) const
{
    return d_ptr->pLocale->categoryCountry( (ML10N::MLocale::Category)category );
}

QString MLocale::categoryScript(Category category) const
{
    return d_ptr->pLocale->categoryScript( (ML10N::MLocale::Category)category );
}

QString MLocale::categoryVariant(Category category) const
{
    return d_ptr->pLocale->categoryVariant( (ML10N::MLocale::Category)category );
}

QString MLocale::categoryName(Category category) const
{
    return d_ptr->pLocale->categoryName( (ML10N::MLocale::Category)category );
}

QString MLocale::formatNumber(qlonglong i) const
{
    return d_ptr->pLocale->formatNumber( i );
}

qlonglong MLocale::toLongLong(const QString &s, bool *ok) const
{
    return d_ptr->pLocale->toLongLong( s, ok );
}

QString MLocale::formatNumber(short i) const
{
    return d_ptr->pLocale->formatNumber( i );
}

short MLocale::toShort(const QString &s, bool *ok) const
{
    return d_ptr->pLocale->toShort( s, ok );
}

QString MLocale::formatNumber(int i) const
{
    return d_ptr->pLocale->formatNumber( i );
}

int MLocale::toInt(const QString &s, bool *ok) const
{
    return d_ptr->pLocale->toInt( s, ok );
}

QString MLocale::formatNumber(double i, int maxPrecision) const
{
    return d_ptr->pLocale->formatNumber( i, maxPrecision );
}

QString MLocale::formatNumber(double i, int maxPrecision, int minPrecision) const
{
    return d_ptr->pLocale->formatNumber( i, maxPrecision, minPrecision );
}

double MLocale::toDouble(const QString &s, bool *ok) const
{
    return d_ptr->pLocale->toDouble( s, ok );
}

QString MLocale::formatNumber(float i) const
{
    return d_ptr->pLocale->formatNumber( i );
}

float MLocale::toFloat(const QString &s, bool *ok) const
{
    return d_ptr->pLocale->toFloat( s, ok );
}

#ifdef HAVE_ICU
QString MLocale::formatPercent(double i, int decimals) const
{
    return d_ptr->pLocale->formatPercent( i, decimals );
}
#endif

QString MLocale::formatCurrency(double amount, const QString &currency) const
{
    return d_ptr->pLocale->formatCurrency( amount, currency );
}

QString MLocale::formatDateTime(const QDateTime &dateTime, DateType dateType,
                                TimeType timeType, CalendarType calendarType) const
{
    return d_ptr->pLocale->formatDateTime( dateTime,
                                           (ML10N::MLocale::DateType)dateType,
                                           (ML10N::MLocale::TimeType)timeType,
                                           (ML10N::MLocale::CalendarType)calendarType );
}

#ifdef HAVE_ICU
QString MLocale::formatDateTime(const MCalendar &mCalendar,
                                DateType dateType, TimeType timeType) const
{
    return d_ptr->pLocale->formatDateTime( *mCalendar.d_ptr->pCalendar,
                                           (ML10N::MLocale::DateType)dateType,
                                           (ML10N::MLocale::TimeType)timeType );
}

QString MLocale::formatDateTime(const QDateTime &dateTime, CalendarType calendarType) const
{
    return d_ptr->pLocale->formatDateTime( dateTime,
                                           ML10N::MLocale::DateLong,
                                           ML10N::MLocale::TimeLong,
                                           (ML10N::MLocale::CalendarType)calendarType );
}

QString MLocale::formatDateTime(const QDateTime &dateTime,
                                const QString &formatString) const
{
    return d_ptr->pLocale->formatDateTime( dateTime, formatString );
}

QString MLocale::formatDateTimeICU(const QDateTime &dateTime,
                                   const QString &formatString) const
{
    return d_ptr->pLocale->formatDateTimeICU( dateTime, formatString );
}

QString MLocale::formatDateTimeICU(const MCalendar &mCalendar,
                                   const QString &formatString) const
{
    return d_ptr->pLocale->formatDateTimeICU( *mCalendar.d_ptr->pCalendar, formatString );
}

QString MLocale::formatDateTime(const MCalendar &mCalendar,
                                const QString &formatString) const
{
    return d_ptr->pLocale->formatDateTime( *mCalendar.d_ptr->pCalendar, formatString );
}

QString MLocale::icuFormatString( DateType dateType,
                                  TimeType timeType,
                                  CalendarType calendarType) const
{
    return d_ptr->pLocale->icuFormatString( (ML10N::MLocale::DateType)dateType,
                                            (ML10N::MLocale::TimeType)timeType,
                                            (ML10N::MLocale::CalendarType)calendarType );
}

QDateTime MLocale::parseDateTime(const QString &dateTime, DateType dateType,
                                 TimeType timeType, CalendarType calendarType) const
{
    return d_ptr->pLocale->parseDateTime( dateTime,
                                          (ML10N::MLocale::DateType)dateType,
                                          (ML10N::MLocale::TimeType)timeType,
                                          (ML10N::MLocale::CalendarType)calendarType );
}

QDateTime MLocale::parseDateTime(const QString &dateTime, CalendarType calendarType) const
{
    return d_ptr->pLocale->parseDateTime( dateTime,
                                          ML10N::MLocale::DateLong,
                                          ML10N::MLocale::TimeLong,
                                          (ML10N::MLocale::CalendarType)calendarType );
}

QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber) const
{
    return d_ptr->pLocale->monthName( *mCalendar.d_ptr->pCalendar, monthNumber );
}

QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber,
                           DateSymbolContext context,
                           DateSymbolLength symbolLength) const
{
    return d_ptr->pLocale->monthName( *mCalendar.d_ptr->pCalendar,
                                      monthNumber,
                                      (ML10N::MLocale::DateSymbolContext)context,
                                      (ML10N::MLocale::DateSymbolLength)symbolLength );
}

QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday) const
{
    return d_ptr->pLocale->weekdayName( *mCalendar.d_ptr->pCalendar, weekday );
}

QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday,
                               DateSymbolContext context,
                               DateSymbolLength symbolLength) const
{
    return d_ptr->pLocale->weekdayName( *mCalendar.d_ptr->pCalendar,
                                        weekday,
                                        (ML10N::MLocale::DateSymbolContext)context,
                                        (ML10N::MLocale::DateSymbolLength)symbolLength );
}
#endif

QString MLocale::languageEndonym() const
{
    return d_ptr->pLocale->languageEndonym();
}

QString MLocale::countryEndonym() const
{
    return d_ptr->pLocale->countryEndonym();
}

QString MLocale::decimalPoint() const
{
    return d_ptr->pLocale->decimalPoint();
}

#ifdef HAVE_ICU
QStringList MLocale::exemplarCharactersIndex() const
{
    return d_ptr->pLocale->exemplarCharactersIndex();
}

QString MLocale::indexBucket(const QString &str, const QStringList &buckets, const MCollator &coll) const
{
    return d_ptr->pLocale->indexBucket( str, buckets, *coll.d_ptr->pCollator );
}

QString MLocale::indexBucket(const QString &str) const
{
    return d_ptr->pLocale->indexBucket( str );
}

QString MLocale::joinStringList(const QStringList &texts) const
{
    return d_ptr->pLocale->joinStringList( texts );
}

QStringList MLocale::localeScripts() const
{
    return d_ptr->pLocale->localeScripts();
}
#endif

void MLocale::copyCatalogsFrom(const MLocale &other)
{
    d_ptr->pLocale->copyCatalogsFrom( *other.d_ptr->pLocale );
}

void MLocale::installTrCatalog(const QString &name)
{
    d_ptr->pLocale->installTrCatalog( name );
}

void MLocale::removeTrCatalog(const QString &name)
{
    d_ptr->pLocale->removeTrCatalog( name );
}

bool MLocale::isInstalledTrCatalog(const QString &name) const
{
    return d_ptr->pLocale->isInstalledTrCatalog( name );
}

QString MLocale::translate(const char *context, const char *sourceText,
                             const char *comment, int n)
{
    return d_ptr->pLocale->translate( context, sourceText, comment, n );
}

void MLocale::setDataPaths(const QStringList &dataPaths)
{
    ML10N::MLocale::setDataPaths( dataPaths );
}

void MLocale::setDataPath(const QString &dataPath)
{
    ML10N::MLocale::setDataPath( dataPath );
}

QStringList MLocale::dataPaths()
{
    return ML10N::MLocale::dataPaths();
}

QString MLocale::toLocalizedNumbers(const QString &text) const
{
    return d_ptr->pLocale->toLocalizedNumbers( text );
}

QString MLocale::toLocalizedNumbers(const QString &text, const QString &targetDigits)
{
    return ML10N::MLocale::toLocalizedNumbers( text, targetDigits );
}

QString MLocale::toLatinNumbers(const QString &text)
{
    return ML10N::MLocale::toLatinNumbers( text );
}

#ifdef HAVE_ICU
QString MLocale::localeScript(const QString &locale)
{
    return ML10N::MLocale::localeScript( locale );
}

QString MLocale::languageEndonym(const QString &locale)
{
    return ML10N::MLocale::languageEndonym( locale );
}
#endif

///////
// the static convenience methods for translation

void MLocale::setTranslationPaths(const QStringList &paths)
{
    ML10N::MLocale::setTranslationPaths( paths );
}

void MLocale::addTranslationPath(const QString &path)
{
    ML10N::MLocale::addTranslationPath( path );
}

void MLocale::removeTranslationPath(const QString &path)
{
    ML10N::MLocale::removeTranslationPath( path );
}

QStringList MLocale::translationPaths()
{
    return ML10N::MLocale::translationPaths();
}

Qt::LayoutDirection MLocale::defaultLayoutDirection()
{
    return ML10N::MLocale::defaultLayoutDirection();
}

Qt::LayoutDirection MLocale::textDirection() const
{
    return d_ptr->pLocale->textDirection();
}

Qt::LayoutDirection MLocale::directionForText(const QString & text)
{
    return ML10N::MLocale::directionForText( text );
}

void MLocale::refreshSettings()
{
    // no need to do anything here, only kept for binary compatibility...
}

QString MLocale::formatPhoneNumber( const QString& phoneNumber,
                                    PhoneNumberGrouping grouping ) const
{
    return d_ptr->pLocale->formatPhoneNumber( phoneNumber,
                                              (ML10N::MLocale::PhoneNumberGrouping)grouping );
}
