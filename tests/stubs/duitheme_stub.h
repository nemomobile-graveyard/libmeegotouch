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

#include "duitheme.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiThemeStub : public StubBase {
  public:
   QString ViewType ;
  virtual void DuiThemeConstructor(const QString &applicationName, const QString &imglistFilename, DuiTheme::ThemeService themeService);
  virtual void DuiThemeDestructor();
  virtual const DuiStyle * style(const char *styleClassName, const QString &objectName, const QString &mode, const QString &type, Dui::Orientation orientation, const DuiWidgetController *parent);
  virtual const DuiStyle * style(const char *styleClassName, const QString &objectName);
  virtual void releaseStyle(const DuiStyle *style);
  virtual bool addPixmapDirectory(const QString &directoryName, Dui::RecursionMode mode);
  virtual void clearPixmapDirectories();
  virtual DuiTheme * instance();
  virtual const QPixmap * pixmap(const QString &id, const QSize &size);
  virtual QPixmap * pixmapCopy(const QString &id, const QSize &size);
  virtual const DuiScalableImage * scalableImage(const QString &id, int left, int right, int top, int bottom);
  virtual void releaseScalableImage(const DuiScalableImage *image);
  virtual void releasePixmap(const QPixmap *pixmap);
  virtual DuiWidgetView * view(const DuiWidgetController *controller);
  virtual const DuiPalette & palette();
  virtual const DuiDefaultFonts & fonts();
  virtual bool loadCSS(const QString &filename, DuiTheme::InsertMode mode);
  virtual QString currentTheme();
  virtual QStringList findAvailableThemes();
  virtual void changeTheme(const QString &theme_id);
  virtual bool hasPendingRequests();
  virtual void rebuildViewsForWidgets();
}; 

// 2. IMPLEMENT STUB
void DuiThemeStub::DuiThemeConstructor(const QString &applicationName, const QString &imglistFilename, DuiTheme::ThemeService themeService) {
  Q_UNUSED(applicationName);
  Q_UNUSED(imglistFilename);
  Q_UNUSED(themeService);

}
void DuiThemeStub::DuiThemeDestructor() {

}
const DuiStyle * DuiThemeStub::style(const char *styleClassName, const QString &objectName, const QString &mode, const QString &type, Dui::Orientation orientation, const DuiWidgetController *parent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const char * >(styleClassName));
  params.append( new Parameter<const QString & >(objectName));
  params.append( new Parameter<const QString & >(mode));
  params.append( new Parameter<const QString & >(type));
  params.append( new Parameter<Dui::Orientation >(orientation));
  params.append( new Parameter<const DuiWidgetController * >(parent));
  stubMethodEntered("style",params);
  return stubReturnValue<const DuiStyle *>("style");
}

const DuiStyle * DuiThemeStub::style(const char *styleClassName, const QString &objectName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const char * >(styleClassName));
  params.append( new Parameter<const QString & >(objectName));
  stubMethodEntered("style",params);
  return stubReturnValue<const DuiStyle *>("style");
}

void DuiThemeStub::releaseStyle(const DuiStyle *style) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiStyle * >(style));
  stubMethodEntered("releaseStyle",params);
}

bool DuiThemeStub::addPixmapDirectory(const QString &directoryName, Dui::RecursionMode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(directoryName));
  params.append( new Parameter<Dui::RecursionMode >(mode));
  stubMethodEntered("addPixmapDirectory",params);
  return stubReturnValue<bool>("addPixmapDirectory");
}

void DuiThemeStub::clearPixmapDirectories() {
  stubMethodEntered("clearPixmapDirectories");
}

DuiTheme * DuiThemeStub::instance() {
  stubMethodEntered("instance");
  return stubReturnValue<DuiTheme *>("instance");
}

const QPixmap * DuiThemeStub::pixmap(const QString &id, const QSize &size) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(id));
  params.append( new Parameter<const QSize & >(size));
  stubMethodEntered("pixmap",params);
  return stubReturnValue<const QPixmap *>("pixmap");
}

QPixmap * DuiThemeStub::pixmapCopy(const QString &id, const QSize &size) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(id));
  params.append( new Parameter<const QSize & >(size));
  stubMethodEntered("pixmapCopy",params);
  return stubReturnValue<QPixmap *>("pixmapCopy");
}

const DuiScalableImage * DuiThemeStub::scalableImage(const QString &id, int left, int right, int top, int bottom) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(id));
  params.append( new Parameter<int >(left));
  params.append( new Parameter<int >(right));
  params.append( new Parameter<int >(top));
  params.append( new Parameter<int >(bottom));
  stubMethodEntered("scalableImage",params);
  return stubReturnValue<const DuiScalableImage *>("scalableImage");
}

void DuiThemeStub::releaseScalableImage(const DuiScalableImage *image) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiScalableImage * >(image));
  stubMethodEntered("releaseScalableImage",params);
}

void DuiThemeStub::releasePixmap(const QPixmap *pixmap) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QPixmap * >(pixmap));
  stubMethodEntered("releasePixmap",params);
}

