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

#ifndef MTHEME_STUB
#define MTHEME_STUB

#include "mtheme.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MThemeStub : public StubBase {
  public:
   QString ViewType ;
  virtual void MThemeConstructor(const QString &applicationName, const QString &imglistFilename, MTheme::ThemeService themeService);
  virtual void MThemeDestructor();
  virtual const MStyle * style(const char *styleClassName, const QString &objectName, const QString &mode, const QString &type, M::Orientation orientation, const MWidgetController *parent);
  virtual const MStyle * style(const char *styleClassName, const QString &objectName);
  virtual void releaseStyle(const MStyle *style);
  virtual bool addPixmapDirectory(const QString &directoryName, M::RecursionMode mode);
  virtual void clearPixmapDirectories();
  virtual MTheme * instance();
  virtual const QPixmap * pixmap(const QString &id, const QSize &size);
  virtual QPixmap * pixmapCopy(const QString &id, const QSize &size);
  virtual const MScalableImage * scalableImage(const QString &id, int left, int right, int top, int bottom);
  virtual void releaseScalableImage(const MScalableImage *image);
  virtual void releasePixmap(const QPixmap *pixmap);
  virtual MWidgetView * view(const MWidgetController *controller);
  virtual const MPalette & palette();
  virtual const MDefaultFonts & fonts();
  virtual bool loadCSS(const QString &filename, MTheme::InsertMode mode);
  virtual QString currentTheme();
  virtual bool hasPendingRequests();
  virtual void rebuildViewsForWidgets();
}; 

// 2. IMPLEMENT STUB
void MThemeStub::MThemeConstructor(const QString &applicationName, const QString &imglistFilename, MTheme::ThemeService themeService) {
  Q_UNUSED(applicationName);
  Q_UNUSED(imglistFilename);
  Q_UNUSED(themeService);

}
void MThemeStub::MThemeDestructor() {

}
const MStyle * MThemeStub::style(const char *styleClassName, const QString &objectName, const QString &mode, const QString &type, M::Orientation orientation, const MWidgetController *parent) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const char * >(styleClassName));
  params.append( new Parameter<const QString & >(objectName));
  params.append( new Parameter<const QString & >(mode));
  params.append( new Parameter<const QString & >(type));
  params.append( new Parameter<M::Orientation >(orientation));
  params.append( new Parameter<const MWidgetController * >(parent));
  stubMethodEntered("style",params);
  return stubReturnValue<const MStyle *>("style");
}

const MStyle * MThemeStub::style(const char *styleClassName, const QString &objectName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const char * >(styleClassName));
  params.append( new Parameter<const QString & >(objectName));
  stubMethodEntered("style",params);
  return stubReturnValue<const MStyle *>("style");
}

void MThemeStub::releaseStyle(const MStyle *style) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MStyle * >(style));
  stubMethodEntered("releaseStyle",params);
}

bool MThemeStub::addPixmapDirectory(const QString &directoryName, M::RecursionMode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(directoryName));
  params.append( new Parameter<M::RecursionMode >(mode));
  stubMethodEntered("addPixmapDirectory",params);
  return stubReturnValue<bool>("addPixmapDirectory");
}

void MThemeStub::clearPixmapDirectories() {
  stubMethodEntered("clearPixmapDirectories");
}

MTheme * MThemeStub::instance() {
  stubMethodEntered("instance");
  return stubReturnValue<MTheme *>("instance");
}

const QPixmap * MThemeStub::pixmap(const QString &id, const QSize &size) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(id));
  params.append( new Parameter<const QSize & >(size));
  stubMethodEntered("pixmap",params);
  return stubReturnValue<const QPixmap *>("pixmap");
}

QPixmap * MThemeStub::pixmapCopy(const QString &id, const QSize &size) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(id));
  params.append( new Parameter<const QSize & >(size));
  stubMethodEntered("pixmapCopy",params);
  return stubReturnValue<QPixmap *>("pixmapCopy");
}

const MScalableImage * MThemeStub::scalableImage(const QString &id, int left, int right, int top, int bottom) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(id));
  params.append( new Parameter<int >(left));
  params.append( new Parameter<int >(right));
  params.append( new Parameter<int >(top));
  params.append( new Parameter<int >(bottom));
  stubMethodEntered("scalableImage",params);
  return stubReturnValue<const MScalableImage *>("scalableImage");
}

