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

#include "mstylablewidget.h"
#include "mscalableimage.h"
#include "mwidgetmodel.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MStylableWidget)


//! \internal
/* Private class acting as a creator for stylable widgets
   subclasses. It also takes responsibility for deleting
   creators when the application is quitting.
*/
class MStylableWidgetCreator : MWidgetCreatorBase
{
public:

    static void registerCreator(const QMetaObject *metaObject,
                                const char *widgetAssemblyName,
                                M::AssemblyType widgetAssemblyType) {
        const MStylableWidgetCreator *widgetCreator = creatorInstanceHolder.value(metaObject->className(), NULL);
        if (!widgetCreator) {
            MStylableWidgetCreator *creator = new MStylableWidgetCreator(metaObject,
                    widgetAssemblyName,
                    widgetAssemblyType);
            creatorInstanceHolder.insert(metaObject->className(), creator);
        }
    }

protected:
    MStylableWidgetCreator(const QMetaObject *_widgetMetaObject,
                             const char *widgetAssemblyName,
                             M::AssemblyType widgetAssemblyType) :
        MWidgetCreatorBase(_widgetMetaObject->className(), widgetAssemblyName, widgetAssemblyType),
        widgetMetaObject(_widgetMetaObject)
    {}

    virtual ~MStylableWidgetCreator()
    {}

    virtual MWidgetController *create() const {
        return NULL;
    }

    virtual const QMetaObject *metaObject() const {
        return widgetMetaObject;
    }

private:
    class CreatorInstanceHolder : public QMap<QString, MStylableWidgetCreator *>
    {
    public:
        ~CreatorInstanceHolder() {
            foreach(MStylableWidgetCreator * creator, this->values())
            delete creator;
        }
    };

    static CreatorInstanceHolder creatorInstanceHolder;
    const QMetaObject *widgetMetaObject;
};
//! \internal_end

MStylableWidgetCreator::CreatorInstanceHolder MStylableWidgetCreator::creatorInstanceHolder;

MStylableWidget::MStylableWidget(QGraphicsItem *parent) : MWidgetController(parent)
{
}

MStylableWidget::~MStylableWidget()
{
}

void MStylableWidget::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    const MWidgetStyle *s = static_cast<const MWidgetStyle*>(style().operator ->());

    if (!s->backgroundTiles().isValid() && !s->backgroundImage() && !s->backgroundColor().isValid())
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(s->backgroundOpacity() * effectiveOpacity());

    QSizeF currentSize = size() - QSizeF(s->marginLeft() + s->marginRight(), s->marginTop() + s->marginBottom());
    if (s->backgroundTiles().isValid()) {
        s->backgroundTiles()[model()->layoutPosition()]->draw(0.0, 0.0, currentSize.width(), currentSize.height(), painter);
    }else if (s->backgroundImage()) {
        s->backgroundImage()->draw(0.0, 0.0, currentSize.width(), currentSize.height(), painter);
    } else { //style background color must be valid
        painter->fillRect(QRectF(QPointF(0, 0), currentSize), QBrush(s->backgroundColor()));
    }
    painter->setOpacity(oldOpacity);
}

void MStylableWidget::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MStylableWidget::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MStylableWidget::applyStyle()
{
    const MWidgetStyle *s = static_cast<const MWidgetStyle*>(style().operator ->());
    if (layoutDirection() == Qt::RightToLeft)
        setContentsMargins(
            s->paddingRight() + s->marginRight(),
            s->paddingTop() + s->marginTop(),
            s->paddingLeft() + s->marginLeft(),
            s->paddingBottom() + s->marginBottom());
    else
        setContentsMargins(
            s->paddingLeft() + s->marginLeft(),
            s->paddingTop() + s->marginTop(),
            s->paddingRight() + s->marginRight(),
            s->paddingBottom() + s->marginBottom());

    updateGeometry();
    update();
}

void MStylableWidget::registerStylableWidgetType(const QMetaObject *metaObject,
        const char *widgetAssemblyName,
        M::AssemblyType widgetAssemblyType)
{
    MStylableWidgetCreator::registerCreator(metaObject, widgetAssemblyName, widgetAssemblyType);
}
