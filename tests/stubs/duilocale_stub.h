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

#ifndef DUILOCALE_STUB
#define DUILOCALE_STUB

#include "duilocale.h"
#include "duicollator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiLocaleStub : public StubBase
{
public:
    virtual DuiLocale *createSystemDuiLocale();
    virtual DuiLocale createCLocale();
    virtual void DuiLocaleConstructor(QObject *parent);
    virtual void DuiLocaleConstructor(const QString &localeName, QObject *parent);
    virtual void DuiLocaleConstructor(const DuiLocale &other, QObject *parent);
    virtual void DuiLocaleDestructor();
    virtual DuiLocale operatorEquals(const DuiLocale &other);
    virtual void setDefault(const DuiLocale &locale);
    virtual bool isValid() const;
    virtual void setCategoryLocale(DuiLocale::Category category, const QString &localeName);
    virtual void setCollation(DuiLocale::Collation collation);
    virtual DuiLocale::Collation collation() const;
    virtual void setCalendar(DuiLocale::Calendar calendar);
    virtual DuiLocale::Calendar calendar() const;
    virtual DuiCollator collator() const;
    virtual QString languageEndonym() const;
    virtual QString countryEndonym() const;
    virtual QString language() const;
    virtual QString country() const;
    virtual QString script() const;
    virtual QString variant() const;
    virtual QString name() const;
    virtual Qt::LayoutDirection textDirection() const;
    virtual QString categoryLanguage(DuiLocale::Category category) const;
    virtual QString categoryCountry(DuiLocale::Category category) const;
    virtual QString categoryVariant(DuiLocale::Category category) const;
    virtual QString categoryName(DuiLocale::Category category) const;
    virtual QString formatNumber(qlonglong i) const;
    virtual QString formatNumber(short i) const;
    virtual QString formatNumber(int i) const;
    virtual QString formatNumber(double i, int precision) const;
    virtual QString formatNumber(float i) const;
    virtual QString formatPercent(double i, int decimals) const;
    virtual QString formatCurrency(double amount, const QString &currency) const;
    virtual QString formatDateTime(const QDateTime &dateTime, DuiLocale::DateType dateType, DuiLocale::TimeType timeType, DuiLocale::Calendar calendarType) const;
    virtual QString formatDateTime(const QDateTime &dateTime, DuiLocale::Calendar calendarType) const;
    virtual QString formatDateTime(const DuiCalendar &duiCalendar, DuiLocale::DateType datetype, DuiLocale::TimeType timetype) const;
    virtual QString formatDateTimeICU(const QDateTime &dateTime, const QString &formatString) const;
    virtual QString formatDateTimeICU(const DuiCalendar &duiCalendar, const QString &formatString) const;
    virtual QString formatDateTime(const QDateTime &dateTime, const QString &formatString) const;
    virtual QString formatDateTime(const DuiCalendar &duiCalendar, const QString &formatString) const;
    virtual QString monthName(const DuiCalendar &duiCalendar, int monthNumber) const;
    virtual QString monthName(const DuiCalendar &duiCalendar, int monthNumber, DuiLocale::DateSymbolContext context, DuiLocale::DateSymbolLength symbolLength) const;
    virtual QString weekdayName(const DuiCalendar &duiCalendar, int weekday) const;
    virtual QString weekdayName(const DuiCalendar &duiCalendar, int weekday, DuiLocale::DateSymbolContext context, DuiLocale::DateSymbolLength symbolLength) const;
    virtual void installCategoryCatalog(DuiLocale::Category category, const QString &name);
    virtual void removeCategoryCatalog(DuiLocale::Category category, const QString &name);
    virtual void copyCatalogsFrom(const DuiLocale &other);
    virtual bool hasCategoryCatalog(DuiLocale::Category category, const QString &name);
    virtual void installTrCatalog(const QString &name);
    virtual QString translate(const char *context, const char *sourceText, const char *comment, int n);
    virtual void setDataPaths(const QStringList &dataPaths);
    virtual void setDataPath(const QString &dataPath);
    virtual void setTranslationPaths(const QStringList &paths);
    virtual void addTranslationPath(const QString &path);
    virtual void removeTranslationPath(const QString &path);
    virtual QStringList translationPaths();
    virtual void connectSettings();
    virtual void disconnectSettings();
    virtual void insertTrToQCoreApp();
    virtual void removeTrFromQCoreApp();
    virtual DuiLocale getDefault();
    virtual bool operator==(const DuiLocale &other) const;
    virtual bool operator!=(const DuiLocale &other) const;
    virtual void refreshSettings();
};

