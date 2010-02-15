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

#ifndef DUIDESKTOPENTRY_STUB
#define DUIDESKTOPENTRY_STUB

#include "duidesktopentry.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiDesktopEntryStub : public StubBase
{
public:
    virtual void DuiDesktopEntryConstructor(const QString &fileName);
    virtual void DuiDesktopEntryDestructor();
    virtual QString fileName() const;
    virtual bool isValid() const;
    virtual uint hash() const;
    virtual QString type() const;
    virtual QString version() const;
    virtual QString name() const;
    virtual QString nameUnlocalized() const;
    virtual QString genericName() const;
    virtual bool noDisplay() const;
    virtual QString comment() const;
    virtual QString icon() const;
    virtual bool hidden() const;
    virtual QStringList onlyShowIn() const;
    virtual QStringList notShowIn() const;
    virtual QString tryExec() const;
    virtual QString exec() const;
    virtual QString path() const;
    virtual bool terminal() const;
    virtual QStringList mimeType() const;
    virtual QStringList categories() const;
    virtual bool startupNotify() const;
    virtual QString startupWMClass() const;
    virtual QString url() const;
    virtual QString xMaemoService() const;
    virtual QString value(const QString &key) const;
    virtual QString value(const QString &group, const QString &key) const;
    virtual bool contains(const QString &key) const;
    virtual bool contains(const QString &group, const QString &key) const;
    virtual void DuiDesktopEntryConstructor(DuiDesktopEntryPrivate &dd);

    void stubSetKeyValue(const QString &key, const QString &value) {
        keyValuePairs.insert(key, value);
    }
    void stubSetFilenameForDesktopEntry(DuiDesktopEntry *entry, const QString &filename) {
        filenames.insert(entry, filename);
    }
    QString stubFilenameForDesktopEntry(DuiDesktopEntry *entry) {
        return filenames.value(entry);
    }

private:
    mutable QMap<QString, QString> keyValuePairs;
    mutable QHash<DuiDesktopEntry *, QString> filenames;
};

// 2. IMPLEMENT STUB
void DuiDesktopEntryStub::DuiDesktopEntryConstructor(const QString &fileName)
{
    Q_UNUSED(fileName);

}
void DuiDesktopEntryStub::DuiDesktopEntryDestructor()
{

}
QString DuiDesktopEntryStub::fileName() const
{
    stubMethodEntered("fileName");
    return stubReturnValue<QString>("fileName");
}

bool DuiDesktopEntryStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

uint DuiDesktopEntryStub::hash() const
{
    stubMethodEntered("hash");
    return stubReturnValue<uint>("hash");
}

QString DuiDesktopEntryStub::type() const
{
    stubMethodEntered("type");
    return stubReturnValue<QString>("type");
}

QString DuiDesktopEntryStub::version() const
{
    stubMethodEntered("version");
    return stubReturnValue<QString>("version");
}

QString DuiDesktopEntryStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

QString DuiDesktopEntryStub::nameUnlocalized() const
{
    stubMethodEntered("nameUnlocalized");
    return stubReturnValue<QString>("nameUnlocalized");
}

QString DuiDesktopEntryStub::genericName() const
{
    stubMethodEntered("genericName");
    return stubReturnValue<QString>("genericName");
}

bool DuiDesktopEntryStub::noDisplay() const
{
    stubMethodEntered("noDisplay");
    return stubReturnValue<bool>("noDisplay");
}

QString DuiDesktopEntryStub::comment() const
{
    stubMethodEntered("comment");
    return stubReturnValue<QString>("comment");
}

QString DuiDesktopEntryStub::icon() const
{
    stubMethodEntered("icon");
    return stubReturnValue<QString>("icon");
}

bool DuiDesktopEntryStub::hidden() const
{
    stubMethodEntered("hidden");
    return stubReturnValue<bool>("hidden");
}

QStringList DuiDesktopEntryStub::onlyShowIn() const
{
    stubMethodEntered("onlyShowIn");
    return stubReturnValue<QStringList>("onlyShowIn");
}

QStringList DuiDesktopEntryStub::notShowIn() const
{
    stubMethodEntered("notShowIn");
    return stubReturnValue<QStringList>("notShowIn");
}

QString DuiDesktopEntryStub::tryExec() const
{
    stubMethodEntered("tryExec");
    return stubReturnValue<QString>("tryExec");
}

QString DuiDesktopEntryStub::exec() const
{
    stubMethodEntered("exec");
    return stubReturnValue<QString>("exec");
}

QString DuiDesktopEntryStub::path() const
{
    stubMethodEntered("path");
    return stubReturnValue<QString>("path");
}

bool DuiDesktopEntryStub::terminal() const
{
    stubMethodEntered("terminal");
    return stubReturnValue<bool>("terminal");
}

QStringList DuiDesktopEntryStub::mimeType() const
{
    stubMethodEntered("mimeType");
    return stubReturnValue<QStringList>("mimeType");
}

QStringList DuiDesktopEntryStub::categories() const
{
    stubMethodEntered("categories");
    return stubReturnValue<QStringList>("categories");
}

bool DuiDesktopEntryStub::startupNotify() const
{
    stubMethodEntered("startupNotify");
    return stubReturnValue<bool>("startupNotify");
}

QString DuiDesktopEntryStub::startupWMClass() const
{
    stubMethodEntered("startupWMClass");
    return stubReturnValue<QString>("startupWMClass");
}

