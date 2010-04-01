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

#include "duistylablewidget.h"
#include "duiscalableimage.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiStylableWidget)


//! \internal
/* Private class acting as a creator for stylable widgets
   subclasses. It also takes responsibility for deleting
   creators when the application is quitting.
*/
class DuiStylableWidgetCreator : DuiWidgetCreatorBase
{
public:

    static void registerCreator(const QMetaObject *metaObject,
                                const char *widgetAssemblyName,
                                Dui::AssemblyType widgetAssemblyType) {
        const DuiStylableWidgetCreator *widgetCreator = creatorInstanceHolder.value(metaObject->className(), NULL);
        if (!widgetCreator) {
            DuiStylableWidgetCreator *creator = new DuiStylableWidgetCreator(metaObject,
                    widgetAssemblyName,
                    widgetAssemblyType);
            creatorInstanceHolder.insert(metaObject->className(), creator);
        }
    }

protected:
    DuiStylableWidgetCreator(const QMetaObject *_widgetMetaObject,
                             const char *widgetAssemblyName,
                             Dui::AssemblyType widgetAssemblyType) :
        DuiWidgetCreatorBase(_widgetMetaObject->className(), widgetAssemblyName, widgetAssemblyType),
        widgetMetaObject(_widgetMetaObject)
    {}

    virtual ~DuiStylableWidgetCreator()
    {}

    virtual DuiWidgetController *create() const {
        return NULL;
    }

    virtual const QMetaObject *metaObject() const {
        return widgetMetaObject;
    }

private:
    class CreatorInstanceHolder : public QMap<QString, DuiStylableWidgetCreator *>
    {
    public:
        ~CreatorInstanceHolder() {
            foreach(DuiStylableWidgetCreator * creator, this->values())
            delete creator;
        }
    };

    static CreatorInstanceHolder creatorInstanceHolder;
    const QMetaObject *widgetMetaObject;
};
//! \internal_end

DuiStylableWidgetCreator::CreatorInstanceHolder DuiStylableWidgetCreator::creatorInstanceHolder;

DuiStylableWidget::DuiStylableWidget(QGraphicsItem *parent) : DuiWidgetController(parent)
{
}

DuiStylableWidget::~DuiStylableWidget()
{
}

void DuiStylableWidget::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    if (!style()->backgroundImage() && !style()->backgroundColor().isValid())
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    QSizeF s = size() - QSizeF(style()->marginLeft() + style()->marginRight(), style()->marginTop() + style()->marginBottom());
    if (style()->backgroundImage()) {
        style()->backgroundImage()->draw(0, 0, s.width(), s.height(), painter);
    } else { //style background color must be valid
        painter->fillRect(QRectF(QPointF(0, 0), s), QBrush(style()->backgroundColor()));
    }
    painter->setOpacity(oldOpacity);
}

void DuiStylableWidget::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void DuiStylableWidget::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void DuiStylableWidget::applyStyle()
{
}

void DuiStylableWidget::registerStylableWidgetType(const QMetaObject *metaObject,
        const char *widgetAssemblyName,
        Dui::AssemblyType widgetAssemblyType)
{
    DuiStylableWidgetCreator::registerCreator(metaObject, widgetAssemblyName, widgetAssemblyType);
}