// 2. IMPLEMENT STUB
DuiLocale *DuiLocaleStub::createSystemDuiLocale()
{
    stubMethodEntered("createSystemDuiLocale");
    return stubReturnValue<DuiLocale *>("createSystemDuiLocale");
}

DuiLocale DuiLocaleStub::createCLocale()
{
    stubMethodEntered("createCLocale");
    return stubReturnValue<DuiLocale>("createCLocale");
}

void DuiLocaleStub::DuiLocaleConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiLocaleStub::DuiLocaleConstructor(const QString &localeName, QObject *parent)
{
    Q_UNUSED(localeName);
    Q_UNUSED(parent);

}
void DuiLocaleStub::DuiLocaleConstructor(const DuiLocale &other, QObject *parent)
{
    Q_UNUSED(other);
    Q_UNUSED(parent);

}
void DuiLocaleStub::DuiLocaleDestructor()
{

}
DuiLocale DuiLocaleStub::operatorEquals(const DuiLocale &other)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiLocale & >(other));
    stubMethodEntered("operator=", params);
    return stubReturnValue<DuiLocale >("operator=");
}

void DuiLocaleStub::setDefault(const DuiLocale &locale)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiLocale & >(locale));
    stubMethodEntered("setDefault", params);
}

bool DuiLocaleStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

void DuiLocaleStub::setCategoryLocale(DuiLocale::Category category, const QString &localeName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    params.append(new Parameter<const QString & >(localeName));
    stubMethodEntered("setCategoryLocale", params);
}

void DuiLocaleStub::setCollation(DuiLocale::Collation collation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Collation >(collation));
    stubMethodEntered("setCollation", params);
}

DuiLocale::Collation DuiLocaleStub::collation() const
{
    stubMethodEntered("collation");
    return stubReturnValue<DuiLocale::Collation>("collation");
}

void DuiLocaleStub::setCalendar(DuiLocale::Calendar calendar)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Calendar >(calendar));
    stubMethodEntered("setCalendar", params);
}

DuiLocale::Calendar DuiLocaleStub::calendar() const
{
    stubMethodEntered("calendar");
    return stubReturnValue<DuiLocale::Calendar>("calendar");
}

DuiCollator DuiLocaleStub::collator() const
{
    stubMethodEntered("collator");
    return stubReturnValue<DuiCollator>("collator");
}

QString DuiLocaleStub::languageEndonym() const
{
    stubMethodEntered("languageEndonym");
    return stubReturnValue<QString>("languageEndonym");
}

QString DuiLocaleStub::countryEndonym() const
{
    stubMethodEntered("countryEndonym");
    return stubReturnValue<QString>("countryEndonym");
}

QString DuiLocaleStub::language() const
{
    stubMethodEntered("language");
    return stubReturnValue<QString>("language");
}

QString DuiLocaleStub::country() const
{
    stubMethodEntered("country");
    return stubReturnValue<QString>("country");
}

QString DuiLocaleStub::script() const
{
    stubMethodEntered("script");
    return stubReturnValue<QString>("script");
}

QString DuiLocaleStub::variant() const
{
    stubMethodEntered("variant");
    return stubReturnValue<QString>("variant");
}

QString DuiLocaleStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

Qt::LayoutDirection DuiLocaleStub::textDirection() const
{
    stubMethodEntered("textDirection");
    return stubReturnValue<Qt::LayoutDirection>("textDirection");
}

QString DuiLocaleStub::categoryLanguage(DuiLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    stubMethodEntered("categoryLanguage", params);
    return stubReturnValue<QString>("categoryLanguage");
}

QString DuiLocaleStub::categoryCountry(DuiLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    stubMethodEntered("categoryCountry", params);
    return stubReturnValue<QString>("categoryCountry");
}

QString DuiLocaleStub::categoryVariant(DuiLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    stubMethodEntered("categoryVariant", params);
    return stubReturnValue<QString>("categoryVariant");
}