QString DuiDesktopEntryStub::url() const
{
    stubMethodEntered("url");
    return stubReturnValue<QString>("url");
}

QString DuiDesktopEntryStub::xMaemoService() const
{
    stubMethodEntered("xMaemoService");
    return stubReturnValue<QString>("xMaemoService");
}

QString DuiDesktopEntryStub::value(const QString &key) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("value", params);
    return keyValuePairs.value(key);
}

QString DuiDesktopEntryStub::value(const QString &group, const QString &key) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(group));
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("value", params);
    return keyValuePairs.value(group + '/' + key);
}

bool DuiDesktopEntryStub::contains(const QString &key) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("contains", params);
    return keyValuePairs.contains(key);
}

bool DuiDesktopEntryStub::contains(const QString &group, const QString &key) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(group));
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("contains", params);
    return keyValuePairs.contains(group + '/' + key);
}

void DuiDesktopEntryStub::DuiDesktopEntryConstructor(DuiDesktopEntryPrivate &dd)
{
    Q_UNUSED(dd);

}


// 3. CREATE A STUB INSTANCE
DuiDesktopEntryStub gDefaultDuiDesktopEntryStub;
DuiDesktopEntryStub *gDuiDesktopEntryStub = &gDefaultDuiDesktopEntryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiDesktopEntry::DuiDesktopEntry(const QString &fileName) : d_ptr(NULL)
{
    gDuiDesktopEntryStub->DuiDesktopEntryConstructor(fileName);
    gDuiDesktopEntryStub->stubSetFilenameForDesktopEntry(this, fileName);
}

DuiDesktopEntry::~DuiDesktopEntry()
{
    gDuiDesktopEntryStub->DuiDesktopEntryDestructor();
}

QString DuiDesktopEntry::fileName() const
{
    return gDuiDesktopEntryStub->fileName().isEmpty() ? gDuiDesktopEntryStub->stubFilenameForDesktopEntry(const_cast<DuiDesktopEntry *>(this)) : gDuiDesktopEntryStub->fileName();
}

bool DuiDesktopEntry::isValid() const
{
    return gDuiDesktopEntryStub->isValid();
}

uint DuiDesktopEntry::hash() const
{
    return gDuiDesktopEntryStub->hash();
}

QString DuiDesktopEntry::type() const
{
    return gDuiDesktopEntryStub->type();
}

QString DuiDesktopEntry::version() const
{
    return gDuiDesktopEntryStub->version();
}

QString DuiDesktopEntry::name() const
{
    return gDuiDesktopEntryStub->name();
}

QString DuiDesktopEntry::nameUnlocalized() const
{
    return gDuiDesktopEntryStub->nameUnlocalized();
}

QString DuiDesktopEntry::genericName() const
{
    return gDuiDesktopEntryStub->genericName();
}

bool DuiDesktopEntry::noDisplay() const
{
    return gDuiDesktopEntryStub->noDisplay();
}

QString DuiDesktopEntry::comment() const
{
    return gDuiDesktopEntryStub->comment();
}

QString DuiDesktopEntry::icon() const
{
    return gDuiDesktopEntryStub->icon();
}

bool DuiDesktopEntry::hidden() const
{
    return gDuiDesktopEntryStub->hidden();
}

QStringList DuiDesktopEntry::onlyShowIn() const
{
    return gDuiDesktopEntryStub->onlyShowIn();
}

QStringList DuiDesktopEntry::notShowIn() const
{
    return gDuiDesktopEntryStub->notShowIn();
}

QString DuiDesktopEntry::tryExec() const
{
    return gDuiDesktopEntryStub->tryExec();
}

QString DuiDesktopEntry::exec() const
{
    return gDuiDesktopEntryStub->exec();
}

QString DuiDesktopEntry::path() const
{
    return gDuiDesktopEntryStub->path();
}

bool DuiDesktopEntry::terminal() const
{
    return gDuiDesktopEntryStub->terminal();
}

QStringList DuiDesktopEntry::mimeType() const
{
    return gDuiDesktopEntryStub->mimeType();
}

QStringList DuiDesktopEntry::categories() const
{
    return gDuiDesktopEntryStub->categories();
}

bool DuiDesktopEntry::startupNotify() const
{
    return gDuiDesktopEntryStub->startupNotify();
}

QString DuiDesktopEntry::startupWMClass() const
{
    return gDuiDesktopEntryStub->startupWMClass();
}

QString DuiDesktopEntry::url() const
{
    return gDuiDesktopEntryStub->url();
}

QString DuiDesktopEntry::xMaemoService() const
{
    return gDuiDesktopEntryStub->xMaemoService();
}

QString DuiDesktopEntry::value(const QString &key) const
{
    return gDuiDesktopEntryStub->value(key);
}

QString DuiDesktopEntry::value(const QString &group, const QString &key) const
{
    return gDuiDesktopEntryStub->value(group, key);
}

bool DuiDesktopEntry::contains(const QString &key) const
{
    return gDuiDesktopEntryStub->contains(key);
}

bool DuiDesktopEntry::contains(const QString &group, const QString &key) const
{
    return gDuiDesktopEntryStub->contains(group, key);
}

DuiDesktopEntry::DuiDesktopEntry(DuiDesktopEntryPrivate &dd) : d_ptr(&dd)
{
    gDuiDesktopEntryStub->DuiDesktopEntryConstructor(dd);
}


#endif
