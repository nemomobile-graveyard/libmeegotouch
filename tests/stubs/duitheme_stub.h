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

#ifndef DUITHEME_STUB
#define DUITHEME_STUB

//#include <duistyle.h>
#include <duitheme.h>
#include <duiwidgetview.h>
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiThemeStub : public StubBase
{
public:
    virtual void DuiThemeConstructor(const QString &applicationName, const QString &imglistFilename = QString(), DuiTheme::ThemeService themeService = DuiTheme::AnyTheme);
    virtual void DuiThemeDestructor();

    virtual DuiTheme *instance();
    /*virtual QPixmap * getPixmap(const QString &id, const QSize &size);
    virtual QPixmap * getPixmap(const QString &id, const QSizeF &size);
    virtual QPixmap * getPixmap(const QString &id, int x, int y);
    virtual QPixmap * getPixmap(const QString &id, qreal x, qreal y);*/
    virtual QPixmap *pixmap(const QString &id, const QSize &size);
    virtual QPixmap *pixmap(const QString &id, const QSizeF &size);
    //virtual QPixmap * pixmap(const QString &id, int x, int y);
    //virtual QPixmap * pixmap(const QString &id, qreal x, qreal y);
    //virtual void setApplicationName(const QString &name);
    virtual void releasePixmap(const QPixmap *pixmap);
    virtual DuiWidgetView *view(const DuiWidgetController *controller);
    virtual bool addPixmapDirectory(const QString &directoryName, const bool &recursive);
    virtual void changeTheme(const QString &theme_id);
    virtual bool loadCSS(const QString &filename, bool append = true);
};

// 2. IMPLEMENT STUB
void DuiThemeStub::DuiThemeConstructor(const QString &applicationName, const QString &imglistFilename, DuiTheme::ThemeService themeService)
{
    Q_UNUSED(applicationName)
    Q_UNUSED(imglistFilename)
    Q_UNUSED(themeService)
}
void DuiThemeStub::DuiThemeDestructor()
{

}

// 3. CREATE A STUB INSTANCE
DuiThemeStub gDefaultDuiThemeStub;
DuiThemeStub *gDuiThemeStub = &gDefaultDuiThemeStub;

static DuiTheme gTheme("STUB");

DuiTheme *DuiThemeStub::instance()
{
    stubMethodEntered("instance");
    // this is an exception to the regular pattern
    // because duitheme has a single instance
    return &gTheme;
}

/*QPixmap * DuiThemeStub::getPixmap(const QString &id, const QSize &size)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<const QSize & >(size));
    stubMethodEntered("getPixmap", params);
    return stubReturnValue<QPixmap *>("getPixmap");
}

QPixmap * DuiThemeStub::getPixmap(const QString &id, const QSizeF &size)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<const QSizeF & >(size));
    stubMethodEntered("getPixmap", params);
    return stubReturnValue<QPixmap *>("getPixmap");
}

QPixmap * DuiThemeStub::getPixmap(const QString &id, int x, int y)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<int >(x));
    params.append(new Parameter<int >(y));
    stubMethodEntered("getPixmap", params);
    return stubReturnValue<QPixmap *>("getPixmap");
}

QPixmap * DuiThemeStub::getPixmap(const QString &id, qreal x, qreal y)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<qreal >(x));
    params.append(new Parameter<qreal >(y));
    stubMethodEntered("getPixmap", params);
    return stubReturnValue<QPixmap *>("getPixmap");
}*/

QPixmap *DuiThemeStub::pixmap(const QString &id, const QSize &size)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<const QSize & >(size));
    stubMethodEntered("pixmap", params);
    return stubReturnValue<QPixmap *>("pixmap");
}

QPixmap *DuiThemeStub::pixmap(const QString &id, const QSizeF &size)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<const QSizeF & >(size));
    stubMethodEntered("pixmap", params);
    return stubReturnValue<QPixmap *>("pixmap");
}