QString DuiLocaleStub::categoryName(DuiLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    stubMethodEntered("categoryName", params);
    return stubReturnValue<QString>("categoryName");
}

QString DuiLocaleStub::formatNumber(qlonglong i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qlonglong >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString DuiLocaleStub::formatNumber(short i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<short >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString DuiLocaleStub::formatNumber(int i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString DuiLocaleStub::formatNumber(double i, int precision) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<double >(i));
    params.append(new Parameter<int >(precision));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString DuiLocaleStub::formatNumber(float i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString DuiLocaleStub::formatPercent(double i, int decimals) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<double >(i));
    params.append(new Parameter<int >(decimals));
    stubMethodEntered("formatPercent", params);
    return stubReturnValue<QString>("formatPercent");
}

QString DuiLocaleStub::formatCurrency(double amount, const QString &currency) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<double >(amount));
    params.append(new Parameter<const QString & >(currency));
    stubMethodEntered("formatCurrency", params);
    return stubReturnValue<QString>("formatCurrency");
}

QString DuiLocaleStub::formatDateTime(const QDateTime &dateTime, DuiLocale::DateType dateType, DuiLocale::TimeType timeType, DuiLocale::Calendar calendarType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<DuiLocale::DateType >(dateType));
    params.append(new Parameter<DuiLocale::TimeType >(timeType));
    params.append(new Parameter<DuiLocale::Calendar >(calendarType));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString DuiLocaleStub::formatDateTime(const QDateTime &dateTime, DuiLocale::Calendar calendarType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<DuiLocale::Calendar >(calendarType));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString DuiLocaleStub::formatDateTime(const DuiCalendar &duiCalendar, DuiLocale::DateType datetype, DuiLocale::TimeType timetype) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<DuiLocale::DateType >(datetype));
    params.append(new Parameter<DuiLocale::TimeType >(timetype));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString DuiLocaleStub::formatDateTimeICU(const QDateTime &dateTime, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTimeICU", params);
    return stubReturnValue<QString>("formatDateTimeICU");
}

QString DuiLocaleStub::formatDateTimeICU(const DuiCalendar &duiCalendar, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTimeICU", params);
    return stubReturnValue<QString>("formatDateTimeICU");
}

QString DuiLocaleStub::formatDateTime(const QDateTime &dateTime, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString DuiLocaleStub::formatDateTime(const DuiCalendar &duiCalendar, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString DuiLocaleStub::monthName(const DuiCalendar &duiCalendar, int monthNumber) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<int >(monthNumber));
    stubMethodEntered("monthName", params);
    return stubReturnValue<QString>("monthName");
}

QString DuiLocaleStub::monthName(const DuiCalendar &duiCalendar, int monthNumber, DuiLocale::DateSymbolContext context, DuiLocale::DateSymbolLength symbolLength) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<int >(monthNumber));
    params.append(new Parameter<DuiLocale::DateSymbolContext >(context));
    params.append(new Parameter<DuiLocale::DateSymbolLength >(symbolLength));
    stubMethodEntered("monthName", params);
    return stubReturnValue<QString>("monthName");
}

QString DuiLocaleStub::weekdayName(const DuiCalendar &duiCalendar, int weekday) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<int >(weekday));
    stubMethodEntered("weekdayName", params);
    return stubReturnValue<QString>("weekdayName");
}

QString DuiLocaleStub::weekdayName(const DuiCalendar &duiCalendar, int weekday, DuiLocale::DateSymbolContext context, DuiLocale::DateSymbolLength symbolLength) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiCalendar & >(duiCalendar));
    params.append(new Parameter<int >(weekday));
    params.append(new Parameter<DuiLocale::DateSymbolContext >(context));
    params.append(new Parameter<DuiLocale::DateSymbolLength >(symbolLength));
    stubMethodEntered("weekdayName", params);
    return stubReturnValue<QString>("weekdayName");
}

void DuiLocaleStub::installCategoryCatalog(DuiLocale::Category category, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("installCategoryCatalog", params);
}

void DuiLocaleStub::removeCategoryCatalog(DuiLocale::Category category, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("removeCategoryCatalog", params);
}

