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

#ifndef DUIWIDGETMODEL_H
#define DUIWIDGETMODEL_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QSizeF>
#include <QDataStream>
#include <duiexport.h>
#include <duiwidgetcontroller.h>
#include <duitheme.h>

// model macro
#define DUI_MODEL(CLASS) \
    public: \
    CLASS(); \
    virtual ~CLASS(); \
    protected: \
    CLASS(class CLASS##Data* data); \
    class CLASS##Data *data; \
    private:


// model macro for internal models
#define DUI_MODEL_INTERNAL(CLASS) \
    public: \
    CLASS(); \
    virtual ~CLASS(); \
    protected: \
    CLASS(class CLASS##Data* data); \
    private:

// model macro with private class
#define DUI_MODEL_WITH_PRIVATE(CLASS) \
    public: \
    CLASS(); \
    virtual ~CLASS(); \
    protected: \
    CLASS(class CLASS##Data* data, class CLASS##Private* dd = NULL); \
    class CLASS##Data *data; \
    class CLASS##Private* const d_ptr; \
    private: \
    Q_DECLARE_PRIVATE(CLASS)

// model macro with private class for internal models
#define DUI_MODEL_INTERNAL_WITH_PRIVATE(CLASS) \
    public: \
    CLASS(); \
    virtual ~CLASS(); \
    protected: \
    CLASS(class CLASS##Data* data, class CLASS##Private* dd = NULL); \
    private: \
    Q_DECLARE_PRIVATE(CLASS)

// model property macro
// TYPE is the type of the property, e.g. QString, QColor or bool.
// NAME is the name of the property. NAME specifies name for Q_PROPERTY and also defines the getter name.
// CAMELNAME should be the same as NAME but with capital first letter.
// GENERATEMETHODS is either true or false and it defines whether the system should generate getter and setter automatically.
// INITIALVALUE is the initial value for the property.
//
// INITIALVALUE and GENERATEMETHODS expands to nothing in header, so they can be changed without API break.
//
// DUI_MODEL_PROPERTY(QString, text, Text, true)
// DUI_MODEL_PROPERTY(bool, visible, Visible, true)
#define DUI_MODEL_PROPERTY(TYPE, NAME, CAMELNAME, GENERATEMETHODS, INITIALVALUE) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set##CAMELNAME) \
    public: \
    static const char* const CAMELNAME; \
    const TYPE& NAME() const; \
    void set##CAMELNAME(const TYPE& NAME); \
    private: \
    TYPE& _##NAME(); \
    const TYPE& _##NAME() const;

#define DUI_MODEL_PTR_PROPERTY(TYPE, NAME, CAMELNAME, GENERATEMETHODS, INITIALVALUE) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set##CAMELNAME) \
    public: \
    static const char* const CAMELNAME; \
    TYPE NAME() const; \
    void set##CAMELNAME(TYPE NAME); \
    private:

/*!
    \class DuiWidgetModel
    \brief DuiWidgetModel implements a base class for MVC

 */
class DUI_EXPORT DuiWidgetModel : public QObject
{
    Q_OBJECT
    DUI_MODEL_WITH_PRIVATE(DuiWidgetModel)
    DUI_MODEL_PROPERTY(QString, objectName, ObjectName, true, QString::null)
    DUI_MODEL_PROPERTY(DuiTheme::ViewType, viewType, ViewType, true, DuiWidgetController::defaultType)
    DUI_MODEL_PROPERTY(QPointF, position, Position, true, QPointF(0.0, 0.0))
    DUI_MODEL_PROPERTY(QSizeF, size, Size, true, QSizeF(0.0, 0.0))
    DUI_MODEL_PROPERTY(qreal, opacity, Opacity, true, 1.0)

public:

    void beginTransaction();
    void commitTransaction();

    void increaseReferenceCount();
    void decreaseReferenceCount();

Q_SIGNALS:
    void modified(const QList<const char *>& members);

protected:
    void memberModified(const char *const member);
private:
    friend DUI_EXPORT QDataStream &operator<<(QDataStream &stream, const DuiWidgetModel &model);
    friend DUI_EXPORT QDataStream &operator>>(QDataStream &stream, DuiWidgetModel &model);
};

DUI_EXPORT QDataStream &operator<<(QDataStream &stream, const DuiWidgetModel &model);
DUI_EXPORT QDataStream &operator>>(QDataStream &stream, DuiWidgetModel &model);

#endif