void MThemeStub::releaseScalableImage(const MScalableImage *image) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MScalableImage * >(image));
  stubMethodEntered("releaseScalableImage",params);
}

void MThemeStub::releasePixmap(const QPixmap *pixmap) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QPixmap * >(pixmap));
  stubMethodEntered("releasePixmap",params);
}

MWidgetView * MThemeStub::view(const MWidgetController *controller) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MWidgetController * >(controller));
  stubMethodEntered("view",params);
  return stubReturnValue<MWidgetView *>("view");
}

const MPalette & MThemeStub::palette() {
  stubMethodEntered("palette");
  return *stubReturnValue<const MPalette *>("palette");
}

const MDefaultFonts & MThemeStub::fonts() {
  stubMethodEntered("fonts");
  return *stubReturnValue<const MDefaultFonts *>("fonts");
}

bool MThemeStub::loadCSS(const QString &filename, MTheme::InsertMode mode) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(filename));
  params.append( new Parameter<MTheme::InsertMode >(mode));
  stubMethodEntered("loadCSS",params);
  return stubReturnValue<bool>("loadCSS");
}

QString MThemeStub::currentTheme() {
  stubMethodEntered("currentTheme");
  return stubReturnValue<QString>("currentTheme");
}

bool MThemeStub::hasPendingRequests() {
  stubMethodEntered("hasPendingRequests");
  return stubReturnValue<bool>("hasPendingRequests");
}

void MThemeStub::rebuildViewsForWidgets() {
  stubMethodEntered("rebuildViewsForWidgets");
}



// 3. CREATE A STUB INSTANCE
MThemeStub gDefaultMThemeStub;
MThemeStub* gMThemeStub = &gDefaultMThemeStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MTheme::MTheme(const QString &applicationName, const QString &imglistFilename, MTheme::ThemeService themeService) :
        d_ptr(NULL)
{
  gMThemeStub->MThemeConstructor(applicationName, imglistFilename, themeService);
}

MTheme::~MTheme() {
  gMThemeStub->MThemeDestructor();
}

const MStyle * MTheme::style(const char *styleClassName, const QString &objectName, const QString &mode, const QString &type, M::Orientation orientation, const MWidgetController *parent) {
  return gMThemeStub->style(styleClassName, objectName, mode, type, orientation, parent);
}

const MStyle * MTheme::style(const char *styleClassName, const QString &objectName) {
  return gMThemeStub->style(styleClassName, objectName);
}

void MTheme::releaseStyle(const MStyle *style) {
  gMThemeStub->releaseStyle(style);
}

bool MTheme::addPixmapDirectory(const QString &directoryName, M::RecursionMode mode) {
  return gMThemeStub->addPixmapDirectory(directoryName, mode);
}

void MTheme::clearPixmapDirectories() {
  gMThemeStub->clearPixmapDirectories();
}

MTheme * MTheme::instance() {
  return gMThemeStub->instance();
}

const QPixmap * MTheme::pixmap(const QString &id, const QSize &size) {
  return gMThemeStub->pixmap(id, size);
}

QPixmap * MTheme::pixmapCopy(const QString &id, const QSize &size) {
  return gMThemeStub->pixmapCopy(id, size);
}

const MScalableImage * MTheme::scalableImage(const QString &id, int left, int right, int top, int bottom) {
  return gMThemeStub->scalableImage(id, left, right, top, bottom);
}

void MTheme::releaseScalableImage(const MScalableImage *image) {
  gMThemeStub->releaseScalableImage(image);
}

void MTheme::releasePixmap(const QPixmap *pixmap) {
  gMThemeStub->releasePixmap(pixmap);
}

MWidgetView * MTheme::view(const MWidgetController *controller) {
  return gMThemeStub->view(controller);
}

const MPalette & MTheme::palette() {
  return gMThemeStub->palette();
}

const MDefaultFonts & MTheme::fonts() {
  return gMThemeStub->fonts();
}

bool MTheme::loadCSS(const QString &filename, InsertMode mode) {
  return gMThemeStub->loadCSS(filename, mode);
}

QString MTheme::currentTheme() {
  return gMThemeStub->currentTheme();
}

bool MTheme::hasPendingRequests() {
  return gMThemeStub->hasPendingRequests();
}

void MTheme::rebuildViewsForWidgets() {
  gMThemeStub->rebuildViewsForWidgets();
}


#endif