void DuiLocaleStub::copyCatalogsFrom(const DuiLocale &other)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiLocale & >(other));
    stubMethodEntered("copyCatalogsFrom", params);
}

bool DuiLocaleStub::hasCategoryCatalog(DuiLocale::Category category, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiLocale::Category >(category));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("hasCategoryCatalog", params);
    return stubReturnValue<bool>("hasCategoryCatalog");
}

void DuiLocaleStub::installTrCatalog(const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString >(name));
    stubMethodEntered("installTrCatalog", params);
}

QString DuiLocaleStub::translate(const char *context, const char *sourceText, const char *comment, int n)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const char * >(context));
    params.append(new Parameter<const char * >(sourceText));
    params.append(new Parameter<const char * >(comment));
    params.append(new Parameter<int >(n));
    stubMethodEntered("translate", params);
    return stubReturnValue<QString>("translate");
}

void DuiLocaleStub::setDataPaths(const QStringList &dataPaths)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QStringList & >(dataPaths));
    stubMethodEntered("setDataPaths", params);
}

void DuiLocaleStub::setDataPath(const QString &dataPath)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(dataPath));
    stubMethodEntered("setDataPath", params);
}

void DuiLocaleStub::setTranslationPaths(const QStringList &paths)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QStringList & >(paths));
    stubMethodEntered("setTranslationPaths", params);
}

void DuiLocaleStub::addTranslationPath(const QString &path)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(path));
    stubMethodEntered("addTranslationPath", params);
}

void DuiLocaleStub::removeTranslationPath(const QString &path)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(path));
    stubMethodEntered("removeTranslationPath", params);
}

QStringList DuiLocaleStub::translationPaths()
{
    stubMethodEntered("translationPaths");
    return stubReturnValue<QStringList>("translationPaths");
}

void DuiLocaleStub::connectSettings()
{
    stubMethodEntered("connectSettings");
}

void DuiLocaleStub::disconnectSettings()
{
    stubMethodEntered("disconnectSettings");
}

void DuiLocaleStub::insertTrToQCoreApp()
{
    stubMethodEntered("insertTrToQCoreApp");
}

void DuiLocaleStub::removeTrFromQCoreApp()
{
    stubMethodEntered("removeTrFromQCoreApp");
}

DuiLocale DuiLocaleStub::getDefault()
{
    stubMethodEntered("getDefault");
    return stubReturnValue<DuiLocale>("getDefault");
}

bool DuiLocaleStub::operator==(const DuiLocale &other) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiLocale & >(other));
    stubMethodEntered("operator==", params);
    return stubReturnValue<bool>("operator==");
}

bool DuiLocaleStub::operator!=(const DuiLocale &other) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiLocale & >(other));
    stubMethodEntered("operator!=", params);
    return stubReturnValue<bool>("operator!=");
}

void DuiLocaleStub::refreshSettings()
{
    stubMethodEntered("refreshSettings");
}



// 3. CREATE A STUB INSTANCE
DuiLocaleStub gDefaultDuiLocaleStub;
DuiLocaleStub *gDuiLocaleStub = &gDefaultDuiLocaleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiLocale *DuiLocale::createSystemDuiLocale()
{
    return gDuiLocaleStub->createSystemDuiLocale();
}

DuiLocale DuiLocale::createCLocale()
{
    return gDuiLocaleStub->createCLocale();
}

DuiLocale::DuiLocale(QObject *parent) : QObject(parent), d_ptr(NULL)
{
    gDuiLocaleStub->DuiLocaleConstructor(parent);
}

DuiLocale::DuiLocale(const QString &localeName, QObject *parent) : QObject(parent), d_ptr(NULL)
{
    gDuiLocaleStub->DuiLocaleConstructor(localeName, parent);
}

DuiLocale::DuiLocale(const DuiLocale &other, QObject *parent) : QObject(parent), d_ptr(NULL)
{
    gDuiLocaleStub->DuiLocaleConstructor(other, parent);
}

DuiLocale::~DuiLocale()
{
    gDuiLocaleStub->DuiLocaleDestructor();
}

DuiLocale &DuiLocale::operator=(const DuiLocale &other)
{
    static DuiLocale equals = gDuiLocaleStub->operatorEquals(other);
    return equals;
}

