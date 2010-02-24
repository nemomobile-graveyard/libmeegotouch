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

#ifndef DUISTYLE_H
#define DUISTYLE_H

#include <QObject>
#include "duiexport.h"

// style macro
#define DUI_STYLE(CLASS) \
    public: \
    CLASS(); \
    protected: \
    virtual ~CLASS(); \
    CLASS(class CLASS##Data* data); \
    class CLASS##Data *data; \
    private:

// style macro for internal styles
#define DUI_STYLE_INTERNAL(CLASS) \
    public: \
    CLASS(); \
    virtual ~CLASS(); \
    protected: \
    CLASS(class CLASS##Data* data); \
    private:

// style attribute macro
#define DUI_STYLE_ATTRIBUTE(TYPE, NAME, CAMELNAME) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set##CAMELNAME) \
    public: \
    const TYPE& NAME() const; \
    void set##CAMELNAME(const TYPE& NAME); \
    private:

// style attribute macro for pointers
#define DUI_STYLE_PTR_ATTRIBUTE(TYPE, NAME, CAMELNAME) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set##CAMELNAME) \
    public: \
    const TYPE NAME() const; \
    void set##CAMELNAME(const TYPE NAME); \
    private:

// style container macro
// TODO: remove reloadStyles() once we are allowed again to break API
// should be a non-virtual method of DuiStyleContainer
#define DUI_STYLE_CONTAINER(STYLE_CLASS) \
    public: \
    STYLE_CLASS##Container(); \
    virtual ~STYLE_CLASS##Container(); \
    const STYLE_CLASS* operator->() const; \
    protected: \
    STYLE_CLASS##Container(class STYLE_CLASS##ContainerPrivate* dd); \
    virtual void reloadStyles(); \
    virtual const char* styleType() const; \
    class STYLE_CLASS##ContainerPrivate * const d_ptr; \
    private: \
    Q_DECLARE_PRIVATE(STYLE_CLASS##Container)

// style container macro for internal styles
// TODO: remove reloadStyles() once we are allowed again to break API
// should be a non-virtual method of DuiStyleContainer
#define DUI_STYLE_CONTAINER_INTERNAL(STYLE_CLASS) \
    public: \
    STYLE_CLASS##Container(); \
    virtual ~STYLE_CLASS##Container(); \
    const STYLE_CLASS* operator->() const; \
    protected: \
    STYLE_CLASS##Container(class STYLE_CLASS##ContainerPrivate* dd); \
    virtual void reloadStyles(); \
    virtual const char* styleType() const; \
    private: \
    Q_DECLARE_PRIVATE(STYLE_CLASS##Container)

// style mode macro
#define DUI_STYLE_MODE(MODE) \
    public: \
    void setMode##MODE();

class DuiWidgetController;

class DUI_EXPORT DuiStyle : public QObject
{
    Q_OBJECT
    DUI_STYLE(DuiStyle)

protected:
    int references() const;
    int addReference();
    int removeReference();

    friend class DuiStyleSheet;
};

class DUI_EXPORT DuiStyleContainer
{
    DUI_STYLE_CONTAINER(DuiStyle)
    DUI_STYLE_MODE(Default)
public:
    void initialize(const QString &objectName, const QString &type, const DuiWidgetController *parent);

    void setObjectName(const QString &objectName);
    void setType(const QString &type);
    QString objectName() const;
    QString type() const;
    void setParent(const DuiWidgetController *parent);
    const DuiWidgetController *parent() const;
protected:
    // TODO: remove this method once we are allowed to break API
    void setCurrentStyle(const DuiStyle*& landscapeStyle, const DuiStyle*& portraitStyle);
    const DuiStyle *currentStyle() const;

    void setCurrentMode(const QString &mode);
    // TODO: remove this method once we are allowed to break ABI
    QString currentMode();
    QString currentMode() const;

    friend class DuiTheme;
};

#endif