/*QPixmap * DuiThemeStub::pixmap(const QString &id, int x, int y)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<int >(x));
    params.append(new Parameter<int >(y));
    stubMethodEntered("pixmap", params);
    return stubReturnValue<QPixmap *>("pixmap");
}

QPixmap * DuiThemeStub::pixmap(const QString &id, qreal x, qreal y)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(id));
    params.append(new Parameter<qreal >(x));
    params.append(new Parameter<qreal >(y));
    stubMethodEntered("pixmap", params);
    return stubReturnValue<QPixmap *>("pixmap");
}

void DuiThemeStub::setApplicationName(const QString &name)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("setApplicationName", params);
}*/

void DuiThemeStub::releasePixmap(const QPixmap *pixmap)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QPixmap * >(pixmap));
    stubMethodEntered("releasePixmap", params);
}

DuiWidgetView *DuiThemeStub::view(const DuiWidgetController *controller)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiWidgetController * >(controller));
    stubMethodEntered("view", params);
    return stubReturnValue<DuiWidgetView *>("view");
}

bool DuiThemeStub::addPixmapDirectory(const QString &directoryName, const bool &recursive)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(directoryName));
    params.append(new Parameter<const bool & >(recursive));
    stubMethodEntered("addPixmapDirectory", params);
    return stubReturnValue<bool>("addPixmapDirectory");
}

void DuiThemeStub::changeTheme(const QString &theme_id)
{
    Q_UNUSED(theme_id);
    stubMethodEntered("changeTheme");
}

bool DuiThemeStub::loadCSS(const QString &filename, bool append)
{
    Q_UNUSED(append);

    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(filename));
    stubMethodEntered("loadCSS", params);
    return stubReturnValue<bool>("loadCSS");
}

// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiTheme::DuiTheme(const QString &applicationName, const QString &imglistFilename, DuiTheme::ThemeService themeService) : d_ptr(0)
{
    gDuiThemeStub->DuiThemeConstructor(applicationName, imglistFilename, themeService);
}

DuiTheme::~DuiTheme()
{
    //gDuiThemeStub->DuiThemeDestructor();
}

DuiTheme *DuiTheme::instance()
{
    return gDuiThemeStub->instance();
}

/*QPixmap * DuiTheme::getPixmap(const QString &id, const QSize &size)
{
    return gDuiThemeStub->getPixmap(id, size);
}

QPixmap * DuiTheme::getPixmap(const QString &id, const QSizeF &size)
{
    return gDuiThemeStub->getPixmap(id, size);
}

QPixmap * DuiTheme::getPixmap(const QString &id, int x, int y)
{
    return gDuiThemeStub->getPixmap(id, x, y);
}

QPixmap * DuiTheme::getPixmap(const QString &id, qreal x, qreal y)
{
    return gDuiThemeStub->getPixmap(id, x, y);
}*/

const QPixmap *DuiTheme::pixmap(const QString &id, const QSize &size)
{
    return gDuiThemeStub->pixmap(id, size);
}

/*QPixmap * DuiTheme::pixmap(const QString &id, int x, int y)
{
    return gDuiThemeStub->pixmap(id, x, y);
}

QPixmap * DuiTheme::pixmap(const QString &id, qreal x, qreal y)
{
    return gDuiThemeStub->pixmap(id, x, y);
}*/


void DuiTheme::releasePixmap(const QPixmap *pixmap)
{
    gDuiThemeStub->releasePixmap(pixmap);
}

DuiWidgetView *DuiTheme::view(const DuiWidgetController *controller)
{
    return gDuiThemeStub->view(controller);
}

bool DuiTheme::addPixmapDirectory(const QString &directoryName, const bool &recursive)
{
    return gDuiThemeStub->addPixmapDirectory(directoryName, recursive);
}

void DuiTheme::changeTheme(const QString &theme_id)
{
    gDuiThemeStub->changeTheme(theme_id);
}

bool DuiTheme::loadCSS(const QString &filename, bool append)
{
    return gDuiThemeStub->loadCSS(filename, append);
}


#endif