void DuiLocale::setDefault(const DuiLocale &locale)
{
    gDuiLocaleStub->setDefault(locale);
}

bool DuiLocale::isValid() const
{
    return gDuiLocaleStub->isValid();
}

void DuiLocale::setCategoryLocale(DuiLocale::Category category, const QString &localeName)
{
    gDuiLocaleStub->setCategoryLocale(category, localeName);
}

void DuiLocale::setCollation(DuiLocale::Collation collation)
{
    gDuiLocaleStub->setCollation(collation);
}

DuiLocale::Collation DuiLocale::collation() const
{
    return gDuiLocaleStub->collation();
}

void DuiLocale::setCalendar(DuiLocale::Calendar calendar)
{
    gDuiLocaleStub->setCalendar(calendar);
}

DuiLocale::Calendar DuiLocale::calendar() const
{
    return gDuiLocaleStub->calendar();
}

DuiCollator DuiLocale::collator() const
{
    return gDuiLocaleStub->collator();
}

QString DuiLocale::languageEndonym() const
{
    return gDuiLocaleStub->languageEndonym();
}

QString DuiLocale::countryEndonym() const
{
    return gDuiLocaleStub->countryEndonym();
}

QString DuiLocale::language() const
{
    return gDuiLocaleStub->language();
}

QString DuiLocale::country() const
{
    return gDuiLocaleStub->country();
}

QString DuiLocale::script() const
{
    return gDuiLocaleStub->script();
}

QString DuiLocale::variant() const
{
    return gDuiLocaleStub->variant();
}

QString DuiLocale::name() const
{
    return gDuiLocaleStub->name();
}

Qt::LayoutDirection DuiLocale::textDirection() const
{
    return gDuiLocaleStub->textDirection();
}

QString DuiLocale::categoryLanguage(DuiLocale::Category category) const
{
    return gDuiLocaleStub->categoryLanguage(category);
}

QString DuiLocale::categoryCountry(DuiLocale::Category category) const
{
    return gDuiLocaleStub->categoryCountry(category);
}

QString DuiLocale::categoryVariant(DuiLocale::Category category) const
{
    return gDuiLocaleStub->categoryVariant(category);
}

QString DuiLocale::categoryName(DuiLocale::Category category) const
{
    return gDuiLocaleStub->categoryName(category);
}

QString DuiLocale::formatNumber(qlonglong i) const
{
    return gDuiLocaleStub->formatNumber(i);
}

QString DuiLocale::formatNumber(short i) const
{
    return gDuiLocaleStub->formatNumber(i);
}

QString DuiLocale::formatNumber(int i) const
{
    return gDuiLocaleStub->formatNumber(i);
}

QString DuiLocale::formatNumber(double i, int precision) const
{
    return gDuiLocaleStub->formatNumber(i, precision);
}

QString DuiLocale::formatNumber(float i) const
{
    return gDuiLocaleStub->formatNumber(i);
}

QString DuiLocale::formatPercent(double i, int decimals) const
{
    return gDuiLocaleStub->formatPercent(i, decimals);
}

QString DuiLocale::formatCurrency(double amount, const QString &currency) const
{
    return gDuiLocaleStub->formatCurrency(amount, currency);
}

QString DuiLocale::formatDateTime(const QDateTime &dateTime, DuiLocale::DateType dateType, DuiLocale::TimeType timeType, DuiLocale::Calendar calendarType) const
{
    return gDuiLocaleStub->formatDateTime(dateTime, dateType, timeType, calendarType);
}

QString DuiLocale::formatDateTime(const QDateTime &dateTime, DuiLocale::Calendar calendarType) const
{
    return gDuiLocaleStub->formatDateTime(dateTime, calendarType);
}

QString DuiLocale::formatDateTime(const DuiCalendar &duiCalendar, DuiLocale::DateType datetype, DuiLocale::TimeType timetype) const
{
    return gDuiLocaleStub->formatDateTime(duiCalendar, datetype, timetype);
}

QString DuiLocale::formatDateTimeICU(const QDateTime &dateTime, const QString &formatString) const
{
    return gDuiLocaleStub->formatDateTimeICU(dateTime, formatString);
}