DuiWidgetView * DuiThemeStub::view(const DuiWidgetController *controller) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiWidgetController * >(controller));
  stubMethodEntered("view",params);
  return stubReturnValue<DuiWidgetView *>("view");
}

const DuiPalette & DuiThemeStub::palette() {
  stubMethodEntered("palette");
  return *stubReturnValue<const DuiPalette *>("palette");
}

const DuiDefaultFonts & DuiThemeStub::fonts() {
  stubMethodEntered("fonts");
  return *stubReturnValue<const DuiDefaultFonts *>("fonts");
}

bool DuiThemeStub::loadCSS(const QString &filename, DuiTheme::InsertMode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(filename));
  params.append( new Parameter<DuiTheme::InsertMode >(mode));
  stubMethodEntered("loadCSS",params);
  return stubReturnValue<bool>("loadCSS");
}

QString DuiThemeStub::currentTheme() {
  stubMethodEntered("currentTheme");
  return stubReturnValue<QString>("currentTheme");
}

QStringList DuiThemeStub::findAvailableThemes() {
  stubMethodEntered("findAvailableThemes");
  return stubReturnValue<QStringList>("findAvailableThemes");
}

void DuiThemeStub::changeTheme(const QString &theme_id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(theme_id));
  stubMethodEntered("changeTheme",params);
}

bool DuiThemeStub::hasPendingRequests() {
  stubMethodEntered("hasPendingRequests");
  return stubReturnValue<bool>("hasPendingRequests");
}

void DuiThemeStub::rebuildViewsForWidgets() {
  stubMethodEntered("rebuildViewsForWidgets");
}



// 3. CREATE A STUB INSTANCE
DuiThemeStub gDefaultDuiThemeStub;
DuiThemeStub* gDuiThemeStub = &gDefaultDuiThemeStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiTheme::DuiTheme(const QString &applicationName, const QString &imglistFilename, DuiTheme::ThemeService themeService) :
        d_ptr(NULL)
{
  gDuiThemeStub->DuiThemeConstructor(applicationName, imglistFilename, themeService);
}

DuiTheme::~DuiTheme() {
  gDuiThemeStub->DuiThemeDestructor();
}

const DuiStyle * DuiTheme::style(const char *styleClassName, const QString &objectName, const QString &mode, const QString &type, Dui::Orientation orientation, const DuiWidgetController *parent) {
  return gDuiThemeStub->style(styleClassName, objectName, mode, type, orientation, parent);
}

const DuiStyle * DuiTheme::style(const char *styleClassName, const QString &objectName) {
  return gDuiThemeStub->style(styleClassName, objectName);
}

void DuiTheme::releaseStyle(const DuiStyle *style) {
  gDuiThemeStub->releaseStyle(style);
}

bool DuiTheme::addPixmapDirectory(const QString &directoryName, Dui::RecursionMode mode) {
  return gDuiThemeStub->addPixmapDirectory(directoryName, mode);
}

void DuiTheme::clearPixmapDirectories() {
  gDuiThemeStub->clearPixmapDirectories();
}

DuiTheme * DuiTheme::instance() {
  return gDuiThemeStub->instance();
}

const QPixmap * DuiTheme::pixmap(const QString &id, const QSize &size) {
  return gDuiThemeStub->pixmap(id, size);
}

QPixmap * DuiTheme::pixmapCopy(const QString &id, const QSize &size) {
  return gDuiThemeStub->pixmapCopy(id, size);
}

const DuiScalableImage * DuiTheme::scalableImage(const QString &id, int left, int right, int top, int bottom) {
  return gDuiThemeStub->scalableImage(id, left, right, top, bottom);
}

void DuiTheme::releaseScalableImage(const DuiScalableImage *image) {
  gDuiThemeStub->releaseScalableImage(image);
}

void DuiTheme::releasePixmap(const QPixmap *pixmap) {
  gDuiThemeStub->releasePixmap(pixmap);
}

DuiWidgetView * DuiTheme::view(const DuiWidgetController *controller) {
  return gDuiThemeStub->view(controller);
}

const DuiPalette & DuiTheme::palette() {
  return gDuiThemeStub->palette();
}

const DuiDefaultFonts & DuiTheme::fonts() {
  return gDuiThemeStub->fonts();
}

bool DuiTheme::loadCSS(const QString &filename, InsertMode mode) {
  return gDuiThemeStub->loadCSS(filename, mode);
}

QString DuiTheme::currentTheme() {
  return gDuiThemeStub->currentTheme();
}

QStringList DuiTheme::findAvailableThemes() {
  return gDuiThemeStub->findAvailableThemes();
}

void DuiTheme::changeTheme(const QString &theme_id) {
  gDuiThemeStub->changeTheme(theme_id);
}

bool DuiTheme::hasPendingRequests() {
  return gDuiThemeStub->hasPendingRequests();
}

void DuiTheme::rebuildViewsForWidgets() {
  gDuiThemeStub->rebuildViewsForWidgets();
}


#endif