QString DuiLocale::formatDateTimeICU(const DuiCalendar &duiCalendar, const QString &formatString) const
{
    return gDuiLocaleStub->formatDateTimeICU(duiCalendar, formatString);
}

QString DuiLocale::formatDateTime(const QDateTime &dateTime, const QString &formatString) const
{
    return gDuiLocaleStub->formatDateTime(dateTime, formatString);
}

QString DuiLocale::formatDateTime(const DuiCalendar &duiCalendar, const QString &formatString) const
{
    return gDuiLocaleStub->formatDateTime(duiCalendar, formatString);
}

QString DuiLocale::monthName(const DuiCalendar &duiCalendar, int monthNumber) const
{
    return gDuiLocaleStub->monthName(duiCalendar, monthNumber);
}

QString DuiLocale::monthName(const DuiCalendar &duiCalendar, int monthNumber, DuiLocale::DateSymbolContext context, DuiLocale::DateSymbolLength symbolLength) const
{
    return gDuiLocaleStub->monthName(duiCalendar, monthNumber, context, symbolLength);
}

QString DuiLocale::weekdayName(const DuiCalendar &duiCalendar, int weekday) const
{
    return gDuiLocaleStub->weekdayName(duiCalendar, weekday);
}

QString DuiLocale::weekdayName(const DuiCalendar &duiCalendar, int weekday, DuiLocale::DateSymbolContext context, DuiLocale::DateSymbolLength symbolLength) const
{
    return gDuiLocaleStub->weekdayName(duiCalendar, weekday, context, symbolLength);
}

void DuiLocale::installCategoryCatalog(DuiLocale::Category category, const QString &name)
{
    gDuiLocaleStub->installCategoryCatalog(category, name);
}

void DuiLocale::removeCategoryCatalog(DuiLocale::Category category, const QString &name)
{
    gDuiLocaleStub->removeCategoryCatalog(category, name);
}

void DuiLocale::copyCatalogsFrom(const DuiLocale &other)
{
    gDuiLocaleStub->copyCatalogsFrom(other);
}

bool DuiLocale::hasCategoryCatalog(DuiLocale::Category category, const QString &name)
{
    return gDuiLocaleStub->hasCategoryCatalog(category, name);
}

void DuiLocale::installTrCatalog(const QString &name)
{
    gDuiLocaleStub->installTrCatalog(name);
}

QString DuiLocale::translate(const char *context, const char *sourceText, const char *comment, int n)
{
    return gDuiLocaleStub->translate(context, sourceText, comment, n);
}

void DuiLocale::setDataPaths(const QStringList &dataPaths)
{
    gDuiLocaleStub->setDataPaths(dataPaths);
}

void DuiLocale::setDataPath(const QString &dataPath)
{
    gDuiLocaleStub->setDataPath(dataPath);
}

void DuiLocale::setTranslationPaths(const QStringList &paths)
{
    gDuiLocaleStub->setTranslationPaths(paths);
}

void DuiLocale::addTranslationPath(const QString &path)
{
    gDuiLocaleStub->addTranslationPath(path);
}

void DuiLocale::removeTranslationPath(const QString &path)
{
    gDuiLocaleStub->removeTranslationPath(path);
}

QStringList DuiLocale::translationPaths()
{
    return gDuiLocaleStub->translationPaths();
}

void DuiLocale::connectSettings()
{
    gDuiLocaleStub->connectSettings();
}

void DuiLocale::disconnectSettings()
{
    gDuiLocaleStub->disconnectSettings();
}

void DuiLocale::insertTrToQCoreApp()
{
    gDuiLocaleStub->insertTrToQCoreApp();
}

void DuiLocale::removeTrFromQCoreApp()
{
    gDuiLocaleStub->removeTrFromQCoreApp();
}

DuiLocale &DuiLocale::getDefault()
{
    static DuiLocale def = gDuiLocaleStub->getDefault();
    return def;
}

bool DuiLocale::operator==(const DuiLocale &other) const
{
    return gDuiLocaleStub->operator==(other);
}

bool DuiLocale::operator!=(const DuiLocale &other) const
{
    return gDuiLocaleStub->operator!=(other);
}

void DuiLocale::refreshSettings()
{
    gDuiLocaleStub->refreshSettings();
}


#endif
